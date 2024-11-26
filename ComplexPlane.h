#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H

#include <SFML/Graphics.hpp>
#include <complex>
#include <sstream>

using namespace sf;
using namespace std;

// Global constants
const unsigned int MAX_ITER = 64; // Maximum iterations for Mandelbrot calculation
const float BASE_WIDTH = 4.0f;    // Base width of the complex plane
const float BASE_HEIGHT = 4.0f;   // Base height of the complex plane
const float BASE_ZOOM = 0.5f;     // Base zoom multiplier

// Enum class for state
enum class State { CALCULATING, DISPLAYING };

class ComplexPlane : public sf::Drawable {
private:
    Vector2i m_pixelSize;          // Pixel dimensions of the screen
    float m_aspectRatio;           // Aspect ratio of the screen

    Vector2f m_planeCenter;        // Center of the complex plane
    Vector2f m_planeSize;          // Dimensions of the complex plane
    int m_zoomCount;               // Zoom level

    VertexArray m_vArray;          // Vertex array for rendering
    Vector2f m_mouseLocation;      // Current mouse position in the complex plane
    State m_state;                 // Current state (CALCULATING or DISPLAYING)

    // Helper functions
    Vector2f mapPixelToCoords(Vector2i mousePixel) const;
    size_t countIterations(Vector2f coord) const;
    void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) const;

    // Required for sf::Drawable
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    // Constructor
    ComplexPlane(int pixelWidth, int pixelHeight);

    // Public member functions
    void updateRender();
    void zoomIn();
    void zoomOut();
    void setCenter(Vector2i mousePixel);
    void setMouseLocation(Vector2i mousePixel);
    void loadText(Text& text) const;
};

#endif
