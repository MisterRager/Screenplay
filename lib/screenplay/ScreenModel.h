#ifndef SCREENPLAY_SCREENMODEL_H
#define SCREENPLAY_SCREENMODEL_H

#include <memory>
#include <rfb/rfbclient.h>
#include <opencv2/imgproc.hpp>
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
    uint8_t *buffer;

    ScreenBuffer(int width, int height);

    ~ScreenBuffer();
};


class ScreenModel {
private:
    shared_ptr<rfbClient> rfb;
    unique_ptr<ScreenBuffer> screenBuffer;

    void readScreen();

    MatchInfo findTemplate(const std::string &templatePath, int matchMethod = TM_SQDIFF);

public:
    explicit ScreenModel(shared_ptr<rfbClient> rfbClient);

    int saveScreen(FILE *path);

    int findFeature(const std::string &featurePath, double minimumConfidence);

    int findAndClickFeature(const std::string &featurePath, double minimumConfidence);

    int sendKey(uint32_t key);

    int sendModifiedKey(uint32_t key, const vector<uint32_t> & modifiers);

    int clickLocation(int clickX, int clickY);
};


#endif //SCREENPLAY_SCREENMODEL_H
