#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <SFML/Graphics.hpp>

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

    sf::Vector2i start(2,2);
    sf::Vector2i goal(18,18);

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
                std::cout << start.x << ":" << start.y << std::endl;
            }
        }

        if (start.x < 0)  { start.x = 0; }
        if (start.x > 19) { start.x = 19;}
        if (start.y < 0)  { start.y = 0; }
        if (start.y > 19) { start.y = 19;}

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            float x = sf::Mouse::getPosition(window).x;
            float y = sf::Mouse::getPosition(window).y;

            goal.x = (int)x / 32;
            goal.y = (int)y / 32;
        }

        std::vector<std::vector<int>> grid = createGrid(baseGrid);
        grid[start.y][start.x] = 2;
        grid[goal.y][goal.x] = 3;
        std::vector<sf::RectangleShape> cells = createCells(grid);

        window.clear();

        for(auto& rect : cells)
            window.draw(rect);

        window.display();
    }

    return 0;
}
