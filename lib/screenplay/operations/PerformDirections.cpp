#include "PerformDirections.h"
#include <iostream>
#include <cctype>

PerformDirections::PerformDirections(shared_ptr<ScreenModel> client)  : client(std::move(client)){}

int PerformDirections::visit(const ClickRegion & clickRegion) const {
    return client->findAndClickFeature(clickRegion.regionPath, clickRegion.regionCertainty);
}

int PerformDirections::visit(const PressKey & pressKey) const {
    return client->sendKey(pressKey.key);
}

int PerformDirections::visit(const EnterText &enterText) const {
    std::cerr << "Entering text... " << enterText.text << std::endl;

    for(auto c : enterText.text) {
        std::cerr << "Character: " << c << std::endl;

        switch(c) {
            // Squash these two... because the protocol mostly does
            case '\n':
            case '\r':
                if (!client->sendKey(XK_Return)) {
                    std::cerr << "Error entering Newline/Return" << std::endl;
                    return 0;
                }
                break;
            default:
                if (isalpha(c) && isupper(c)) {
                    if (!client->sendModifiedKey(c, { XK_Shift_L })) {
                        std::cerr << "Error entering uppercase character: [" << c << "]" << std::endl;
                    }
                } else  if (!client->sendKey(c)) {
                    std::cerr << "Error entering character: [" << c << "]" << std::endl;
                    return 0;
                }
                break;
        }
    }
    return 1;
}
