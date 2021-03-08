#include "TabuAlgorithm.hpp"

#include "Core/Logger/Logger.hpp"

#include "Domain/Neighborhood/PickNDropNeighborhood.hpp"

#include <sstream>

static const Yolo::PickNDropNeighborhood sDefaultNeighborhood;
static constexpr int sDefaultMaxIterations = 1000;
static constexpr int sDefaultTabuListSize = 100;
static constexpr bool sDefaultStoreAll = true;

namespace Yolo
{
    TabuAlgorithm::TabuAlgorithm(const Graph& graph, int nbClasses)
        : Algorithm(graph, nbClasses)
    {
        mNeighborhood = &sDefaultNeighborhood;

        mTabuListSize = sDefaultTabuListSize;
        mTabuList = std::list<Solution>();

        mStoreAll = sDefaultStoreAll;
        mMaxIterations = sDefaultMaxIterations;
    }

    std::string TabuAlgorithm::getDetails() const
    {
        std::stringstream ss;
        ss << "\n\tNumber of iterations: " << mMaxIterations;
        ss << "\n\tTabu list size: " << mTabuListSize;
        ss << "\n\tAre we storing potentially better solutions: " << (mStoreAll ? "Yes" : "No");

        return ss.str();
    }

    std::optional<Solution> TabuAlgorithm::solve()
    {
        std::optional<Solution> opt = mCriterion->generateInitialSolution(mGraph, mNbClasses);
        if (opt)
        {
            return solve(*opt);
        }

        return std::nullopt;
    }

    Solution TabuAlgorithm::solve(Solution initialSolution)
    {
        mActualSolution = initialSolution;
        mActualSolutionCost = mGraph.getSolutionCost(initialSolution);

        Solution veryBest = initialSolution;
        double veryBestCost = mActualSolutionCost;

        // Solution bestNeighbor = mNeighborhood->generateBestWithExceptions(mGraph, mTabu, mCriterion, initialSolution);
        Solution bestNeighbor = mNeighborhood->generateBest(initialSolution, mGraph, mCriterion);
        double bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);

        // YOLO_INFO("{0} {1}",bestNeighbor.toString(), bestNeighborCost);

        int iter = 0;
        while (iter < mMaxIterations)
        {
            ++iter;

            if (mStoreAll || bestNeighborCost >= mActualSolutionCost)
            {
                mTabuList.push_back(bestNeighbor);
            }

            if (mTabuList.size() > mTabuListSize)
            {
                mTabuList.pop_front();
            }

            mActualSolution = bestNeighbor;
            mActualSolutionCost = bestNeighborCost;

            if (mActualSolutionCost < veryBestCost)
            {
                veryBest = mActualSolution;
                veryBestCost = mActualSolutionCost;
            }

            bestNeighbor = mNeighborhood->generateBest(mActualSolution, mGraph, mCriterion, mTabuList);
            bestNeighborCost = mGraph.getSolutionCost(bestNeighbor);
        }
        if (bestNeighborCost < veryBestCost)
        {
            veryBest = bestNeighbor;
            veryBestCost = bestNeighborCost;
        }
        return veryBest;
    }
} // namespace Yolo
