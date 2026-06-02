#pragma once
#include <array>
#include <expected>

#include "constants.h"

enum class CellType : int { EMPTY, SAND, WATER, STONE };

struct Grid {
    std::array<std::array<CellType, consts::HEIGHT>, consts::WIDTH> cells{};
    std::array<std::array<bool, consts::HEIGHT>, consts::WIDTH> updated_cells{};

    inline bool is_in_bound(int x, int y) const {
        return x >= 0 && x < consts::WIDTH && y >= 0 && y < consts::HEIGHT; 
    }

    inline bool is_empty(int x, int y) const { return is_in_bound(x, y) && cells[x][y] == CellType::EMPTY; }
    bool move_cell(int sx, int sy, int dx, int dy);

    void update_grid();
    bool spawn(int x, int y, CellType cell_type);
private:
    void update_sand(int x, int y);
    void update_water(int x, int y);
};