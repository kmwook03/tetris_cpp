#include "tetris/ui.hpp"
#include <ncurses.h>

void ui_init() {
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);
    timeout(0);

    if (has_colors()) {
        start_color();
        // 블록별 색상 정의 (I, O, T, S, Z, J, L)
        init_pair(1, COLOR_CYAN, COLOR_BLACK);   // I
        init_pair(2, COLOR_YELLOW, COLOR_BLACK); // O
        init_pair(3, COLOR_MAGENTA, COLOR_BLACK);// T
        init_pair(4, COLOR_GREEN, COLOR_BLACK);  // S
        init_pair(5, COLOR_RED, COLOR_BLACK);    // Z
        init_pair(6, COLOR_BLUE, COLOR_BLACK);   // J
        init_pair(7, COLOR_WHITE, COLOR_BLACK);  // L
        
        // 테두리 및 텍스트 색상
        init_pair(8, COLOR_WHITE, COLOR_BLACK);
    }
}

void ui_cleanup(const GameState& state) {
    if (!state.running) {
        attron(COLOR_PAIR(5) | A_BOLD);
        mvprintw(BOARD_HEIGHT / 2, (BOARD_WIDTH + 10) / 2 - 5, " GAME OVER ");
        attroff(COLOR_PAIR(5) | A_BOLD);
        refresh();
        timeout(-1); // 입력을 기다림
        getch();
    }
    endwin();
}

void ui_draw_block(int y, int x, int type) {
    if (type > 0 && type <= 7) {
        attron(COLOR_PAIR(type));
        mvaddch(y, x, '#' | A_REVERSE); // 색상 반전으로 블록 느낌 강조
        attroff(COLOR_PAIR(type));
    } else {
        mvaddch(y, x, '.');
    }
}

void ui_draw(const GameState& state) {
    int offset_x = 2;
    int offset_y = 1;

    erase(); // 화면 지우기 (잔상 방지, clear() 대신 erase() 사용하여 깜빡임 방지)

    // 테두리 그리기
    attron(COLOR_PAIR(8));
    mvaddch(offset_y - 1, offset_x - 1, '+');
    mvhline(offset_y - 1, offset_x, '-', BOARD_WIDTH);
    mvaddch(offset_y - 1, offset_x + BOARD_WIDTH, '+');

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        mvaddch(offset_y + y, offset_x - 1, '|');
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            int block_type = state.board[y][x];
            ui_draw_block(offset_y + y, offset_x + x, block_type);
        }
        mvaddch(offset_y + y, offset_x + BOARD_WIDTH, '|');
    }

    mvaddch(offset_y + BOARD_HEIGHT, offset_x - 1, '+');
    mvhline(offset_y + BOARD_HEIGHT, offset_x, '-', BOARD_WIDTH);
    mvaddch(offset_y + BOARD_HEIGHT, offset_x + BOARD_WIDTH, '+');
    attroff(COLOR_PAIR(8));

    // 현재 떨어지는 블록 그리기
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (state.current_block[i][j]) {
                int draw_x = offset_x + state.block_x + j;
                int draw_y = offset_y + state.block_y + i;
                if (draw_y >= offset_y && draw_y < offset_y + BOARD_HEIGHT &&
                    draw_x >= offset_x && draw_x < offset_x + BOARD_WIDTH) {
                    ui_draw_block(draw_y, draw_x, state.current_tetromino + 1);
                }
            }
        }
    }

    // 사이드바 정보
    int sidebar_x = offset_x + BOARD_WIDTH + 4;
    mvprintw(offset_y, sidebar_x, "SCORE: %d", state.score);
    mvprintw(offset_y + 1, sidebar_x, "HIGH:  %d", state.high_score);
    mvprintw(offset_y + 2, sidebar_x, "LEVEL: %d", state.level);
    mvprintw(offset_y + 3, sidebar_x, "LINES: %d", state.lines_cleared);
    
    mvprintw(offset_y + 5, sidebar_x, "NEXT:");
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (state.tetrominoes[state.next_tetromino][0][i][j]) {
                ui_draw_block(offset_y + 6 + i, sidebar_x + j, state.next_tetromino + 1);
            }
        }
    }
    
    mvprintw(offset_y + BOARD_HEIGHT - 2, sidebar_x, "CONTROLS:");
    mvprintw(offset_y + BOARD_HEIGHT - 1, sidebar_x, "ARROWS: Move/Rotate");
    mvprintw(offset_y + BOARD_HEIGHT, sidebar_x, "SPACE:  Drop, Q: Quit");
    
    refresh();
}

int convert_ncurses_key_to_action(int key) {
    switch (key) {
        case 'q': case 'Q': return ACTION_QUIT;
        case KEY_LEFT: return ACTION_LEFT;
        case KEY_RIGHT: return ACTION_RIGHT;
        case KEY_DOWN: return ACTION_DOWN;
        case KEY_UP: return ACTION_ROTATE;
        case ' ': return ACTION_DROP;
        default: return ACTION_NONE;
    }
}

int ui_get_input() {
    int ch = getch();
    return convert_ncurses_key_to_action(ch);
}
