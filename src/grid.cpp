#include "grid.h"
#include "utils.h"

#include <vector>

void Grid::move(int sy, int sx, int dy, int dx) {
    cells[dy][dx] = cells[sy][sx];
    cells[sy][sx] = CellType::EMPTY;
}

void Grid::swap(int sy, int sx, int dy, int dx) {
    CellType temp_cell = cells[sy][sx];
    cells[sy][sx] = cells[dy][dx];
    cells[dy][dx] = temp_cell;
}

bool Grid::update_generic_down(int y, int x) {
    const int dwn { y + 1 };
    
    if (in_bound(dwn, x)) { 
        if (is_empty(dwn, x)) {
            move(y, x, dwn, x);
            return true;
        } else if (cells[dwn][x] == CellType::STONE) {
            return false;
        }
    }

    bool mv_ld { in_bound(dwn, x - 1) && is_empty(dwn, x - 1) };
    bool mv_rd { in_bound(dwn, x + 1) && is_empty(dwn, x + 1) };

    if (mv_ld && mv_rd) {
        int dir = (2 * utils::get_random_int(0, 1)) - 1;
        move(y, x, dwn, x + dir);
    }
    else if (mv_ld) move(y, x, dwn, x - 1);
    else if (mv_rd) move(y, x, dwn, x + 1);

    return mv_ld || mv_rd;
}

bool Grid::update_generic_side(int y, int x) {
    bool mv_l { in_bound(y, x - 1) && is_empty(y, x - 1) };
    bool mv_r { in_bound(y, x + 1) && is_empty(y, x + 1) };

    if (mv_l && mv_r) {
        int dir = (2 * utils::get_random_int(0, 1)) - 1;
        move(y, x, y, x + dir);
    }
    else if (mv_l) move(y, x, y, x - 1);
    else if (mv_r) move(y, x, y, x + 1);

    return mv_l || mv_r;
}

bool Grid::update_generic_sink(int y, int x) {
    const int dwn { y + 1 }; 

    std::array<std::pair<int, int>, 3> moves {};

    int mv_c = 0;
    if (in_bound(dwn, x) && cells[dwn][x] == CellType::WATER) moves[mv_c++] = { dwn, x };
    if (in_bound(dwn, x - 1) && cells[dwn][x - 1] == CellType::WATER) moves[mv_c++] = { dwn, x - 1 };
    if (in_bound(dwn, x + 1) && cells[dwn][x + 1] == CellType::WATER) moves[mv_c++] = {dwn, x + 1 };

    if (mv_c == 0) return false; // no cell to move

    int dir = utils::get_random_int(0, mv_c);
    if (mv_c == dir) return false; // a probability of sand not falling
    auto [dy, dx] = moves[dir];
    swap(y, x, dy, dx);

    return true;
}

void Grid::update_sand(int y, int x) {
    update_generic_down(y, x);
    update_generic_sink(y, x);
}

void Grid::update_water(int y, int x) {
    if (update_generic_down(y, x)) return;
    if (update_generic_side(y, x)) return;
}

void Grid::update() {

    for (int i = height - 2; i >= 0; --i) {
        for (int j = width - 1; j >= 0; --j) {
            if (is_empty(i, j)) continue;

            switch (cells[i][j]) {
                case CellType::SAND: update_sand(i, j); break;
                case CellType::WATER: update_water(i, j); break;
            }
        }
    }
}

void Grid::spawn(int y, int x, CellType cell_type) {
    if (!in_bound(y, x)) return;
    if (!is_empty(y, x) && cell_type != CellType::EMPTY) return;
    cells[y][x] = cell_type; 
}

void Grid::clear() {
    for (auto& row : cells) {
        row.fill(CellType::EMPTY);
    }
}
