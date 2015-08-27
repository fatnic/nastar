#include "pathfinder.hpp"
#include <algorithm>

PathFinder::PathFinder(std::vector<std::vector<int> > grid, int x, int y)
    : xSize(x)
    , ySize(y)
{

}

void PathFinder::setStart(int x, int y)
{
    start = sf::Vector2i(x, y);
}

void PathFinder::setGoal(int x, int y)
{
    goal = sf::Vector2i(x ,y);
}

std::vector<sf::Vector2i> PathFinder::find()
{
    current = new PathNode(start, nullptr, 0, distanceToGoal(start));
    openList.push_back(current);

    // Sort openList;

    while(!openList.empty())
    {
        current = openList[0];

//        if(current.cell.equal(goal)) { return; }

    }

    return finalPath;
}

float PathFinder::distanceToGoal(sf::Vector2i cell)
{
    float dX = cell.x - goal.x;
    float dY = cell.y - goal.y;
    return std::sqrt(dX * dX + dY * dY);
}

bool PathFinder::sortNodes(PathNode n0, PathNode n1)
{
    return (n0.F < n1.F);
}

