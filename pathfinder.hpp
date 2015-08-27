#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <vector>
#include <math.h>

#include <SFML/Graphics.hpp>

struct PathNode
{
public:
    sf::Vector2i cell;
    PathNode parent;
    float G = 0;
    float H = 0;
    float F = 0;
    PathNode(sf::Vector2i cell, PathNode parent, float Gcost, float Hcost)
        : cell(cell)
        , parent(parent)
        , G(Gcost)
        , H(Hcost)
        , F(Gcost + Hcost)
    {}
};

class PathFinder
{
public:
    PathFinder(std::vector<std::vector<int>> grid, int x, int y);
    void setStart(int x, int y);
    void setGoal(int x, int y);
    std::vector<sf::Vector2i> find();
    float distanceToGoal(sf::Vector2i cell);
    bool sortNodes(PathNode n0, PathNode n1);
    int xSize;
    int ySize;
    sf::Vector2i start;
    sf::Vector2i goal;
    PathNode current;

    std::vector<PathNode> openList;
    std::vector<PathNode> closedList;
    std::vector<PathNode> finalPath;
};

#endif // PATHFINDER_HPP
