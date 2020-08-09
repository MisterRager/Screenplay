#include "EnterText.h"

int EnterText::visit(const class DirectionVisitor & visitor) const {
    return visitor.visit(*this);
}
