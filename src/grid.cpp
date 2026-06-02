#include "grid.h"

#include <print>
bool Grid::move_cell(int sx, int sy, int dx, int dy) {
    if (!is_in_bound(sx, sy) || !is_in_bound(dx, dy)) return false;
    
    cells[dx][dy] = cells[sx][sy];
    cells[sx][sy] = CellType::EMPTY;
    updated_cells[dx][dy] = true;
    return true;
}

void Grid::update_sand(int x, int y) {
    int down = y + 1;
    if (is_empty(x, down) && move_cell(x, y, x, down)) return;
    if (is_empty(x - 1, down) && move_cell(x, y, x - 1, down)) return;
    if (is_empty(x + 1, down) && move_cell(x, y, x + 1, down)) return;
}

void Grid::update_water(int x, int y) {
    // to do
}

bool Grid::spawn(int x, int y, CellType cell_type) {
    if (!is_in_bound(x, y) && !is_empty(x, y)) return false;
    cells[x][y] = cell_type;
    return true;
}

void Grid::update_grid() {
    for (auto& col : updated_cells) {
        col.fill(false);
    }

    for (int i = consts::WIDTH - 1; i >= 0; --i) {
        for (int j = consts::HEIGHT - 2; j >= 0; --j) {
            if (updated_cells[i][j]) continue;

            switch (cells[i][j]) {
                case CellType::SAND: update_sand(i, j); break;
                case CellType::WATER: update_water(i, j); break;
            }
        }
    }
}
