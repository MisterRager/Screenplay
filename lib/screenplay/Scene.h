#ifndef SCREENPLAY_SCENE_H
#define SCREENPLAY_SCENE_H

#include <utility>
#include <vector>
#include <memory>

#include <rfb/rfbclient.h>

#include "ScreenModel.h"
#include "DirectionVisitable.h"

using namespace std;

class Scene {
public:
    Scene(
            std::string name,
            std::string templatePath,
            double templateCertainty,
            std::shared_ptr<vector<DirectionVisitable *>> directionList
    ) :
    name(std::move(name)),
    matchTemplatePath(std::move(templatePath)),
    matchCertainty(templateCertainty),
    directions(std::move(directionList))
    {};

    ~Scene();

    const std::string name;
    const std::string matchTemplatePath;
    double matchCertainty;
    std::shared_ptr<vector<DirectionVisitable *>> directions;
};


#endif //SCREENPLAY_SCENE_H
