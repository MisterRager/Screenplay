#include "ScriptParser.h"

#include <cfloat>
#include <cmath>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

std::vector<Scene *> *ScriptParser::parseScreenplay(const std::string &screenplayPathStr) {
    auto results = new std::vector<Scene *>;
    YAML::Node config = YAML::LoadFile(screenplayPathStr);

    fs::path screenplayPathPath(screenplayPathStr);

    if (config.IsSequence()) {
        for (const auto &scene : config) {
            if (scene.IsMap()) {
                bool hasName, hasPath, hasConfidence;
                hasName = false;
                hasPath = false;
                hasConfidence = false;

                std::string name, path;
                double confidence = 0;
                std::vector<DirectionVisitable *> *directions = nullptr;

                for (const auto& keyValue : scene) {
                    auto key = keyValue.first.as<std::string>();

                    if (key == "step") {
                        name = keyValue.second.as<std::string>();
                        hasName = true;
                    } else if (key == "image") {
                        path = screenplayPathPath
                                .parent_path()
                                .append(keyValue.second.as<std::string>())
                                .u8string();

                        hasPath = true;
                    } else if (key == "confidence") {
                        confidence = keyValue.second.as<double>();
                        hasConfidence = true;
                    } else if (key == "directions") {
                        directions = parseDirections(keyValue.second);
                    }
                }

                if (hasName && hasPath && hasConfidence) {
                    results->push_back(
                            new Scene(
                                    name,
                                    path,
                                    confidence,
                                    directions
                            )
                    );
                } else if (directions) {
                    free(directions);
                }
            }
        }
    }

    return results;
}

std::vector<DirectionVisitable *> *ScriptParser::parseDirections(const YAML::Node& directionsValue) {
    auto directions = new std::vector<DirectionVisitable *>;

    for (const auto &directionNode: directionsValue) {
        if (directionNode.IsMap()) {
            for (const auto &dirKV: directionNode) {
                auto key = dirKV.first.as<std::string>();

                if (key == "clickTarget") {
                    auto clickAction = parseClickTarget(dirKV.second);
                    if (clickAction) {
                        directions->push_back(clickAction);
                    } else {
                        // Invalid YAML - the click isn't properly formed
                        std::stringstream errString;
                        auto mark = dirKV.second.Mark();
                        errString << "Malformed clickTarget object at line " << mark.line << ", col " << mark.column << endl;

                        throw std::runtime_error(errString.str());
                    }
                } else if (key == "keyboardInput") {
                    auto input = dirKV.second.as<std::string>();

                    for (char const &c: input) {
                        directions->push_back(new PressKey(0xFF00 | std::abs(c)));
                    }
                }
            }
        }
    }

    return directions;
}

ClickRegion *ScriptParser::parseClickTarget(const YAML::Node& clickTargetValue) {
    if (clickTargetValue.IsMap()) {
        std::string image;
        auto confidence = DBL_MAX; // Default: unreasonably high confidence, the highest possible double
        bool hasImage = false, hasConfidence = false;

        for (const auto &kv: clickTargetValue) {
            auto key = kv.first.as<std::string>();

            if (key == "image") {
                image = kv.second.as<std::string>();
                hasImage = true;
            } else if (key == "confidence") {
                confidence = kv.second.as<double>();
                hasConfidence = true;
            }
        }

        if (hasImage && hasConfidence) {
            return new ClickRegion(image, confidence);
        }
    }
    return nullptr;
}
