#ifndef SCREENPLAY_AGENT_H
#define SCREENPLAY_AGENT_H

#include <memory>

#include "ScreenModel.h"
#include "Scene.h"

class Agent {
public:
    explicit Agent(std::shared_ptr<ScreenModel> screen) : screen(std::move(screen)) {};

    /*
    * Check to see if a given current Scene is active
    */
    bool isActive(const Scene & scene);

    /**
     * Do the actions for this "Scene"
     * @return 1 if success
     */
    bool perform(const Scene & scene);
private:
    const std::shared_ptr<ScreenModel> screen;
};


#endif //SCREENPLAY_AGENT_H
