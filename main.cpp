#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>
#include "pathfinder.hpp"

std::vector<std::string> getBaseGrid(std::string filename)
{
    std::vector<std::string> basegrid;
    std::ifstream infile(filename);
    std::string line;
    while(std::getline(infile, line))
        basegrid.push_back(line);
    return basegrid;
}

std::vector<std::vector<int>> createGrid(std::vector<std::string> baseGrid)
{
    std::vector<std::vector<int>> grid;
    std::vector<int> row;

    for(std::string line : baseGrid)
    {
        for(char& c: line)
        {
            int ic = c - '0';
            row.push_back(ic);
        }
        grid.push_back(row);
        row.clear();
    }
    return grid;
}

std::vector<sf::RectangleShape> createCells(std::vector<std::vector<int>> grid)
{
    int gridSize = 32;
    int y = 0;

    std::vector<sf::RectangleShape> cells;
    for(std::vector<int> row : grid)
    {
        for(unsigned int i = 0; i < row.size(); i++ )
        {
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(gridSize, gridSize));
            rect.setPosition(sf::Vector2f(gridSize * i, gridSize * y));
            rect.setOutlineColor(sf::Color(64,64,64));
            rect.setOutlineThickness(1.f);
            switch(row[i])
            {
            case 1:
                rect.setFillColor(sf::Color::Blue);
                break;
            case 2:
                rect.setFillColor(sf::Color::Green);
                break;
            case 3:
                rect.setFillColor(sf::Color::Magenta);
                break;
            case 4:
                rect.setFillColor(sf::Color::Red);
                break;
            case 5:
                rect.setFillColor(sf::Color::White);
                break;
            default:
                rect.setFillColor(sf::Color::Black);
                break;
            }
            cells.push_back(rect);
        }
        y++;
    }
    return cells;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 640), "GameWindow");

    std::vector<std::string> baseGrid = getBaseGrid("assets/grid.txt");

    sf::Vector2i start(10,18);
    sf::Vector2i goal(16,7);

    bool moved = true;
    std::vector<sf::Vector2i> path;
    std::vector<sf::Vector2i> considered;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::W)
                    start.y -= 1;
                if(event.key.code == sf::Keyboard::S)
                    start.y += 1;
                if(event.key.code == sf::Keyboard::A)
                    start.x -= 1;
                if(event.key.code == sf::Keyboard::D)
                    start.x += 1;

                if(event.key.code == sf::Keyboard::Up)
                    goal.y -= 1;
                if(event.key.code == sf::Keyboard::Down)
                    goal.y += 1;
                if(event.key.code == sf::Keyboard::Left)
                    goal.x -= 1;
                if(event.key.code == sf::Keyboard::Right)
                    goal.x += 1;

                moved = true;
            }
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            start.x = (int)sf::Mouse::getPosition(window).x/32+1;
            start.y = (int)sf::Mouse::getPosition(window).y/32+1;
            moved = true;
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            goal.x = (int)sf::Mouse::getPosition(window).x/32+1;
            goal.y = (int)sf::Mouse::getPosition(window).y/32+1;
            moved = true;
        }

        if (start.x < 1)  { start.x = 1; }
        if (start.x > 20) { start.x = 20;}
        if (start.y < 1)  { start.y = 1; }
        if (start.y > 20) { start.y = 20;}

        if (goal.x < 1)  { goal.x = 1; }
        if (goal.x > 20) { goal.x = 20;}
        if (goal.y < 1)  { goal.y = 1; }
        if (goal.y > 20) { goal.y = 20;}

        std::vector<std::vector<int>> grid = createGrid(baseGrid);
        grid[start.y-1][start.x-1] = 2;

        if(moved)
        {
            PathFinder pathfinder(grid, 20, 20);
            pathfinder.setStart(start.x, start.y);
            pathfinder.setGoal(goal.x, goal.y);
            path = pathfinder.find();
            considered = pathfinder.consideredList;
            moved = false;
        }

        for(sf::Vector2i point : considered)
            grid[point.y-1][point.x-1] = 5;

        for(sf::Vector2i point : path)
            grid[point.y-1][point.x-1] = 4;

        grid[goal.y-1][goal.x-1] = 3;

        std::vector<sf::RectangleShape> cells = createCells(grid);

        window.clear();

        for(auto& rect : cells)
            window.draw(rect);

        window.display();
    }

    return 0;
}
