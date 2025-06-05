// Compilation (Linux): g++ main.cpp -o snake -lsfml-graphics -lsfml-window -lsfml-system

#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>

const int width = 800;
const int height = 600;
const int size = 20;
const int cols = width / size;
const int rows = height / size;

sf::Vector2i direction(1, 0); // Start moving right

class Snake {
public:
    std::vector<sf::Vector2i> body;
    Snake() {
        body.push_back(sf::Vector2i(cols / 2, rows / 2));
    }

    void move() {
        for (int i = body.size() - 1; i > 0; --i)
            body[i] = body[i - 1];
        body[0] += direction;
    }

    void grow() {
        body.push_back(body.back());
    }

    bool checkCollision() {
        for (int i = 1; i < body.size(); ++i)
            if (body[0] == body[i]) return true;

        return body[0].x < 0 || body[0].y < 0 || body[0].x >= cols || body[0].y >= rows;
    }
};

sf::Vector2i generateFood() {
    return sf::Vector2i(rand() % cols, rand() % rows);
}

int main() {
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(width, height), "Snake Game with SFML");
    window.setFramerateLimit(10);

    Snake snake;
    sf::Vector2i food = generateFood();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up && direction.y != 1)
                    direction = sf::Vector2i(0, -1);
                else if (event.key.code == sf::Keyboard::Down && direction.y != -1)
                    direction = sf::Vector2i(0, 1);
                else if (event.key.code == sf::Keyboard::Left && direction.x != 1)
                    direction = sf::Vector2i(-1, 0);
                else if (event.key.code == sf::Keyboard::Right && direction.x != -1)
                    direction = sf::Vector2i(1, 0);
            }
        }

        snake.move();

        if (snake.body[0] == food) {
            snake.grow();
            food = generateFood();
        }

        if (snake.checkCollision()) {
            window.close();
        }

        window.clear();

        // Draw snake
        sf::RectangleShape rect(sf::Vector2f(size - 2, size - 2));
        rect.setFillColor(sf::Color::Green);
        for (auto& segment : snake.body) {
            rect.setPosition(segment.x * size, segment.y * size);
            window.draw(rect);
        }

        // Draw food
        rect.setFillColor(sf::Color::Red);
        rect.setPosition(food.x * size, food.y * size);
        window.draw(rect);

        window.display();
    }

    return 0;
}
