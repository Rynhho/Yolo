#include "ImplicitEnumerationAlgorithm.hpp"

#include "Core/Logger/Logger.hpp"

namespace Yolo
{
    ImplicitEnumerationAlgorithm::ImplicitEnumerationAlgorithm(const Graph& graph, int nbClasses)
        : Algorithm(graph, nbClasses)
    {
        mBestSolution = Solution(mGraph.getNbVertices(), mNbClasses);
        mIsBestSolutionValid = false;
        mBestSolutionCost = mGraph.getSolutionCost(mBestSolution);
    }

    std::optional<Solution> ImplicitEnumerationAlgorithm::solve()
    {
        Solution solution = Solution(mGraph.getNbVertices(), mNbClasses);
        enumerateFrom(solution, 0, 0);

        if (!mCriterion->evaluate(mGraph, mBestSolution))
        {
            YOLO_DEBUG("ImplicitEnumerationAlgorithm::solve(): Infeasible instance.\n");
            return std::nullopt;
        }

        return mBestSolution;
    }

    void ImplicitEnumerationAlgorithm::enumerateFrom(Solution& solution, int vertex, double cost)
    {
        if (vertex == mGraph.getNbVertices())
        {
            if (mCriterion->evaluate(mGraph, solution))
            {
                if (!mIsBestSolutionValid)
                {
                    mIsBestSolutionValid = true;
                    mBestSolution = solution;
                    mBestSolutionCost = cost;
                }
                else
                {
                    if (cost < mBestSolutionCost)
                    {
                        mBestSolution = solution;
                        mBestSolutionCost = cost;
                    }
                }
            }
        }
        else
        {
            int previousClass = solution.getVertexClass(vertex);

            for (int i = 0; i <= std::min(vertex, mNbClasses - 1); ++i)
            {
                cost = cost + mGraph.getSolutionCostDifference(solution, vertex, i);
                solution.setVertexClass(vertex, i);

                if (mCriterion->evaluate(mGraph, solution, true))
                {
                    enumerateFrom(solution, vertex + 1, cost);
                }
            }

            solution.setVertexClass(vertex, previousClass);
        }
    }
} // namespace Yolo
