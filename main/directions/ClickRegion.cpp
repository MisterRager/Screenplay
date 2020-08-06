#include "ClickRegion.h"

int ClickRegion::visit(const class DirectionVisitor & visitor) const{
    return visitor.visit(*this);
}
