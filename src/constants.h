#pragma once

namespace consts {
    constexpr unsigned int PANEL_WIDTH = 300;
    constexpr unsigned int WIDTH = 800 + PANEL_WIDTH;
    constexpr unsigned int HEIGHT = 600;
    constexpr unsigned int FRAME_LIMIT = 60;
    constexpr unsigned int CELL_SIZE = 8;

    enum class error {
        OUT_OF_BOUNDS
    };
} // namespace consts
