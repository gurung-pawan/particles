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

    bool is_mouse_held{};
    CellType selected_type { CellType::SAND };

    void init_window();

    void poll_events();
    void update();
    void im_gui_update();
    void render();
    void render_grid();
public:
    Sim();
    ~Sim();

    void run();
};