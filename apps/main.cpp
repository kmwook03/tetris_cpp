#include "tetris/ui.hpp"
#include <chrono>
#include <thread>

int main() {
    GameState state;
    ui_init();
    init_game(state);
    
    const auto update_interval = std::chrono::milliseconds(50);
    const auto input_polling_interval = std::chrono::milliseconds(10);
    auto last_update_time = std::chrono::steady_clock::now();
    
    while (state.running) {
        auto current_time = std::chrono::steady_clock::now();
        bool state_changed = false;
        
        // 모든 입력 처리 (입력 반응성 향상)
        int action;
        while ((action = ui_get_input()) != ACTION_NONE) {
            handle_game_action(state, action);
            state_changed = true;
            if (!state.running) break;
        }
        
        // 게임 로직 업데이트 (중력 등)
        if (current_time - last_update_time >= update_interval) {
            update_game(state);
            state_changed = true;
            last_update_time = current_time;
        }
        
        // 상태가 변했을 때만 화면 그리기 (불필요한 redraw 감소)
        if (state_changed) {
            ui_draw(state);
        }
        
        // 짧은 대기 (CPU 점유율 감소 및 입력 감도 조절)
        std::this_thread::sleep_for(input_polling_interval);
    }
    
    ui_cleanup(state);
    return 0;
}