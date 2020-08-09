#ifndef SCREENPLAY_SCRIPTPARSER_H
#define SCREENPLAY_SCRIPTPARSER_H

#include <string>

#include <yaml-cpp/yaml.h>

#include "Scene.h"

/**
 * Parse a "script" - a YAML document describing a list of "Scene" objects
 */
class ScriptParser {
public:
    std::vector<Scene *> *parseScreenplay(const std::string &screenplayPathStr);

private:
    static std::shared_ptr<std::vector<std::shared_ptr<DirectionVisitable>>> parseDirections(
            const std::string &basePath,
            const YAML::Node &directionsValue
    );

    static shared_ptr<ClickRegion> parseClickTarget(
            const std::string &basePath,
            const YAML::Node &clickTargetValue
    );
};


#endif //SCREENPLAY_SCRIPTPARSER_H
