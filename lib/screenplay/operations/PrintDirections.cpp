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
    std::cout << (pressKey.key) + 0 << std::endl;
    return 1;
}

int PrintDirections::visit(const class EnterText & enterText) const {
    std::cout << "EnterText" << endl;
    std::cout << enterText.text << endl;
    return 1;
}
