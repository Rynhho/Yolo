#pragma once

#include "Domain/Criterion/Criterion.hpp"

#include "Domain/Graph/Graph.hpp"

#include "Domain/Solution/Solution.hpp"

namespace Yolo
{
    class SimilarSizeCriterion : public Criterion
    {
    public:
        SimilarSizeCriterion(int maxSlack);
        SimilarSizeCriterion(double percentage);

        virtual bool evaluate(Graph graph, Solution solution) const override;

    private:
        bool mUseIntSlack;

        int mIntSlack = -1;
        double mPercentageSlack = -1;
    };
} // namespace Yolo