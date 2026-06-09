#include "Graph.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

Graph::Graph()
    : window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Function Grapher: y=(x2-3)/((x-1)(5-x))"),
      offsetX(0), offsetY(0), scale(30.0f), unitSize(30.0f) {
    window.setFramerateLimit(60);
}

Graph::~Graph() {
}

void Graph::run() {
    while (window.isOpen()) {
        handleInput();
        update();
        render();
    }
}

void Graph::handleInput() {
    while (const auto event = window.pollEvent()) {
        if (event.is<sf::Event::Closed>()) {
            window.close();
        }
        if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->code == sf::Keyboard::Key::Escape) {
                window.close();
            }
        }
    }
    
    // Масштабирование
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Equal)) {
        if (scale < MAX_SCALE) {
            scale *= SCALE_STEP;
            unitSize *= SCALE_STEP;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Dash)) {
        if (scale > MIN_SCALE) {
            scale /= SCALE_STEP;
            unitSize /= SCALE_STEP;
        }
    }
    
    // Перемещение стрелками
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        offsetX -= MOVE_STEP;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        offsetX += MOVE_STEP;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        offsetY += MOVE_STEP;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        offsetY -= MOVE_STEP;
    }
    
    // Перемещение WASD
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        offsetY += MOVE_STEP;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        offsetX -= MOVE_STEP;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        offsetY -= MOVE_STEP;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        offsetX += MOVE_STEP;
    }
}

void Graph::update() {
    // Обновления происходят в handleInput
}

void Graph::render() {
    window.clear(COLOR_BACKGROUND);
    
    drawCoordinateSystem();
    drawGraph();
    drawAsymptotes();
    drawFunctionLabel();
    
    window.display();
}

float Graph::evaluateFunction(float x) const {
    // Избегаем деления на ноль
    float denominator = (x - 1.0f) * (5.0f - x);
    if (std::abs(denominator) < 1e-6f) {
        return NAN;
    }
    
    float numerator = x * x - 3.0f;
    return numerator / denominator;
}

sf::Vector2f Graph::worldToScreen(float x, float y) const {
    float screenX = static_cast<float>(WINDOW_WIDTH) / 2.0f + offsetX + x * scale;
    float screenY = static_cast<float>(WINDOW_HEIGHT) / 2.0f - offsetY - y * scale;
    return sf::Vector2f(screenX, screenY);
}

sf::Vector2f Graph::screenToWorld(float screenX, float screenY) const {
    float x = (screenX - static_cast<float>(WINDOW_WIDTH) / 2.0f - offsetX) / scale;
    float y = (static_cast<float>(WINDOW_HEIGHT) / 2.0f - offsetY - screenY) / scale;
    return sf::Vector2f(x, y);
}

bool Graph::isPointVisible(float x, float y) const {
    sf::Vector2f screen = worldToScreen(x, y);
    return screen.x >= -50 && screen.x <= static_cast<float>(WINDOW_WIDTH) + 50 &&
           screen.y >= -50 && screen.y <= static_cast<float>(WINDOW_HEIGHT) + 50;
}

bool Graph::hasDiscontinuity(float x1, float x2) const {
    if ((x1 < 1 && x2 > 1) || (x1 < 5 && x2 > 5)) {
        return true;
    }
    return false;
}

void Graph::drawCoordinateSystem() {
    drawGrid();
    drawAxes();
    drawAxisLabels();
}

void Graph::drawGrid() {
    float gridStep = 1.0f;
    if (scale < 5) gridStep = 5.0f;
    if (scale < 2) gridStep = 10.0f;
    if (scale > 20) gridStep = 0.5f;
    if (scale > 40) gridStep = 0.2f;
    
    sf::Vector2f topLeft = screenToWorld(0, 0);
    sf::Vector2f bottomRight = screenToWorld(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT));
    
    // Вертикальные линии сетки
    for (float x = std::ceil(topLeft.x / gridStep) * gridStep; 
         x <= bottomRight.x; x += gridStep) {
        sf::Vector2f p1 = worldToScreen(x, topLeft.y);
        sf::Vector2f p2 = worldToScreen(x, bottomRight.y);
        
        sf::VertexBuffer vb(sf::PrimitiveType::Lines);
        std::vector<sf::Vertex> vertices = {
            sf::Vertex(p1, COLOR_GRID),
            sf::Vertex(p2, COLOR_GRID)
        };
        window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Lines);
    }
    
    // Горизонтальные линии сетки
    for (float y = std::ceil(bottomRight.y / gridStep) * gridStep;
         y <= topLeft.y; y += gridStep) {
        sf::Vector2f p1 = worldToScreen(topLeft.x, y);
        sf::Vector2f p2 = worldToScreen(bottomRight.x, y);
        
        std::vector<sf::Vertex> vertices = {
            sf::Vertex(p1, COLOR_GRID),
            sf::Vertex(p2, COLOR_GRID)
        };
        window.draw(vertices.data(), vertices.size(), sf::PrimitiveType::Lines);
    }
}

