#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>

#include "grid.h"

class Sim {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    sf::Vector2i mouse_click_pos {};
    bool is_mouse_clicked {};

    std::vector<sf::RectangleShape> cells {};

    float delta_t{};
    Grid grid;

    void init_window();

    void poll_events();
    void update();
    void render();
public:
    Sim();

    void run();
};