#include "pathfinder.hpp"
#include <algorithm>
#include <iostream>
#include <stdlib.h>

PathFinder::PathFinder(std::vector<std::vector<int>> grid, int grid_size_x, int grid_size_y)
    : grid(grid)
    , xSize(grid_size_x)
    , ySize(grid_size_y)
    , finalPath()
    , skipCorners(true)
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
    PathNode *current = new PathNode(start, nullptr, 0.f, distance(start, goal));
    openList.push_back(current);

    while(!openList.empty())
    {
        std::sort(openList.begin(), openList.end(), sortNodes);
        current = openList[0];

        if(current->equals(goal))
        {

            while(current->parent != nullptr)
            {
                finalPath.push_back(current->cell);
                current = current->parent;
            }

            clearLists();
            std::reverse(finalPath.begin(), finalPath.end());
            return finalPath;
        }

        openList.erase(openList.begin());
        closedList.push_back(current);

        int x = current->cell.x;
        int y = current->cell.y;

        calcNeighbours(x, y);

        if(skipCorners) removeCorners();

        for(int i=0; i < 9; i++)
        {
            if(neighbours[i] == false) continue;

            int xi = (i % 3) - 1;
            int yi = (i / 3) - 1;

            sf::Vector2i neighbour(x + xi, y + yi);

            float G = distance(current->cell, neighbour);
            float H = manhattenDistance(neighbour, goal);

            if(vectorInList(closedList, neighbour) && G >= current->G) continue;

            if(!vectorInList(openList, neighbour))
            {
                consideredList.push_back(neighbour);
                PathNode *newNode = new PathNode(neighbour, current, G, H);
                openList.push_back(newNode);
            }
            else
            {
                for(PathNode *node : openList)
                {
                    if(node->equals(neighbour) && G + H < node->F)
                    {
                        node->G = G;
                        node->H = H;
                        node->parent = current;
                    }
                }
            }
        }

    }

    clearLists();
    return finalPath;
}

float PathFinder::distance(sf::Vector2i cell0, sf::Vector2i cell1)
{
    float dX = cell0.x - cell1.x;
    float dY = cell0.y - cell1.y;
    return std::sqrt(dX * dX + dY * dY);
}

float PathFinder::manhattenDistance(sf::Vector2i cell0, sf::Vector2i cell1)
{
    float x = (float)(std::fabs((float)(cell0.x - cell1.x)));
    float y = (float)(std::fabs((float)(cell0.y - cell1.y)));
    return x + y;
}

bool PathFinder::sortNodes(PathNode* n0, PathNode* n1)
{
    return (n0->F < n1->F);
}

bool PathFinder::blocked(sf::Vector2i cell)
{
    if (cell.x < 1 || cell.x > xSize || cell.y < 1 || cell.y > ySize)
        return true;

    int cellValue = grid[cell.y-1][cell.x-1];
    if (cellValue == 1)
        return true;

    return false;
}

bool PathFinder::vectorInList(std::vector<PathNode*> list, sf::Vector2i cell)
{
    for (int i = 0; i < list.size(); i++)
    {
        PathNode node = *list[i];
        if(node.cell.x == cell.x && node.cell.y == cell.y)
            return true;
    }
    return false;
}

void PathFinder::clearLists()
{
    for(auto& node : openList)
        delete node;

    for(auto& node : closedList)
        delete node;

    openList.clear();
    closedList.clear();
}

void PathFinder::calcNeighbours(int x, int y)
{
    for(int i = 0; i < 9; i++)
        neighbours[i] = true;

    neighbours[4] = false;

    for(int i = 0; i < 9; i++)
    {
        int xi = (i % 3) - 1;
        int yi = (i / 3) - 1;

        sf::Vector2i neighbour(x + xi, y + yi);

        if(blocked(neighbour)) neighbours[i] = false;
    }
}

void PathFinder::removeCorners()
{
    if(neighbours[1] == false)
    {
        neighbours[0] = false;
        neighbours[2] = false;
    }

    if(neighbours[3] == false)
    {
        neighbours[0] = false;
        neighbours[6] = false;
    }

    if(neighbours[5] == false)
    {
        neighbours[2] = false;
        neighbours[8] = false;
    }

    if(neighbours[7] == false)
    {
        neighbours[6] = false;
        neighbours[8] = false;
    }
}
