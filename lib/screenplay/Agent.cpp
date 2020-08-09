//
// Created by guile on 8/9/20.
//

#include "Agent.h"
#include <filesystem>
#include <iostream>
#include "operations/PerformDirections.h"

Agent::Agent(std::shared_ptr<ScreenModel> screen) : screen(std::move(screen)) {}

bool Agent::isActive(const Scene &scene) {
    if (!std::filesystem::exists(std::filesystem::path(scene.matchTemplatePath))) {
        std::cerr << "Path for template [" << scene.matchTemplatePath << "] does not exist!" << std::endl;
        return false;
    }

    return screen->findFeature(scene.matchTemplatePath, scene.matchCertainty);
}

bool Agent::perform(const Scene &scene) {
    PerformDirections performer(screen);

    for (const auto &direction : *scene.directions) {
        if (!direction->visit(performer)) {
            std::cerr << "Error performing direction!" << std::endl;
            return true;
        }
    }

    return false;
}
