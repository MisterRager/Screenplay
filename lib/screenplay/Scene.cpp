#include "lib/screenplay/Scene.h"
#include "lib/screenplay/operations/PerformDirections.h"
#include <filesystem>
#include <iostream>

int Scene::isActive(ScreenModel * screenMatcher) {
    if (!std::filesystem::exists(std::filesystem::path(matchTemplatePath))) {
        std::cerr << "Path for template [" << matchTemplatePath << "] does not exist!" << std::endl;

        return false;
    }

    return screenMatcher->findFeature(matchTemplatePath, matchCertainty);
}

int Scene::perform(ScreenModel * client) {
    PerformDirections performer(client);

    for (auto direction : *directions) {
        int result = direction->visit(performer);

        if (!result) {
            return result;
        }
    }

    return 1;
}

Scene::~Scene() {
    for (auto direction : *directions) {
        free(direction);
    }

    free(directions);
}
