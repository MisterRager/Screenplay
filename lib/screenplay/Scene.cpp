#include "Scene.h"

Scene::Scene(
        std::string name,
        std::string templatePath,
        double templateCertainty,
        std::shared_ptr<std::vector<std::shared_ptr<DirectionVisitable>>> directionList
) :
        name(std::move(name)),
        matchTemplatePath(std::move(templatePath)),
        matchCertainty(templateCertainty),
        directions(std::move(directionList)) {}