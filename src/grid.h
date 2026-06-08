#pragma once
#include <cstdint>
#include <array>

#include "constants.h"

enum class CellType : std::uint8_t {
    EMPTY = 0, SAND, WATER
};

struct Grid {
    static constexpr int width { (consts::WIDTH - consts::PANEL_WIDTH) / consts::CELL_SIZE };
    static constexpr int height { consts::HEIGHT / consts::CELL_SIZE };

    std::array<std::array<CellType, width>, height> cells{};

    [[nodiscard]]
    inline bool in_bound(int y, int x) const { return y >= 0 && y < height && x >= 0 && x < width; }
    
    [[nodiscard]]
    inline bool is_empty(int y, int x) const { return cells[y][x] == CellType::EMPTY; }

    void update();
    void spawn(int y, int x, CellType cell_type);
private:
    bool update_generic_down(int y, int x);
    bool update_generic_sink(int y, int x);
    bool update_generic_side(int y, int x);
    void update_sand(int y, int x);
    void update_water(int y, int x);
    void move(int sy, int sx, int dy, int dx);
    void swap(int sy, int sx, int dy, int dx);
};