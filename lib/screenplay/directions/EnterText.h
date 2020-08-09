#ifndef SCREENPLAY_ENTERTEXT_H
#define SCREENPLAY_ENTERTEXT_H

#include <string>
#include <utility>

#include "lib/screenplay/DirectionVisitable.h"
#include "lib/screenplay/DirectionVisitor.h"

class EnterText : public DirectionVisitable {
public:
    explicit EnterText(std::string text) : text(std::move(text)) {};
    std::string text;

    int visit(const class DirectionVisitor &visitor) const override;
};


#endif //SCREENPLAY_ENTERTEXT_H
