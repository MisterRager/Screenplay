#include "ScreenModel.h"

#include <iostream>
#include <filesystem>
#include <rfb/keysym.h>

#define DUMMY_CLICK_LOCATION 0x07FFF

MatchInfo ScreenModel::findTemplate(const std::string& templatePath, int matchMethod) {
    readScreen();

    //std::cerr << "Try to read screen as a Mat" << std::endl;

    cv::Mat screen(
            cv::Size(rfb->width, rfb->height),
            CV_8UC3,
            screenBuffer->buffer,
            cv::Mat::AUTO_STEP
    );

    // color in OpenCV is BGR - convert the RGB expected from ScreenModel
    cv::cvtColor(screen, screen, COLOR_RGB2BGR);
    //cv::imwrite("lastframe_cv.png", screen, {IMWRITE_PNG_COMPRESSION, 100});

    //std::cerr << "Now read path [" << templatePath << "]" << std::endl;
    Mat matchMe = imread(templatePath, IMREAD_COLOR);
    //cv::imwrite("lasttemplate_cv.png", matchMe);

    //std::cerr << "red." << std::endl;

    int resultColumns = screen.cols - matchMe.cols + 1;
    int resultRows = screen.rows - matchMe.rows + 1;

    if (resultColumns < 1 || resultRows < 1) {
        std::cerr << "Not enough space to search, bail" << std::endl;

        MatchInfo nope;

        nope.location = Point(-1, -1);
        nope.confidence = -1.0;
        nope.height = -1;
        nope.width = -1;

        return nope;
    }

    Mat result(resultRows, resultColumns, CV_32FC1);

    //std::cerr << "Matching, now." << std::endl;

    matchTemplate(screen, matchMe, result, matchMethod);

    //std::cerr << "Matched." << std::endl;

    double minVal, maxVal;
    Point minLoc, maxLoc;
    MatchInfo results;

    //std::cerr << "Find the min/max values based on matching" << std::endl;

    minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

    //std::cerr << "Ok, interpret." << std::endl;

    switch (matchMethod) {
        case TM_SQDIFF_NORMED:
        case TM_SQDIFF:
            results.confidence = (matchMe.cols * matchMe.rows * 3.0) / minVal;
            results.location = minLoc;
            break;
        default:
            results.location = maxLoc;
            results.confidence = maxVal;
    }

    results.width = matchMe.cols;
    results.height = matchMe.rows;

    //std::cerr << "Return matching results" << std::endl;

    return results;
}

ScreenBuffer::ScreenBuffer(int width, int height) {
    this->width = width;
    this->height = height;
    buffer = (uint8_t *) malloc( width * height * sizeof(uint8_t) * 3);
}

ScreenBuffer::~ScreenBuffer() {
    free(buffer);
}

void ScreenModel::readScreen() {
    int width, height;

    width = rfb->width;
    height = rfb->height;

    if (screenBuffer->width != width || screenBuffer->height != height) {
        free(screenBuffer);
        screenBuffer = new ScreenBuffer(width, height);
    }

     /*
    cerr <<
    "Update frame from [" << x << ", " << y << "] width " << w << " height " << h <<
    " Screen: " << rfb->desktopName <<
    endl;
     */

    uint8_t * currentPixel = screenBuffer->buffer;

    // Read Frame info
    rfbPixelFormat * pf = &rfb->format;
    int bpp = pf->bitsPerPixel / 8;

    // TODO: just use the same buffer or at least do this with memcpy instead of loops
    for (int j = 0, endY = height; j < endY; j++) {
        // Number of bytes in the rows preceding this one
        int rowOffset = j * width * bpp;

        for (int k = 0, endX = width; k < endX; k++) {
            int offset = k * bpp;

            unsigned int pixelData;
            void * rawPixel = rfb->frameBuffer + rowOffset + offset;

            switch(bpp) {
                case 4:
                    pixelData = *(unsigned int *) rawPixel;
                    break;
                case 2:
                    pixelData =  *(unsigned short *) rawPixel;
                    break;
                default:
                    pixelData = *(unsigned char *) rawPixel;
                    break;
            }

            // red
            *currentPixel = (((pixelData >> pf->redShift) * 256 / (pf->redMax + 1)) & 255);
            currentPixel += 1; // advance to next
            // green
            *currentPixel = (((pixelData >> pf->greenShift) * 256 / (pf->greenMax + 1)) & 255);
            currentPixel += 1;
            // blue
            *currentPixel = (((pixelData >> pf->blueShift) * 256 / (pf->blueMax + 1)) & 255);
            currentPixel += 1;
        }
    }
}

