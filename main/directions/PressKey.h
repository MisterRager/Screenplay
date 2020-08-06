#ifndef SCREENPLAY_PRESSKEY_H
#define SCREENPLAY_PRESSKEY_H


#include <cstdint>
#include "main/DirectionVisitable.h"
#include "main/DirectionVisitor.h"

class PressKey : public DirectionVisitable {
public:
    explicit PressKey(uint32_t key): DirectionVisitable(), key(key) {}

    virtual int visit(const DirectionVisitor & visitor) const override;

    uint32_t key;
};


#endif //SCREENPLAY_PRESSKEY_H
