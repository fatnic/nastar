#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <vector>
#include <math.h>
#include <string>
#include <iostream>
#include <list>

#include <SFML/Graphics.hpp>

struct PathNode
{
    PathNode(){}
    PathNode(sf::Vector2i cell, PathNode* parent, float Gcost, float Hcost)
        : cell(cell)
        , parent(parent)
        , G(Gcost)
        , H(Hcost)
        , F(Gcost + Hcost) {}
    sf::Vector2i cell;
    float G = 0;
    float H = 0;
    float F = 0;
    PathNode* parent;
    bool equals(sf::Vector2i a)
    {
        if (a.x == cell.x && a.y == cell.y)
            return true;
        return false;
    }
};

class PathFinder
{
public:
    std::vector<sf::Vector2i> consideredList;
    bool skipCorners;
private:
    int xSize;
    int ySize;
    sf::Vector2i start;
    sf::Vector2i goal;
    PathNode current;
    std::vector<PathNode*> openList;
    std::vector<PathNode*> closedList;
    std::vector<sf::Vector2i> finalPath;
    bool neighbours[9];
public:
    PathFinder(std::vector<std::vector<int>> grid, int x, int y);
    void setStart(int x, int y);
    void setGoal(int x, int y);
    std::vector<sf::Vector2i> find();
private:
    float distance(sf::Vector2i cell0, sf::Vector2i cell1);
    float manhattenDistance(sf::Vector2i cell0, sf::Vector2i cell1);
    static bool sortNodes(PathNode *n0, PathNode *n1);
    bool blocked(sf::Vector2i cell);
    bool vectorInList(std::vector<PathNode *> list, sf::Vector2i cell);
    std::vector<std::vector<int>> grid;
    void clearLists();
    void calcNeighbours(int x, int y);
    void removeCorners();
};

#endif // PATHFINDER_HPP
