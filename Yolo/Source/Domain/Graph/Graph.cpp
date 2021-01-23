# include "Graph.hpp"

namespace Yolo
{
    Graph::Graph(const std::vector<std::vector<Edge>>& adjacencyList)
        : mAdjacencyList(std::vector<std::vector<Edge>>(adjacencyList))
    {
        mVertexDegrees = std::vector<int>(mAdjacencyList.size());
        mMaxDegree = static_cast<int>(mAdjacencyList[0].size());
        mMinDegree = static_cast<int>(mAdjacencyList[0].size());

        mNbEdges = 0;

        for (int i = 0; i < mAdjacencyList.size(); ++i)
        {
            mVertexDegrees[i] = static_cast<int>(mAdjacencyList[i].size());
            if (mVertexDegrees[i] > mMaxDegree)
                mMaxDegree = mVertexDegrees[i];
            if (mVertexDegrees[i] < mMinDegree)
                mMinDegree = mVertexDegrees[i];

            mNbEdges += static_cast<int>(mAdjacencyList[i].size());
        }

        mNbEdges /= 2; /* Undirected graph */
    }

    CheckerOutput Graph::checkSolution(const Solution& solution, bool &criterion(std::vector<int>)) const
    {
        return {getSolutionCost(solution), isValid(solution, criterion)};
    }
    // go through every arc twice.
    double Graph::getSolutionCost(const Solution& solution) const
    {
        double cost = 0;
        for (int i = 0; i < mAdjacencyList.size(); ++i)
        {
            for (int j = 0; j < mAdjacencyList[i].size(); ++j)
            {
                if(solution.getVertexClass(mAdjacencyList[i][j].getSource()) != solution.getVertexClass(mAdjacencyList[i][j].getDestination())){
                    cost += mAdjacencyList[i][j].getWeight();
                }
            }
            
        }
    return cost/2;
    }

    bool Graph::isValid(const Solution& solution,bool &criterion(std::vector<int>)) const{
        std::vector<int> nbElementPerClasses = std::vector<int>(solution.getNbClasses(), 0);
        for (int i = 0; i < mAdjacencyList.size(); ++i)
        {
            nbElementPerClasses[solution.getVertexClass(i)] ++;
        }
        return criterion(nbElementPerClasses);
        
    }
}