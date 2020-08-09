#include "ClickRegion.h"

int ClickRegion::visit(const class DirectionVisitor &visitor) const {
    return visitor.visit(*this);
}

ClickRegion::ClickRegion(
        std::string path,
        double certainty
) : DirectionVisitable(),
    regionPath(std::move(path)),
    regionCertainty(certainty) {}
