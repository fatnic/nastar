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
//        std::sort(openList.begin(), openList.end(), sortNodes);
        openList.sort(sortNodes);
        current = openList.front();
        openList.erase(openList.begin());
        closedList.push_back(current);

        if(current.parent != nullptr)
        {
            std::cout << "I'm am currently (" << current.cell.x << ":" << current.cell.y <<") and my parent is (";
            std::cout << current.parent->cell.x << ":" << current.parent->cell.y << ")\n";
        }

        if(current.equals(goal))
        {
            std::cout << "found\n";
            while(current.parent != nullptr)
            {
                finalPath.push_back(current.cell);
                current = *current.parent;
            }
            return finalPath;
        }

        for (int i = 0; i < 9; i++)
        {
            if (i == 4)
                continue;

            int x = current.cell.x;
            int y = current.cell.y;

            int xi = (i % 3) - 1;
            int yi = (i / 3) - 1;

            sf::Vector2i cCell(x + xi, y + yi);

            if (blocked(cCell))
                continue;

            float G = distance(start, cCell);
            float H = distance(cCell, goal);

            if(vectorInList(closedList, cCell))
            {
                continue;
            }

            if(!vectorInList(openList, cCell)  && G >= current.G)
            {
                PathNode newNode(cCell, &current, G, H);
                openList.push_back(newNode);
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

bool PathFinder::blocked(sf::Vector2i cell)
{
    int cellValue = grid[cell.y-1][cell.x-1];
    if (cellValue == 1)
    {
        return true;
    }

    if (cell.x < 1 || cell.x > xSize || cell.y < 1 || cell.y > ySize)
    {
        return true;
    }

    return false;
}

bool PathFinder::vectorInList(std::list<PathNode> list, sf::Vector2i node)
{
    for (PathNode& cell : list)
    {
        if (cell.equals(node))
        {
            return true;
        }
    }
    return false;
}
