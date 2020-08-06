//
// Created by guile on 7/26/20.
//

#include "PerformDirections.h"

int PerformDirections::visit(const ClickRegion & clickRegion) const {
    return client->findAndClickFeature(clickRegion.regionPath, clickRegion.regionCertainty);
}

int PerformDirections::visit(const PressKey & pressKey) const {
    return client->sendKey(pressKey.key);
}
