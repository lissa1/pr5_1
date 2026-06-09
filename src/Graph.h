#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class Graph {
private:
    // Window and view parameters
    sf::RenderWindow window;
    float offsetX, offsetY;  // Смещение графика
    float scale;             // Масштаб (пиксели на единицу)
    float unitSize;          // Размер единичного отрезка в пикселях
    
    // Constants
    const int WINDOW_WIDTH = 1200;
    const int WINDOW_HEIGHT = 800;
    const float MIN_SCALE = 0.5f;
    const float MAX_SCALE = 50.0f;
    const float SCALE_STEP = 1.1f;
    const float MOVE_STEP = 20.0f;
    
    // Colors
    const sf::Color COLOR_BACKGROUND = sf::Color::White;
    const sf::Color COLOR_AXIS = sf::Color::Black;
    const sf::Color COLOR_GRID = sf::Color(200, 200, 200);
    const sf::Color COLOR_GRAPH = sf::Color::Blue;
    const sf::Color COLOR_ASYMPTOTE = sf::Color::Red;
    
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
    void drawGraph();
    void drawAsymptotes();
    void drawFunctionLabel();
    
    // Преобразования координат
    sf::Vector2f worldToScreen(float x, float y) const;
    sf::Vector2f screenToWorld(float screenX, float screenY) const;
    
    // Проверка видимости точки
    bool isPointVisible(float x, float y) const;
    
    // Проверка наличия разрыва функции
    bool hasDiscontinuity(float x1, float x2) const;
};
