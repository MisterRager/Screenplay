#ifndef SCREENPLAY_CLICKREGION_H
#define SCREENPLAY_CLICKREGION_H

#include <string>
#include <utility>

#include "main/DirectionVisitable.h"
#include "main/DirectionVisitor.h"

class ClickRegion : public DirectionVisitable {
public:
    ClickRegion(std::string path, double certainty) : DirectionVisitable(), regionPath(std::move(path)), regionCertainty(certainty) {}

    int visit(const class DirectionVisitor & visitor) const override;
    std::string regionPath;
    double regionCertainty;
};


#endif //SCREENPLAY_CLICKREGION_H
