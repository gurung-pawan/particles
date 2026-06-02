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
        }
        else if (event->is<sf::Event::MouseButtonPressed>()) {
            is_mouse_clicked = true;
            mouse_click_pos = sf::Mouse::getPosition(window);
        }
        else if (is_mouse_clicked && event->is<sf::Event::MouseMoved>()) {
            mouse_click_pos = sf::Mouse::getPosition(window);
        }
        else if (event->is<sf::Event::MouseButtonReleased>()) {
            is_mouse_clicked = false;
        }
    }
}

// --- Sim Updates --- //
void Sim::update() {
    int offset[] = { -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5 };
    grid.update_grid();
    cells.clear();
    if (is_mouse_clicked) {
        for (size_t i = 0; i < 11; i++) {
            for (size_t j = 0; j < 11; j++) {
                grid.spawn(mouse_click_pos.x + offset[i], mouse_click_pos.y + offset[j], CellType::SAND);
            }
        }
    }
    for (size_t i = 0; i < grid.cells.size(); ++i) {
        for (size_t j = 0; j < grid.cells[i].size(); ++j) {
            if (grid.cells[i][j] == CellType::SAND) {
                sf::RectangleShape rs { sf::Vector2f { 1.f, 1.f }};
                rs.setFillColor(sf::Color::Yellow);
                rs.setPosition(sf::Vector2f { static_cast<float>(i), static_cast<float>(j) });
                cells.push_back(rs);
            }
        }
    }
    
}

// --- Sim Renders --- //
void Sim::render() {
    window.clear(sf::Color::Black);
    for (auto& rs : cells) {
        window.draw(rs);
    }
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