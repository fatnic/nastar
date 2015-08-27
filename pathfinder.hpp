#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <vector>
#include <math.h>

#include <SFML/Graphics.hpp>

struct PathNode
{
    PathNode(){ parent = nullptr; }
    ~PathNode() {}
    sf::Vector2i cell;
    float G = 0;
    float H = 0;
    float F = 0;
    PathNode* parent;
    PathNode(sf::Vector2i cell, PathNode* parent, float Gcost, float Hcost)
        : cell(cell)
        , parent(parent)
        , G(Gcost)
        , H(Hcost)
        , F(Gcost + Hcost) {}
    bool equal(sf::Vector2i a)
    {
        if (a.x == cell.x && a.y == cell.y)
            return true;
        return false;
    }
};

class PathFinder
{
public:
    PathFinder(std::vector<std::vector<int>> grid, int x, int y);
    void setStart(int x, int y);
    void setGoal(int x, int y);
    std::vector<sf::Vector2i> find();
private:
    float distance(sf::Vector2i cell0, sf::Vector2i cell1);
    static bool sortNodes(PathNode n0, PathNode n1);
    bool blocked(int tile);
    bool vectorInList(std::vector<PathNode> list, sf::Vector2i node);
    std::vector<std::vector<int>> grid;
    int xSize;
    int ySize;
    sf::Vector2i start;
    sf::Vector2i goal;
    PathNode current;
    std::vector<PathNode> openList;
    std::vector<PathNode> closedList;
    std::vector<sf::Vector2i> finalPath;
};

#endif // PATHFINDER_HPP
