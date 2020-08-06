//
// Created by guile on 7/26/20.
//

#ifndef SCREENPLAY_PRINTDIRECTIONS_H
#define SCREENPLAY_PRINTDIRECTIONS_H

#include "../ScreenModel.h"
#include "../DirectionVisitor.h"
#include "../directions/ClickRegion.h"
#include "../directions/PressKey.h"


class PrintDirections : public DirectionVisitor {
public:
    int visit(const class ClickRegion & clickRegion) const override;
    int visit(const class PressKey & pressKey) const override;
};


#endif //SCREENPLAY_PRINTDIRECTIONS_H
