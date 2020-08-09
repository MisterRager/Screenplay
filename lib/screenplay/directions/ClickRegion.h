#ifndef SCREENPLAY_CLICKREGION_H
#define SCREENPLAY_CLICKREGION_H

#include <string>
#include <utility>

#include "lib/screenplay/DirectionVisitable.h"
#include "lib/screenplay/DirectionVisitor.h"

class ClickRegion : public DirectionVisitable {
public:
    ClickRegion(std::string path, double certainty);

    [[nodiscard]] int visit(const class DirectionVisitor & visitor) const override;

    const std::string regionPath;
    const double regionCertainty;
};


#endif //SCREENPLAY_CLICKREGION_H
