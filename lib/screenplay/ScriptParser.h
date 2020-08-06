#ifndef SCREENPLAY_SCRIPTPARSER_H
#define SCREENPLAY_SCRIPTPARSER_H

#include <string>

#include <yaml-cpp/yaml.h>

#include "Scene.h"
#include "directions/ClickRegion.h"
#include "directions/PressKey.h"

/**
 * Parse a "script" - a YAML document describing a list of "Scene" objects
 */
class ScriptParser {
public:
    std::vector<Scene *> *parseScreenplay(const std::string &screenplayPathStr);

private:
    static std::vector<DirectionVisitable *> *
    parseDirections(const std::string &basePath, const YAML::Node &directionsValue);

    static ClickRegion *parseClickTarget(const std::string &basePath, const YAML::Node &clickTargetValue);
};


#endif //SCREENPLAY_SCRIPTPARSER_H
