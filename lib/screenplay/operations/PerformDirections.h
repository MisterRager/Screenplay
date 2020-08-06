#ifndef SCREENPLAY_PERFORMDIRECTIONS_H
#define SCREENPLAY_PERFORMDIRECTIONS_H


#include "../ScreenModel.h"
#include "../DirectionVisitor.h"
#include "../directions/ClickRegion.h"
#include "../directions/PressKey.h"

class PerformDirections : public DirectionVisitor {
private:
    ScreenModel *client;
public:
    explicit PerformDirections(ScreenModel * client) : client(client){};
    int visit(const ClickRegion & clickRegion) const override;
    int visit(const PressKey & pressKey) const override;
};


#endif //SCREENPLAY_PERFORMDIRECTIONS_H
