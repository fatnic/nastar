#ifndef PATHNODE
#define PATHNODE

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

#endif // PATHNODE

