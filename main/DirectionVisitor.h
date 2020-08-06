//
// Created by guile on 7/26/20.
//

#ifndef SCREENPLAY_DIRECTIONVISITOR_H
#define SCREENPLAY_DIRECTIONVISITOR_H

class PressKey;
class ClickRegion;

class DirectionVisitor {
public:
    virtual int visit(const class PressKey & pressKey) const = 0;
    virtual int visit(const class ClickRegion & clickRegion) const = 0;
};


#endif //SCREENPLAY_DIRECTIONVISITOR_H