void Graph::drawAxes() {
    sf::Vector2f topLeft = screenToWorld(0, 0);
    sf::Vector2f bottomRight = screenToWorld(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT));
    
    // Ось X
    std::vector<sf::Vertex> axisX = {
        sf::Vertex(worldToScreen(topLeft.x, 0), COLOR_AXIS),
        sf::Vertex(worldToScreen(bottomRight.x, 0), COLOR_AXIS)
    };
    window.draw(axisX.data(), axisX.size(), sf::PrimitiveType::Lines);
    
    // Ось Y
    std::vector<sf::Vertex> axisY = {
        sf::Vertex(worldToScreen(0, topLeft.y), COLOR_AXIS),
        sf::Vertex(worldToScreen(0, bottomRight.y), COLOR_AXIS)
    };
    window.draw(axisY.data(), axisY.size(), sf::PrimitiveType::Lines);
    
    // Стрелки на осях
    float arrowSize = 10.0f;
    
    sf::Vector2f arrowXEnd = worldToScreen(bottomRight.x - 0.5f, 0);
    std::vector<sf::Vertex> arrowX = {
        sf::Vertex(arrowXEnd, COLOR_AXIS),
        sf::Vertex(arrowXEnd - sf::Vector2f(arrowSize, arrowSize/2), COLOR_AXIS),
        sf::Vertex(arrowXEnd, COLOR_AXIS),
        sf::Vertex(arrowXEnd - sf::Vector2f(arrowSize, -arrowSize/2), COLOR_AXIS)
    };
    window.draw(arrowX.data(), arrowX.size(), sf::PrimitiveType::Lines);
    
    sf::Vector2f arrowYEnd = worldToScreen(0, topLeft.y - 0.5f);
    std::vector<sf::Vertex> arrowY = {
        sf::Vertex(arrowYEnd, COLOR_AXIS),
        sf::Vertex(arrowYEnd + sf::Vector2f(arrowSize/2, arrowSize), COLOR_AXIS),
        sf::Vertex(arrowYEnd, COLOR_AXIS),
        sf::Vertex(arrowYEnd + sf::Vector2f(-arrowSize/2, arrowSize), COLOR_AXIS)
    };
    window.draw(arrowY.data(), arrowY.size(), sf::PrimitiveType::Lines);
    
    // Точка начала координат
    sf::CircleShape originPoint(3.0f);
    originPoint.setFillColor(COLOR_AXIS);
    sf::Vector2f origin = worldToScreen(0, 0);
    originPoint.setPosition(origin.x - 3, origin.y - 3);
    window.draw(originPoint);
}

void Graph::drawAxisLabels() {
    float labelStep = 1.0f;
    if (scale < 5) labelStep = 5.0f;
    if (scale < 2) labelStep = 10.0f;
    if (scale > 20) labelStep = 0.5f;
    
    sf::Vector2f topLeft = screenToWorld(0, 0);
    sf::Vector2f bottomRight = screenToWorld(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT));
    
    // Подписи для оси X
    for (float x = std::ceil(topLeft.x / labelStep) * labelStep;
         x <= bottomRight.x; x += labelStep) {
        if (std::abs(x) < 1e-5) continue;
        
        sf::Vector2f pos = worldToScreen(x, 0);
        
        std::vector<sf::Vertex> tick = {
            sf::Vertex(pos + sf::Vector2f(0, -5), COLOR_AXIS),
            sf::Vertex(pos + sf::Vector2f(0, 5), COLOR_AXIS)
        };
        window.draw(tick.data(), tick.size(), sf::PrimitiveType::Lines);
    }
    
    // Подписи для оси Y
    for (float y = std::ceil(bottomRight.y / labelStep) * labelStep;
         y <= topLeft.y; y += labelStep) {
        if (std::abs(y) < 1e-5) continue;
        
        sf::Vector2f pos = worldToScreen(0, y);
        
        std::vector<sf::Vertex> tick = {
            sf::Vertex(pos + sf::Vector2f(-5, 0), COLOR_AXIS),
            sf::Vertex(pos + sf::Vector2f(5, 0), COLOR_AXIS)
        };
        window.draw(tick.data(), tick.size(), sf::PrimitiveType::Lines);
    }
}

