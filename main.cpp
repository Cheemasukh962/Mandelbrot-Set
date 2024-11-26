#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"
#include <stdexcept>

int main() {
    // Get desktop resolution
    unsigned int screenWidth = sf::VideoMode::getDesktopMode().width / 2;
    unsigned int screenHeight = sf::VideoMode::getDesktopMode().height / 2;

    // Create window
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Mandelbrot Set Viewer");

    // Create ComplexPlane
    ComplexPlane complexPlane(screenWidth, screenHeight);

    // Load font and create text
    sf::Font font;
    if (!font.loadFromFile("Arial.ttf")) {
        throw std::runtime_error("Could not load font!");
    }
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::White);

    // Main loop
    while (window.isOpen()) {
        // Handle input
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
                    complexPlane.zoomIn();
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    complexPlane.setCenter({ event.mouseButton.x, event.mouseButton.y });
                    complexPlane.zoomOut();
                }
            }
            if (event.type == sf::Event::MouseMoved) {
                complexPlane.setMouseLocation({ event.mouseMove.x, event.mouseMove.y });
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        // Update scene
        complexPlane.updateRender();
        complexPlane.loadText(text);

        // Draw scene
        window.clear();
        window.draw(complexPlane);
        window.draw(text);
        window.display();
    }

    return 0;
}
