#ifndef SCREENPLAY_PERFORMDIRECTIONS_H
#define SCREENPLAY_PERFORMDIRECTIONS_H

#include <memory>

#include "../ScreenModel.h"
#include "../DirectionVisitor.h"
#include "../directions/ClickRegion.h"
#include "../directions/PressKey.h"
#include "../directions/EnterText.h"

class PerformDirections : public DirectionVisitor {
private:
    shared_ptr<ScreenModel> client;
public:
    explicit PerformDirections(shared_ptr<ScreenModel> client);

    [[nodiscard]] int visit(const ClickRegion & clickRegion) const override;
    [[nodiscard]] int visit(const PressKey & pressKey) const override;
    [[nodiscard]] int visit(const EnterText & enterText) const override;
};


#endif //SCREENPLAY_PERFORMDIRECTIONS_H
