#include "pathfinder.hpp"
#include <algorithm>
#include <iostream>
#include <stdlib.h>

PathFinder::PathFinder(std::vector<std::vector<int> > grid, int x, int y)
    : grid(grid)
    , xSize(x)
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
    PathNode current(start, nullptr, 0.f, distance(start, goal));
    openList.push_back(current);

    while(!openList.empty())
    {
        std::sort(openList.begin(), openList.end(), sortNodes);
        current = openList[0];
        openList.erase(openList.begin());
        closedList.push_back(current);

        if(current.equal(goal))
        {
            std::cout << "found goal" << std::endl;
            while(current.parent != nullptr)
            {
                // parent == current so always adds last node. fix this!!!
                finalPath.push_back(current.cell);
                current = *current.parent;
            }
            openList.clear();
            closedList.clear();

            return finalPath;
        }

        for (int i = 0; i < 9; i++)
        {
            if (i == 4) continue;

            int x = current.cell.x;
            int y = current.cell.y;
            int xi = (i % 3) - 1;
            int yi = (i / 3) - 1;

            if (x+xi < 0 || x+xi > xSize) continue;
            if (y+yi < 0 || y+yi > ySize) continue;

            int at = grid[y + yi][x + xi];

            if (blocked(at)) continue;

            sf::Vector2i a(x + xi, y + yi);
            float G = current.G + distance(current.cell, a);
            float H = distance(a, goal);

            if(vectorInList(closedList, a)) continue;
            if(!vectorInList(openList, a) || G < current.G)
            {
                PathNode node(a, &current, G, H);
                openList.push_back(node);
            }
        }
    }
    closedList.clear();
    return finalPath;
}

float PathFinder::distance(sf::Vector2i cell0, sf::Vector2i cell1)
{
    float dX = cell0.x - cell1.x;
    float dY = cell0.y - cell1.y;
    return std::sqrt(dX * dX + dY * dY);
}

bool PathFinder::sortNodes(PathNode n0, PathNode n1)
{
    return (n0.F < n1.F);
}

bool PathFinder::blocked(int tile)
{
    if (tile == 1)
        return true;
    return false;
}

bool PathFinder::vectorInList(std::vector<PathNode> list, sf::Vector2i node)
{
    for (PathNode& cell : list)
        if (cell.equal(node)) return true;

    return false;
}
