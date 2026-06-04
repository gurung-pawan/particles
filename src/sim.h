#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "grid.h"

class Sim {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    Grid grid;

    sf::Vector2i mouse_pos;

    float delta_t{};
    bool is_mouse_held{};

    void init_window();

    void poll_events();
    void update();
    void render();
    void render_grid();
public:
    Sim();

    void run();
};