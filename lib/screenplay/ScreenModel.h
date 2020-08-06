#ifndef SCREENPLAY_SCREENMODEL_H
#define SCREENPLAY_SCREENMODEL_H

#include <memory>
#include <rfb/rfbclient.h>
#include <opencv2/imgcodecs.hpp>
#include <turbojpeg.h>
#include <cstdio>

using namespace cv;
using namespace std;

typedef struct {
    Point location;
    int width;
    int height;
    double confidence;
} MatchInfo;

class ScreenBuffer {
public:
    int width;
    int height;
    uint8_t * buffer;

    ScreenBuffer(int width, int height);
    ~ScreenBuffer();
};


class ScreenModel {
private:
    rfbClient * rfb;
    ScreenBuffer * screenBuffer;

    void readScreen();

    MatchInfo findTemplate(const std::string& templatePath, int matchMethod);
    MatchInfo findTemplate(const std::string& templatePath);

public:
    explicit ScreenModel(rfbClient * rfbClient);
    ~ScreenModel();

    int saveScreen(FILE * path);

    int findFeature(std::string featurePath, double minimumConfidence);
    int findAndClickFeature(std::string featurePath, double minimumConfidence);
    int sendKey(uint32_t key);
    int clickLocation(int clickX, int clickY);
};


#endif //SCREENPLAY_SCREENMODEL_H
