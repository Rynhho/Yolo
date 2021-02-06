#pragma once

#include "Domain/Neighborhood/Neighborhood.hpp"

namespace Yolo
{
    class PickNDropNeighborhood : public Neighborhood
    {
    public:
        PickNDropNeighborhood() {}

        virtual std::vector<Solution> generate(Solution solution) const override;
    };
} // namespace Yolo