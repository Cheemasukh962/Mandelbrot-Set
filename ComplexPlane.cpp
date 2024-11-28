#include "ComplexPlane.h"

// Constructor
ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_pixelSize(pixelWidth, pixelHeight),
    m_zoomCount(0),
    m_state(State::CALCULATING),
    m_vArray(Points, pixelWidth* pixelHeight) {
    m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;
    m_planeCenter = { 0.0f, 0.0f };
    m_planeSize = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
}

// Map pixel coordinates to complex plane coordinates
Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) const {
    float real = ((mousePixel.x - 0.0f) / m_pixelSize.x) * m_planeSize.x +
        (m_planeCenter.x - m_planeSize.x / 2.0f);
    float imag = ((mousePixel.y - static_cast<float>(m_pixelSize.y)) / -m_pixelSize.y) * m_planeSize.y +
        (m_planeCenter.y - m_planeSize.y / 2.0f);
    return { real, imag };
}

// Count Mandelbrot iterations
size_t ComplexPlane::countIterations(Vector2f coord) const {
    std::complex<float> z(0.0f, 0.0f);
    std::complex<float> c(coord.x, coord.y);
    size_t count = 0;

    while (std::abs(z) <= 2.0f && count < MAX_ITER) {
        z = z * z + c;
        count++;
    }
    return count;
}

// Convert iterations to RGB values
void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) const {
    if (count == MAX_ITER) {
        r = g = b = 0;  // Black for points inside the Mandelbrot set
    }
    else if (count > MAX_ITER / 2) {
        r = 234; g = 50; b = 60;  // Red for high iteration counts
    }
    else if (count > MAX_ITER / 4) {
        r = 12; g = 2; b = 147;  // Purple for medium iteration counts
    }
    else {
        r = 230; g = 230; b = 235;  // White for low iteration counts
    }
}

// Render the Mandelbrot set
void ComplexPlane::updateRender() {
    if (m_state == State::CALCULATING) {
        unsigned int conThreads = thread::hardware_concurrency();
        //std::cout << "# concurrent threads supported: " << conThreads << endl; // usually 3 on Ubuntu VM

        int threadCount = conThreads;
        // v this is lambda v
        auto calcRow = [this](int row)
        {
            for (int j = 0; j < m_pixelSize.x; ++j) {
                size_t index = j + row * m_pixelSize.x;
                m_vArray[index].position = Vector2f(static_cast<float>(j), static_cast<float>(row));

                Vector2f coord = mapPixelToCoords({ j, row });
                size_t iterations = countIterations(coord);

                Uint8 r, g, b;
                iterationsToRGB(iterations, r, g, b);
                m_vArray[index].color = Color(r, g, b);
            }
        };

        vector<thread> threads;
        for (int i = 0; i < m_pixelSize.y; i++)
        {
            if (threads.size() < threadCount)
            {
                threads.push_back(thread(calcRow, i));
            }
            else
            {
                threads.front().join();
                threads.erase(threads.begin());
                threads.push_back(thread(calcRow, i));
            }
        }
        for (auto& t : threads)
        {
            t.join();
        }
        m_state = State::DISPLAYING;
    }
}

// Zoom in
void ComplexPlane::zoomIn() {
    m_zoomCount++;
    float xSize = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
    m_planeSize = { xSize, ySize };
    m_state = State::CALCULATING;
}

// Zoom out
void ComplexPlane::zoomOut() {
    m_zoomCount--;
    float xSize = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
    m_planeSize = { xSize, ySize };
    m_state = State::CALCULATING;
}

// Set the center of the complex plane
void ComplexPlane::setCenter(Vector2i mousePixel) {
    m_planeCenter = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

// Set the mouse location
void ComplexPlane::setMouseLocation(Vector2i mousePixel) {
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

// Load text information
void ComplexPlane::loadText(Text& text) const {
    std::stringstream ss;
    ss << "Mandelbrot Set\n";
    ss << "Center: (" << m_planeCenter.x << ", " << m_planeCenter.y << ")\n";
    ss << "Size: (" << m_planeSize.x << ", " << m_planeSize.y << ")\n";
    ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
    ss << "Left-click: Zoom in\n";
    ss << "Right-click: Zoom out";
    text.setString(ss.str());
}

// Draw the Mandelbrot set
void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray, states);
}
