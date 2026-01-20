#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    std::cout << "Starting SFML test\n";

    sf::RenderWindow window(sf::VideoMode(400, 400), "Test Window");
    std::cout << "Window created\n";

    sf::CircleShape shape(50);
    shape.setFillColor(sf::Color::Red);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        window.draw(shape);
        window.display();
    }

    return 0;
}
