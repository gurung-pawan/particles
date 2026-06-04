#include <optional>
#include <print>

#include <SFML/Graphics.hpp>

#include "sim.h"
#include "constants.h"

Sim::Sim() {
    init_window();
}

// --- Initializers --- //

void Sim::init_window() {
    sf::ContextSettings context_settings;
    context_settings.antiAliasingLevel = 8;

    window = { sf::VideoMode({ consts::WIDTH, consts::HEIGHT }), "Particles", sf::Style::Close, sf::State::Windowed, context_settings };
    window.setFramerateLimit(consts::FRAME_LIMIT);
}

// --- Sim Events --- //
void Sim::poll_events() {
    while (std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (event->is<sf::Event::MouseButtonPressed>()) {
            is_mouse_held = true;
        } else if (event->is<sf::Event::MouseButtonReleased>()) {
            is_mouse_held = false;
        }
    }
}

// --- Sim Updates --- //
void Sim::update() {
    if (is_mouse_held) { 
        mouse_pos = sf::Mouse::getPosition(window);
        grid.spawn(
            mouse_pos.y / consts::CELL_SIZE,
            mouse_pos.x / consts::CELL_SIZE,
            CellType::WATER
        );
        grid.spawn(
            (mouse_pos.y / consts::CELL_SIZE) - 1,
            mouse_pos.x / consts::CELL_SIZE,
            CellType::WATER
        );
        grid.spawn(
            (mouse_pos.y / consts::CELL_SIZE) + 1,
            mouse_pos.x / consts::CELL_SIZE,
            CellType::WATER
        );
        grid.spawn(
            mouse_pos.y / consts::CELL_SIZE,
            (mouse_pos.x / consts::CELL_SIZE) - 1,
            CellType::WATER
        );
        grid.spawn(
            mouse_pos.y / consts::CELL_SIZE,
            (mouse_pos.x / consts::CELL_SIZE) + 1,
            CellType::WATER
        );
    }

    grid.update();
}

void Sim::render_grid() {
    sf::RectangleShape rs {
        sf::Vector2f {
            static_cast<float>(consts::CELL_SIZE),
            static_cast<float>(consts::CELL_SIZE) 
        }
    };

    for (int i = 0; i < grid.cells.size(); ++i) {
        for (int j = 0; j < grid.cells[i].size(); ++j) {
            if (grid.cells[i][j] == CellType::WATER) {
                rs.setFillColor(sf::Color{ 0, 0, 128 });
                rs.setPosition(
                    sf::Vector2f {
                        static_cast<float>(consts::CELL_SIZE * j),
                        static_cast<float>(consts::CELL_SIZE * i)
                    }
                );
                window.draw(rs);
            }
        }
    }
}

// --- Sim Renders --- //
void Sim::render() {
    window.clear(sf::Color::Black);
    render_grid();
    window.display();
}

// --- Sim entry point --- //
void Sim::run() {
    while (window.isOpen()) {
        delta_t = clock.restart().asSeconds();
        poll_events();
        update();
        render();
    }
}