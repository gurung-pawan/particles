#pragma once

#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "grid.h"

class Sim {
private:
    static constexpr std::array materials = {
        std::pair{ CellType::SAND, "Sand" },
        std::pair{ CellType::WATER, "Water" },
        std::pair{ CellType::STONE, "Stone" },
        std::pair{ CellType::EMPTY, "Eraser" }
    };
    sf::RenderWindow window;
    sf::Clock clock;
    Grid grid;

    sf::Vector2i mouse_pos;

    bool is_mouse_held{}, paused{};
    int brush_size{ 1 };
    sf::CircleShape brush;

    CellType selected_type { CellType::SAND };

    void init_window();

    void poll_events();
    void im_gui_update();
    void spawn_from_brush();
    void update_brush();
    void update();
    
    void render();
    void render_grid();
    void clear_grid();
public:
    Sim();
    ~Sim();

    void run();
};