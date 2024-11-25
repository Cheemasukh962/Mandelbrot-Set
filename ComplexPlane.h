#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H

#include <SFML/Graphics.hpp>
#include <sstream>

// Global constants
const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0f;
const float BASE_HEIGHT = 4.0f;
const float BASE_ZOOM = 0.5f;

// Enum class for state
enum class State { CALCULATING, DISPLAYING };

class ComplexPlane : public sf::Drawable {
private:
    unsigned int m_pixelWidth;
    unsigned int m_pixelHeight;
    float m_aspectRatio;

    sf::Vector2f m_planeCenter;
    sf::Vector2f m_planeSize;
    int m_zoomCount;

    sf::VertexArray m_vArray;
    sf::Vector2f m_mouseLocation;
    State m_State;

    // Helper functions
    sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel) const;
    size_t countIterations(sf::Vector2f coord) const;
    void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const;

    // Required for sf::Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    // Constructor
    ComplexPlane(unsigned int pixelWidth, unsigned int pixelHeight);

    // Public member functions
    void updateRender();
    void zoomIn();
    void zoomOut();
    void setCenter(sf::Vector2i mousePixel);
    void setMouseLocation(sf::Vector2i mousePixel);
    void loadText(sf::Text& text) const;
};

#endif
