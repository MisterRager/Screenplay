//
// Created by guile on 7/26/20.
//

#include "lib/screenplay/operations/PrintDirections.h"
#include "lib/screenplay/directions/ClickRegion.h"
#include <iostream>

int PrintDirections::visit(const class ClickRegion & clickRegion) const {
    std::cout << "ClickRegion " << endl;
    std::cout << clickRegion.regionPath << std::endl;
    return 1;
}

int PrintDirections::visit(const class PressKey & pressKey) const {
    std::cout << "PressKey " << endl;
    std::cout << (pressKey.key & 0x00FF) << std::endl;
    return 1;
}
