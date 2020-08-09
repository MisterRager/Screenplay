#ifndef SCREENPLAY_SCENE_H
#define SCREENPLAY_SCENE_H

#include <utility>
#include <vector>
#include <memory>

#include <rfb/rfbclient.h>

#include "ScreenModel.h"
#include "DirectionVisitable.h"

class Scene {
public:
    Scene(
            std::string name,
            std::string templatePath,
            double templateCertainty,
            std::shared_ptr<std::vector<std::shared_ptr<DirectionVisitable>>> directionList
    );

    const std::string name;
    const std::string matchTemplatePath;
    double matchCertainty;
    std::shared_ptr<std::vector<std::shared_ptr<DirectionVisitable>>> directions;
};


#endif //SCREENPLAY_SCENE_H
