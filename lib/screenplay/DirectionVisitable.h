#ifndef SCREENPLAY_DIRECTIONVISITABLE_H
#define SCREENPLAY_DIRECTIONVISITABLE_H

#include "lib/screenplay/DirectionVisitor.h"

class DirectionVisitable {
public:
    virtual int visit(const DirectionVisitor & visitor) const = 0;
};

#endif //SCREENPLAY_DIRECTIONVISITABLE_H
