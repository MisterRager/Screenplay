#ifndef SCREENPLAY_PRINTDIRECTIONS_H
#define SCREENPLAY_PRINTDIRECTIONS_H

#include "../ScreenModel.h"
#include "../DirectionVisitor.h"
#include "../directions/ClickRegion.h"
#include "../directions/PressKey.h"
#include "../directions/EnterText.h"


class PrintDirections : public DirectionVisitor {
public:
    int visit(const class ClickRegion & clickRegion) const override;
    int visit(const class PressKey & pressKey) const override;
    int visit(const class EnterText & enterText) const override;
};


#endif //SCREENPLAY_PRINTDIRECTIONS_H
