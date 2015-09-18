#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "pathfinder.hpp"

std::vector<sf::RectangleShape> createCells(std::vector<std::vector<int>> grid, int cellSize)
{
    int gridSize = cellSize;
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

std::vector<std::vector<int>> getBaseGrid(int grid_max_x, int grid_max_y)
{
    std::vector<std::vector<int>> grid;
    for(int i = 0; i < grid_max_y; i++)
        grid.push_back(std::vector<int>(grid_max_x));
    return grid;
}

int main()
{
    int cell_size = 24;
    int grid_max_x = 800 / cell_size;
    int grid_max_y = 600 / cell_size;
    bool moved = true;
    bool cut_corners = true;
    bool recalc = true;

    sf::Clock clock;

    sf::Vector2i start(1, grid_max_y / 2);
    sf::Vector2i goal(grid_max_x, grid_max_y / 2);

    std::vector<sf::Vector2i> path;
    std::vector<sf::Vector2i> considered;
    std::vector<sf::Vector2i> walls;

    sf::RenderWindow window(sf::VideoMode(grid_max_x * cell_size, grid_max_y * cell_size), "GameWindow");

    while (window.isOpen())
    {
        sf::Event event;
        std::vector<std::vector<int>> grid = getBaseGrid(grid_max_x, grid_max_y);

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

                if(event.key.code == sf::Keyboard::Escape)
                    window.close();
                if(event.key.code == sf::Keyboard::C)
                    walls.clear();
                if(event.key.code == sf::Keyboard::Z)
                    cut_corners = !cut_corners;
                if(event.key.code == sf::Keyboard::G)
                    recalc = !recalc;

                moved = true;
            }
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            int x = (int)sf::Mouse::getPosition(window).x / cell_size + 1;
            int y = (int)sf::Mouse::getPosition(window).y / cell_size + 1;

            if(x < 1 || y < 1 || x > grid_max_x || y > grid_max_y || grid[y-1][x-1] == 1)
                continue;

            walls.push_back(sf::Vector2i(x,y));
            moved = true;
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            int x = (int)sf::Mouse::getPosition(window).x / cell_size + 1;
            int y = (int)sf::Mouse::getPosition(window).y / cell_size + 1;

            if(x < 1 || y < 1 || x > grid_max_x || y > grid_max_y || grid[y-1][x-1] == 1)
                continue;
            for(std::size_t i = 0; i < walls.size(); i++ )
            {
                sf::Vector2i wall = walls[i];
                if(wall.x == x && wall.y == y)
                    walls.erase(walls.begin() + i);
            }
            moved = true;
        }

        if (start.x < 1)  { start.x = 1; }
        if (start.x > grid_max_x) { start.x = grid_max_x;}
        if (start.y < 1)  { start.y = 1; }
        if (start.y > grid_max_x) { start.y = grid_max_x;}

        if (goal.x < 1)  { goal.x = 1; }
        if (goal.x > grid_max_x) { goal.x = grid_max_x;}
        if (goal.y < 1)  { goal.y = 1; }
        if (goal.y > grid_max_x) { goal.y = grid_max_x;}

        grid[start.y-1][start.x-1] = 2;

        for(sf::Vector2i& wall : walls)
            grid[wall.y-1][wall.x-1] = 1;

        if(recalc)
        {
            PathFinder pathfinder(grid, grid_max_x, grid_max_y);
            pathfinder.setStart(start.x, start.y);
            pathfinder.setGoal(goal.x, goal.y);
            pathfinder.skipCorners = cut_corners;
            path = pathfinder.find();
            considered = pathfinder.consideredList;
            moved = false;

            for(sf::Vector2i point : considered)
                grid[point.y-1][point.x-1] = 5;

            for(sf::Vector2i point : path)
                grid[point.y-1][point.x-1] = 4;
        }


        grid[goal.y-1][goal.x-1] = 3;

        std::vector<sf::RectangleShape> cells = createCells(grid, cell_size);

        window.clear();

        for(auto& rect : cells)
            window.draw(rect);

        window.display();
        std::cout << 1 / clock.restart().asSeconds() << std::endl;
    }

    return 0;
}
