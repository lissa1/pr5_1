#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <string>

class Graph {
private:
    // Window and view parameters
    sf::RenderWindow window;
    float offsetX, offsetY;  // Смещение графика
    float scale;             // Масштаб (пиксели на единицу)
    float unitSize;          // Размер единичного отрезка в пикселях
    
    // Constants
    static constexpr unsigned int WINDOW_WIDTH = 1400;
    static constexpr unsigned int WINDOW_HEIGHT = 900;
    static constexpr float MIN_SCALE = 0.5f;
    static constexpr float MAX_SCALE = 100.0f;
    static constexpr float SCALE_STEP = 1.15f;
    static constexpr float MOVE_STEP = 15.0f;
    
    // Colors
    static constexpr sf::Color COLOR_BACKGROUND = sf::Color::White;
    static constexpr sf::Color COLOR_AXIS = sf::Color::Black;
    static constexpr sf::Color COLOR_GRID = sf::Color(220, 220, 220);
    static constexpr sf::Color COLOR_MAJOR_GRID = sf::Color(200, 200, 200);
    static constexpr sf::Color COLOR_GRAPH = sf::Color(0, 0, 255);
    static constexpr sf::Color COLOR_ASYMPTOTE = sf::Color(255, 0, 0);
    static constexpr sf::Color COLOR_TEXT = sf::Color::Black;
    
    // Font for labels (we'll use a simple approach without font file)
    sf::Font* font;
    
public:
    Graph();
    ~Graph();
    
    void run();
    void handleInput();
    void update();
    void render();
    
private:
    // Функция y = (x^2 - 3) / ((x - 1) * (5 - x))
    float evaluateFunction(float x) const;
    
    // Вспомогательные функции
    void drawCoordinateSystem();
    void drawGrid();
    void drawAxes();
    void drawAxisLabels();
    void drawAxisTicksAndLabels();
    void drawGraph();
    void drawAsymptotes();
    void drawFunctionLabel();
    void drawScaleInfo();
    
    // Преобразования координат
    sf::Vector2f worldToScreen(float x, float y) const;
    sf::Vector2f screenToWorld(float screenX, float screenY) const;
    
    // Проверка видимости точки
    bool isPointVisible(float x, float y) const;
    
    // Проверка наличия разрыва функции
    bool hasDiscontinuity(float x1, float x2) const;
    
    // Рисование текста (без использования шрифтов)
    void drawSimpleText(const std::string& text, float x, float y, float size = 12.0f);
    void drawTickLabel(float value, float x, float y, bool isVertical = false);
};