int ScreenModel::saveScreen(FILE * file) {
    readScreen();
    uint8_t * srcBuf = screenBuffer->buffer;

    tjhandle handle = tjInitCompress();

    if(!handle) {
        const char *err = (const char *) tjGetErrorStr();
        cerr << "TJ Error: " << err << " UNABLE TO INIT TJ Compressor Object" << endl;
        return -1;
    }

    int jpegQual = 100;
    int flags = 0;
    int nbands = 3;

    unsigned char* jpegBuf = nullptr;

    int width = rfb->width;
    int height = rfb->height;

    int pitch = width * nbands;
    int pixelFormat = TJPF_RGB;
    int jpegSubsamp = TJSAMP_411;

    unsigned long jpegSize = 0;

    int res = tjCompress2(
            handle,
            srcBuf,
            width,
            pitch,
            height,
            pixelFormat,
            &(jpegBuf),
            &jpegSize,
            jpegSubsamp,
            jpegQual,
            flags
    );

    if(res != 0) {
        const char *err = (const char *) tjGetErrorStr();
        cerr << "TurboJPEG Error: " << err << " UNABLE TO COMPRESS JPEG IMAGE" << endl;
        tjDestroy(handle);
        return -1;
    }

    if (fwrite(jpegBuf, jpegSize, 1, file) < 1) {
        cerr << "Could not write JPEG file: " << strerror(errno);
        return -1;
    }

    //write out the compress date to the image file
    //cleanup
    res = tjDestroy(handle); //should deallocate data buffer

    if(res != 0) {
        const char *err = (const char *) tjGetErrorStr();
        cerr << "TurboJPEG Error: " << err << " UNABLE TO PROPERLY CLEAN UP" << endl;
        tjDestroy(handle);
        return -1;
    }

    return 0;
}

ScreenModel::ScreenModel(rfbClient *rfbClient) {
    this->rfb = rfbClient;
    this->screenBuffer = new ScreenBuffer(rfbClient->width, rfbClient->height);
}

ScreenModel::~ScreenModel() {
    free(screenBuffer);
}

static int matches(const MatchInfo& match, double minimumConfidence) {
    std::cerr << "matching... how confident? " << match.confidence << std::endl;
    return match.confidence >= minimumConfidence;
}

int ScreenModel::findFeature(const std::string& featurePath, double minimumConfidence) {
    return matches(findTemplate(featurePath), minimumConfidence);
}

int ScreenModel::findAndClickFeature(const std::string& featurePath, double minimumConfidence) {
    if (!std::filesystem::exists(std::filesystem::path(featurePath))) {
        std::cerr << "Nonexistent Feature File: " << featurePath << std::endl;
        return 0;
    }

    MatchInfo match = findTemplate(featurePath);

    if (!matches(match, minimumConfidence)) {
        return 0;
    }

    cerr << "Click on the item " << featurePath << " from confidence " << match.confidence << endl;
    int clickX, clickY;

    clickX = match.location.x + match.width / 2;
    clickY = match.location.y + match.height / 2;

    return clickLocation(clickX, clickY);
}

int ScreenModel::clickLocation(int clickX, int clickY) {
    cerr << "Click on location [" << clickX << ", " << clickY << "]" << endl;

    int ret;

    // Prep for motion by wiggling:
    ret = SendPointerEvent(
            rfb,
            0,
            0,
            DUMMY_CLICK_LOCATION
    );

    if (!ret) {
        cerr << "Failure for the prep motion: move to top left" << endl;
        return 0;
    }

    ret = SendPointerEvent(
            rfb,
            rfb->width,
            rfb->height,
            DUMMY_CLICK_LOCATION
    );

    if (!ret) {
        cerr << "Failure for the prep motion: move to bottom right" << endl;
        return 0;
    }
    // move to where the click will happen
    ret = SendPointerEvent(
            rfb,
            clickX,
            clickY,
            0
    );

    if (!ret) {
        cerr << "Failure for the prep motion: move to the click location" << endl;
        return 0;
    }

    cerr << "Clicking at [" << clickX << ", " << clickY << "]" << endl;

    ret = SendPointerEvent(
            rfb,
            clickX,
            clickY,
            rfbButton1Mask
    );

    if (!ret) {
        cerr << "Failure for the click mousedown" << endl;
        return 0;
    }

    ret = SendPointerEvent(
            rfb,
            DUMMY_CLICK_LOCATION,
            DUMMY_CLICK_LOCATION,
            0
    );

    if (!ret) {
        cerr << "Failure for the click mouseup" << endl;
        return 0;
    }

    return 1;
}

int ScreenModel::sendKey(uint32_t key) {
     int ret = SendKeyEvent(rfb, key, true);

    if (!ret) {
        cerr << "Failure to send key down " << key << endl;
        return 0;
    }

    ret = SendKeyEvent(rfb, key, false);

    if (!ret) {
        cerr << "Failure to send key up " << key << endl;
        return 0;
    }

    return 1;
}
