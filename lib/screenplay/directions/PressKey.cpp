#include "PressKey.h"

PressKey::PressKey(uint32_t key) : DirectionVisitable(), key(key) {}

int PressKey::visit(const DirectionVisitor &visitor) const {
    return visitor.visit(*this);
}
