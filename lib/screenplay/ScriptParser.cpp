#include "ScriptParser.h"

#include <filesystem>
#include <cfloat>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include <memory>

#include "directions/ClickRegion.h"
#include "directions/PressKey.h"
#include "directions/EnterText.h"

namespace fs = std::filesystem;

std::vector<Scene *> *ScriptParser::parseScreenplay(const std::string &screenplayPathStr) {
    auto results =  new std::vector<Scene *>;
    YAML::Node config = YAML::LoadFile(screenplayPathStr);

    fs::path playPath(screenplayPathStr);

    if (config.IsSequence()) {
        for (const auto &scene : config) {
            if (scene.IsMap()) {
                bool hasName, hasPath, hasConfidence, hasDirections;
                hasName = false;
                hasPath = false;
                hasConfidence = false;
                hasDirections = false;

                std::string name, path;
                double confidence = 0;
                shared_ptr<std::vector<shared_ptr<DirectionVisitable>>> directions = nullptr;

                for (const auto &keyValue : scene) {
                    auto key = keyValue.first.as<std::string>();

                    if (key == "step") {
                        name = keyValue.second.as<std::string>();
                        hasName = true;
                    } else if (key == "image") {
                        path = playPath.parent_path().append(keyValue.second.as<std::string>()).u8string();
                        hasPath = true;
                    } else if (key == "confidence") {
                        confidence = keyValue.second.as<double>();
                        hasConfidence = true;
                    } else if (key == "directions") {
                        directions = parseDirections(playPath.parent_path().u8string(), keyValue.second);
                        hasDirections = true;
                    }
                }

                if (hasName && hasPath && hasConfidence && hasDirections) {
                    results->push_back(
                            new Scene(
                                    name,
                                    path,
                                    confidence,
                                    directions
                            )
                    );
                }
            }
        }
    }

    return results;
}

std::shared_ptr<std::vector<shared_ptr<DirectionVisitable>>> ScriptParser::parseDirections(
        const std::string &basePath,
        const YAML::Node &directionsValue
) {
    auto directions = make_shared<std::vector<std::shared_ptr<DirectionVisitable>>>();

    for (const auto &directionNode: directionsValue) {
        if (directionNode.IsMap()) {
            for (const auto &dirKV: directionNode) {
                auto key = dirKV.first.as<std::string>();

                if (key == "clickTarget") {
                    auto clickAction = parseClickTarget(basePath, dirKV.second);
                    if (clickAction) {
                        directions->push_back(clickAction);
                    } else {
                        // Invalid YAML - the click isn't properly formed
                        std::stringstream errString;
                        auto mark = dirKV.second.Mark();
                        errString << "Malformed clickTarget object at line " << mark.line << ", col " << mark.column
                                  << endl;

                        throw std::runtime_error(errString.str());
                    }
                } else if (key == "keyboardInput") {
                    auto input = dirKV.second.as<std::string>();

                    for (char const &c: input) {
                        directions->push_back(make_shared<PressKey>(0xFF00 | std::abs(c)));
                    }
                } else if (key == "enterText") {
                    std::cerr << directions->size() << " directions becomes..." << std::endl;
                    directions->push_back(make_shared<EnterText>(dirKV.second.as<std::string>()));
                }
            }
        }
    }

    return directions;
}

shared_ptr<ClickRegion> ScriptParser::parseClickTarget(
        const std::string &basePath,
        const YAML::Node &clickTargetValue
) {
    if (clickTargetValue.IsMap()) {
        std::string image;
        auto confidence = DBL_MAX; // Default: unreasonably high confidence, the highest possible double
        bool hasImage = false, hasConfidence = false;

        for (const auto &kv: clickTargetValue) {
            auto key = kv.first.as<std::string>();

            if (key == "image") {
                image = fs::path(basePath).append(
                        kv.second.as<std::string>()).u8string(); // append supplied path to base
                hasImage = true;
            } else if (key == "confidence") {
                confidence = kv.second.as<double>();
                hasConfidence = true;
            }
        }

        if (hasImage && hasConfidence) {
            return make_shared<ClickRegion>(image, confidence);
        }
    }
    return nullptr;
}
