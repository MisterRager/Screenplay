#include "PressKey.h"

int PressKey::visit(const DirectionVisitor & visitor) const {
    return visitor.visit(*this);
}
