#ifndef SCREENPLAY_SCENE_H
#define SCREENPLAY_SCENE_H

#include <utility>
#include <vector>

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
            vector<DirectionVisitable *> * directionList
    ) :
    name(std::move(name)),
    matchTemplatePath(std::move(templatePath)),
    matchCertainty(templateCertainty),
    directions(directionList)
    {};

    ~Scene();

    std::string name;
    std::string matchTemplatePath;
    double matchCertainty;
    vector<DirectionVisitable *> * directions;
    /*
     * Check to see if the current "Scene" is active. Return 1 if true, 0 if not.
     */
    int isActive(ScreenModel * screenMatcher);

    /**
     * Do the actions for this "Scene"
     * @return 1 if success
     */
    int perform(ScreenModel * client);
};


#endif //SCREENPLAY_SCENE_H
