#pragma once

#include "tetris/tetris.hpp"

void ui_init();
void ui_cleanup(const GameState& state);
void ui_draw(const GameState& state);
int ui_get_input();