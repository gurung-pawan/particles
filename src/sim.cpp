#include <optional>

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "sim.h"
#include "constants.h"

Sim::Sim() {
    init_window();
    if (!ImGui::SFML::Init(window)) exit(EXIT_FAILURE);
    brush.setRadius(brush_size * consts::CELL_SIZE);
    brush.setFillColor(sf::Color::Transparent);
    brush.setOutlineColor(sf::Color{ 128, 128, 128 });
    brush.setOutlineThickness(2.f);
    brush.setOrigin(sf::Vector2f(
        static_cast<float>(brush_size * consts::CELL_SIZE), static_cast<float>(brush_size * consts::CELL_SIZE)
    ));
}

Sim::~Sim() {
    ImGui::SFML::Shutdown();
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

        ImGui::SFML::ProcessEvent(window, *event);
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (event->is<sf::Event::MouseButtonPressed>()) {
            if (!ImGui::GetIO().WantCaptureMouse)
                is_mouse_held = true;
        } else if (event->is<sf::Event::MouseButtonReleased>()) {
            if (!ImGui::GetIO().WantCaptureMouse)
                is_mouse_held = false;
        }
    }
}

// --- ImGui --- //
void Sim::im_gui_update() {
    ImGui::SFML::Update(window, clock.restart());

    ImGui::SetNextWindowPos(ImVec2(consts::WIDTH - consts::PANEL_WIDTH, 0));
    ImGui::SetNextWindowSize(ImVec2(consts::PANEL_WIDTH, consts::HEIGHT));
    ImGui::Begin("Options", nullptr, 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoCollapse
    );
    // Material Selection
    ImGui::Text("Materials");
    for (auto& [cell, name] : materials) {
        if (ImGui::RadioButton(name, selected_type == cell))
            selected_type = cell;
        ImGui::SameLine();
    }
    ImGui::NewLine();

    // Control buttons
    ImGui::Spacing();
    if (ImGui::Button("Clear")) {
        clear_grid();
    }
    ImGui::SameLine();
    if (ImGui::Button(paused ? "Play" : "Pause")) {
        paused = !paused;
    }

    // Brush options
    ImGui::Spacing();
    ImGui::SliderInt("Brush Size", &brush_size, 0, 4);

    ImGui::End();
}

void Sim::spawn_from_brush() {
    int cy { mouse_pos.y / static_cast<int>(consts::CELL_SIZE) };
    int cx { mouse_pos.x / static_cast<int>(consts::CELL_SIZE) };
    int r { brush_size };

    // Spawning in the circle all points (x, y) | (x - cx)^2 + (y - cy)^2 <= r^2
    // lower and upper bound are (center - radius) and (center + radius) respectively
 
    for (int x = (cx - r); x <= (cx + r); ++x) {
        for (int y = (cy - r); y <= (cy + r); ++y) {
            int px { x - cx }, py { y - cy };
            if (px * px + py * py <= r * r) {
                grid.spawn(y, x, selected_type);
            }
        }
    }
}

void Sim::update_brush() {
    brush.setPosition(
        sf::Vector2f {
            static_cast<float>(mouse_pos.x),
            static_cast<float>(mouse_pos.y)
        }
    );
    if (brush.getRadius() != brush_size) {
        brush.setRadius(brush_size * consts::CELL_SIZE);
        brush.setOrigin(sf::Vector2f(
            static_cast<float>(brush_size * consts::CELL_SIZE), static_cast<float>(brush_size * consts::CELL_SIZE)
        ));
    }
}

// --- Sim Updates --- //
void Sim::update() {
    im_gui_update();
    mouse_pos = sf::Mouse::getPosition(window);
    update_brush();
    if (is_mouse_held) spawn_from_brush();
    if (!paused) grid.update();
}

void Sim::clear_grid() {
    grid.clear();
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
            rs.setPosition(
                sf::Vector2f {
                    static_cast<float>(consts::CELL_SIZE * j),
                    static_cast<float>(consts::CELL_SIZE * i)
                }
            );

            switch (grid.cells[i][j]) {
                case CellType::WATER: rs.setFillColor(sf::Color{ 0, 0, 128 }); break;
                case CellType::SAND: rs.setFillColor(sf::Color{ 194, 178, 128 }); break;
                case CellType::STONE: rs.setFillColor(sf::Color{ 64, 64, 64 }); break;
            }

            if (grid.cells[i][j] != CellType::EMPTY) {
                window.draw(rs);
            }
        }
    }
}

// --- Sim Renders --- //
void Sim::render() {
    window.clear(sf::Color::Black);
    render_grid();
    window.draw(brush);
    ImGui::SFML::Render(window);
    window.display();
}

// --- Sim entry point --- //
void Sim::run() {
    while (window.isOpen()) {
        poll_events();
        update();
        render();
    }
}