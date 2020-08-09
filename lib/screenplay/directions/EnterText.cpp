#include "EnterText.h"

EnterText::EnterText(std::string text) : text(std::move(text)) {}

int EnterText::visit(const class DirectionVisitor &visitor) const {
    return visitor.visit(*this);
}