void Graph::drawGraph() {
    sf::Vector2f topLeft = screenToWorld(0, 0);
    sf::Vector2f bottomRight = screenToWorld(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT));
    
    float xStart = topLeft.x - 2.0f;
    float xEnd = bottomRight.x + 2.0f;
    float step = (xEnd - xStart) / (static_cast<float>(WINDOW_WIDTH) * 2.0f);
    
    std::vector<sf::Vertex> lineVertices;
    bool inSegment = false;
    
    for (float x = xStart; x <= xEnd; x += step) {
        // Пропускаем области разрыва вблизи x=1 и x=5
        if ((std::abs(x - 1.0f) < 0.15f) || (std::abs(x - 5.0f) < 0.15f)) {
            if (inSegment && lineVertices.size() > 1) {
                window.draw(lineVertices.data(), lineVertices.size(), sf::PrimitiveType::LineStrip);
                lineVertices.clear();
                inSegment = false;
            }
            continue;
        }
        
        float y = evaluateFunction(x);
        
        if (std::isnan(y) || std::isinf(y) || std::abs(y) > 500.0f) {
            if (inSegment && lineVertices.size() > 1) {
                window.draw(lineVertices.data(), lineVertices.size(), sf::PrimitiveType::LineStrip);
                lineVertices.clear();
                inSegment = false;
            }
            continue;
        }
        
        sf::Vector2f screenPos = worldToScreen(x, y);
        
        if (screenPos.y >= -100 && screenPos.y <= static_cast<float>(WINDOW_HEIGHT) + 100) {
            lineVertices.push_back(sf::Vertex(screenPos, COLOR_GRAPH));
            inSegment = true;
        } else {
            if (inSegment && lineVertices.size() > 1) {
                window.draw(lineVertices.data(), lineVertices.size(), sf::PrimitiveType::LineStrip);
                lineVertices.clear();
                inSegment = false;
            }
        }
    }
    
    if (inSegment && lineVertices.size() > 1) {
        window.draw(lineVertices.data(), lineVertices.size(), sf::PrimitiveType::LineStrip);
    }
}

void Graph::drawAsymptotes() {
    sf::Vector2f topLeft = screenToWorld(0, 0);
    sf::Vector2f bottomRight = screenToWorld(static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT));
    
    // Вертикальная асимптота x = 1
    sf::Vector2f p1 = worldToScreen(1.0f, topLeft.y);
    sf::Vector2f p2 = worldToScreen(1.0f, bottomRight.y);
    std::vector<sf::Vertex> asymptote1 = {
        sf::Vertex(p1, COLOR_ASYMPTOTE),
        sf::Vertex(p2, COLOR_ASYMPTOTE)
    };
    window.draw(asymptote1.data(), asymptote1.size(), sf::PrimitiveType::Lines);
    
    // Вертикальная асимптота x = 5
    p1 = worldToScreen(5.0f, topLeft.y);
    p2 = worldToScreen(5.0f, bottomRight.y);
    std::vector<sf::Vertex> asymptote2 = {
        sf::Vertex(p1, COLOR_ASYMPTOTE),
        sf::Vertex(p2, COLOR_ASYMPTOTE)
    };
    window.draw(asymptote2.data(), asymptote2.size(), sf::PrimitiveType::Lines);
}

void Graph::drawFunctionLabel() {
    sf::Vector2f pos(10, 10);
    
    sf::RectangleShape infoBox(sf::Vector2f(380, 140));
    infoBox.setPosition(pos);
    infoBox.setFillColor(sf::Color(255, 255, 255, 240));
    infoBox.setOutlineThickness(2.0f);
    infoBox.setOutlineColor(COLOR_AXIS);
    window.draw(infoBox);
}
