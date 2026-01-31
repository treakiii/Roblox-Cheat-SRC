#include "../../hook.h"
#include "../../../util/console/console.h"
#include <chrono>
#include <thread>
#include <iostream>

void hooks::autoslide() {
    static int last_state_id = -1;
    static std::chrono::steady_clock::time_point last_spam_time;
    
    while (true) {
        if (!globals::firstreceived) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }

        if (globals::misc::autoslide) {
            auto humanoid = globals::instances::lp.humanoid;
            if (!is_valid_address(humanoid.address)) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }

            try {
                uintptr_t state_instance = read<uintptr_t>(humanoid.address + offsets::HumanoidState);
                if (state_instance != 0) {
                    int state_id = read<int>(state_instance + offsets::HumanoidStateId);
                    
                    if (state_id == 8) {
                        auto now = std::chrono::steady_clock::now();
                        auto spam_duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_spam_time).count();
                        
                        if (spam_duration >= 80) {
                            last_spam_time = now;
                            
                            keybd_event('C', MapVirtualKey('C', MAPVK_VK_TO_VSC), 0, 0);
                            std::this_thread::sleep_for(std::chrono::milliseconds(20));
                            keybd_event('C', MapVirtualKey('C', MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
                            
                            std::this_thread::sleep_for(std::chrono::milliseconds(30));
                            
                            keybd_event(VK_SPACE, MapVirtualKey(VK_SPACE, MAPVK_VK_TO_VSC), 0, 0);
                            std::this_thread::sleep_for(std::chrono::milliseconds(20));
                            keybd_event(VK_SPACE, MapVirtualKey(VK_SPACE, MAPVK_VK_TO_VSC), KEYEVENTF_KEYUP, 0);
                        }
                    }
                    
                    last_state_id = state_id;
                }
            }
            catch (...) {
            }
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
