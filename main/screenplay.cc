#include <iostream>
#include <cstring>
#include <string>
#include <cerrno>
#include <ctime>
#include <thread>
#include <chrono>

#include <rfb/rfbclient.h>

#include "lib/screenplay/ScriptParser.h"
#include "lib/screenplay/ScreenModel.h"
#include "lib/screenplay/Scene.h"
#include "lib/screenplay/DirectionVisitable.h"

#include "lib/screenplay/directions/ClickRegion.h"
#include "lib/screenplay/directions/PressKey.h"
#include "lib/screenplay/operations/PerformDirections.h"
#include "lib/screenplay/operations/PrintDirections.h"
#include "lib/screenplay/Agent.h"

using namespace cv;
using namespace std;

typedef struct {
    int frameCount;
    int lastSavedFrame;
    int lastRulesFrame;
    shared_ptr<ScreenModel> screenModel;
} CurrentVncState;

/*
  Convenience function: get the client data for the current vnc session.
 */
static CurrentVncState *getAppState(rfbClient *client) {
    return (CurrentVncState *) rfbClientGetClientData(client, client);
}

static int writeFrameToFile(rfbClient *client, char *framePath) {
    FILE *file = fopen(framePath, "wb");

    if (!file) {
        cerr << "Could not open JPEG file! " << strerror(errno) << endl;
        return -1;
    }

    CurrentVncState *state = getAppState(client);

    int result = state->screenModel->saveScreen(file);
    fclose(file);

    return result;
}

const char *FRAME_FILENAME = "lastframe.jpg";

void loop(shared_ptr<rfbClient> client, shared_ptr<vector<shared_ptr<Scene>>> script) {
    int quitting = 0;

    auto executeScript = [&client, &quitting, &script]() {
        CurrentVncState *state = getAppState(client.get());
        int currentStep = -1;

        auto agent = make_unique<Agent>(state->screenModel);

        do {
            cerr << "Current Stage: " << currentStep << endl;
            if (state->frameCount <= state->lastRulesFrame) {
                cerr << "No updates on frame - wait a second" << endl;

                int result = SendFramebufferUpdateRequest(
                        client.get(),
                        0, 0,
                        client->width, client->height,
                        false // not incremental
                );

                if (!result) {
                    cerr << "Error requesting update" << endl;
                }

                state->lastRulesFrame = state->frameCount;
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                continue;
            } else {
                cerr << "Evaluate rules on frame " << state->frameCount << endl;
            }

            int oldStep = currentStep;

            if (currentStep == -1) {
                for (int k = 0, stepCount = script->size(); k < stepCount && k != currentStep; k++) {
                    std::cerr << "Check if screen [" << k << "] is active" << std::endl;
                    if (agent->isActive(*script->at(k))) {
                        std::cerr << "Screen " << k << " is active." << std::endl;
                        currentStep = k;
                    }
                }
            }

            if (currentStep == -1) {
                cerr << "Unknown Screen... wait a bit?" << endl;

                state->lastRulesFrame = state->frameCount;
                std::this_thread::sleep_for(std::chrono::milliseconds(5000));
                continue;
            }


            std::cerr << "Check if current step is active for using, now (" << currentStep << ")" << std::endl;
            if (agent->isActive(*script->at(currentStep))) {
                std::cerr << "Perfoming step " << currentStep << std::endl;
                if (agent->perform(*script->at(currentStep))) {
                    cerr << "Scene " << currentStep << " performed" << endl;
                } else {
                    cerr << "Error performing scene " << currentStep << endl;
                }
            } else {
                std::cerr << "Ok, now checking if next step is ready " << currentStep + 1 << std::endl;
                if (agent->isActive(*script->at(currentStep + 1))) {
                    currentStep += 1;
                    cerr << "Moving to step " << currentStep;
                }
            }

            state->lastRulesFrame = state->frameCount;

            if (currentStep == oldStep) {
                cerr << "No changes reported, request an update" << endl;

                int result = SendFramebufferUpdateRequest(
                        client.get(),
                        0, 0,
                        client->width, client->height,
                        false // not incremental
                );

                if (!result) {
                    cerr << "Error requesting update" << endl;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            }

        } while ((script->size() - currentStep) > 0);

        cerr << "Done with rules at stage" << currentStep << " out of " << script->size() << endl;
        quitting = 1;
    };

    auto saveFrames = [&client, &quitting]() {
        while (!quitting) {
            CurrentVncState *state = getAppState(client.get());

            if (state->frameCount > state->lastSavedFrame) {
                writeFrameToFile(client.get(), (char *) FRAME_FILENAME);
                state->lastSavedFrame = state->frameCount;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
    };

    thread scriptRunner(executeScript);
    thread frameSaver(saveFrames);

    while (!quitting) {
        if (WaitForMessage(client.get(), 50) < 0)
            break;

        if (!HandleRFBServerMessage(client.get()))
            break;
    }

    scriptRunner.join();
    frameSaver.join();
}

static void increaseFrameCounter(rfbClient *client, int x, int y, int width, int height) {
    CurrentVncState *state = getAppState(client);
    state->frameCount += 1;
}

static void printScreenplay(const vector<Scene *> &script) {
    int sceneN = 0;

    PrintDirections printer;

    for (auto scene : script) {
        cout << "Scene " << sceneN++ << endl;
        cout << "Scene name: " << scene->name << " Template image [" << scene->matchTemplatePath << "] Confidence: "
             << scene->matchCertainty << endl;
        //cout << scene->directions->size() << " directions on the scene" << std::endl;

        for (auto direction : *scene->directions) {
            cout << "Direction?" << endl;
            if (direction) {
                direction->visit(printer);
            } else {
                cout << "NULL direction :(" << endl;
            }
        }

        free(scene);
    }
}

int main(int argc, char **argv) {
    cout << "Try " << argv[1] << endl;
    shared_ptr<vector<shared_ptr<Scene>>> scenes;

    try {
        scenes = ScriptParser().parseScreenplay(argv[1]);
    } catch (std::string &error) {
        std::cerr << "Error parsing YAML: " << error << endl;
        exit(1);
    }

    //printScreenplay(*scenes);

    cerr << "Starting \"Screenplay\" remote scripting engine..." << endl;

    shared_ptr<rfbClient> vnc;
    CurrentVncState state;

    vnc = shared_ptr<rfbClient>(rfbGetClient(8, 3, 4));

    // Sign up vnc callbacks
    if (argc < 3) {
        std::cerr << "Invalid arguments - needs 3: [script] [host] [port]" << endl;
        exit(1);
    }

    vnc->GotFrameBufferUpdate = increaseFrameCounter;
    vnc->HandleTextChat = FALSE;
    vnc->HandleKeyboardLedState = FALSE;

    // init app state, attach it to the vnc client
    state.frameCount = 0;
    state.lastRulesFrame = 0;
    state.lastSavedFrame = 0;
    state.screenModel = make_shared<ScreenModel>(vnc);

    rfbClientSetClientData(vnc.get(), vnc.get(), &state);

    cerr << "Connecting!" << endl;

    int argcFake = 2;
    char *argvFake[3] = {argv[0], argv[2], argv[3]};

    if (!rfbInitClient(vnc.get(), &argcFake, argvFake)) {
        std::cerr << "Error initializing connection: " << strerror(errno) << endl;
        return 1;
    }

    cerr << "Connected!" << endl;

    loop(vnc, scenes);

    cerr << "Tearing down!" << endl;

    rfbClientCleanup(vnc.get());

    cerr << "Done!" << endl;

    return 0;
}