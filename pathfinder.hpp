#ifndef PATHFINDER_HPP
#define PATHFINDER_HPP

#include <vector>
#include <math.h>
#include <string>
#include <iostream>
#include <list>

#include <SFML/Graphics.hpp>
#include "pathnode.hpp"

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
    bool blocked(sf::Vector2i cell);
    bool vectorInList(std::list<PathNode> list, sf::Vector2i node);
    std::vector<std::vector<int>> grid;
    int xSize;
    int ySize;
    sf::RenderWindow* window;
    sf::Vector2i start;
    sf::Vector2i goal;
    PathNode current;
    std::list<PathNode*> openList;
    std::list<PathNode*> closedList;
    std::vector<sf::Vector2i> finalPath;
};

#endif // PATHFINDER_HPP
