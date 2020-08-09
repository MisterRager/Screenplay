#include "lib/screenplay/Scene.h"
#include "lib/screenplay/operations/PerformDirections.h"
#include <filesystem>
#include <iostream>

Scene::~Scene() {
    for (auto direction : *directions) {
        free(direction);
    }

    free(directions);
}
