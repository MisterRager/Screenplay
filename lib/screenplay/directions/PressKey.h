#ifndef SCREENPLAY_PRESSKEY_H
#define SCREENPLAY_PRESSKEY_H


#include <cstdint>
#include "lib/screenplay/DirectionVisitable.h"
#include "lib/screenplay/DirectionVisitor.h"

class PressKey : public DirectionVisitable {
public:
    explicit PressKey(uint32_t key): DirectionVisitable(), key(key) {}

    [[nodiscard]] int visit(const DirectionVisitor & visitor) const override;

    uint32_t key;
};


#endif //SCREENPLAY_PRESSKEY_H
