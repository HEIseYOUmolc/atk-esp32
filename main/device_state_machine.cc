#include "device_state_machine.h"

#include <algorithm>
#include <esp_log.h>

static const char* TAG = "StateMachine";/*日志标签状态机*/

// State name strings for logging
static const char* const STATE_STRINGS[] = {/*对应device_state.h的枚举类型*/
    "unknown",
    "starting",
    "wifi_configuring",
    "idle",
    "connecting",
    "listening",
    "speaking",
    "upgrading",
    "activating",
    "audio_testing",
    "fatal_error",
    "invalid_state"
};

DeviceStateMachine::DeviceStateMachine() {/*DeviceStateMachine类建构函数*/
}

const char* DeviceStateMachine::GetStateName(DeviceState state) {/*获取状态名称*/
    if (state >= 0 && state <= kDeviceStateFatalError) {
        return STATE_STRINGS[state];
    }
    return STATE_STRINGS[kDeviceStateFatalError + 1];
}

bool DeviceStateMachine::IsValidTransition(DeviceState from, DeviceState to) const {/*检查状态转换是否合法*/
    // Allow transition to the same state (no-op)
    if (from == to) {/*相同状态跳转：允许*/
        return true;
    }

    // Define valid state transitions based on the state diagram
    switch (from) {
        case kDeviceStateUnknown:/*未知-》启动中*/
            return to == kDeviceStateStarting;

        case kDeviceStateStarting:/*启动中-》wifi配置状态 或 激活状态*/
            return to == kDeviceStateWifiConfiguring ||
                   to == kDeviceStateActivating;

        case kDeviceStateWifiConfiguring:/*配置状态-》激活状态或音频测试状态*/
            return to == kDeviceStateActivating ||
                   to == kDeviceStateAudioTesting;

        case kDeviceStateAudioTesting:/*音频测试状态-》Wifi配置状态*/
            return to == kDeviceStateWifiConfiguring;

        case kDeviceStateActivating:/*激活状态-》升级状态、空闲状态或WiFi配置状态*/
            return to == kDeviceStateUpgrading ||
                   to == kDeviceStateIdle ||
                   to == kDeviceStateWifiConfiguring;

        case kDeviceStateUpgrading:/*升级状态-》空闲状态 或 激活状态*/
            return to == kDeviceStateIdle ||
                   to == kDeviceStateActivating;

        case kDeviceStateIdle:/*空闲状态-》连接状态、监听状态、说话状态、激活状态、升级状态或WiFi配置状态*/
            return to == kDeviceStateConnecting ||
                   to == kDeviceStateListening ||
                   to == kDeviceStateSpeaking ||
                   to == kDeviceStateActivating ||
                   to == kDeviceStateUpgrading ||
                   to == kDeviceStateWifiConfiguring;

        case kDeviceStateConnecting:/*连接状态-》空闲状态 或 监听状态*/
            return to == kDeviceStateIdle ||
                   to == kDeviceStateListening;

        case kDeviceStateListening:/*监听状态-》说话状态 或 空闲状态*/
            return to == kDeviceStateSpeaking ||
                   to == kDeviceStateIdle;

        case kDeviceStateSpeaking:/*说话状态-》监听状态 或 空闲状态*/
            return to == kDeviceStateListening ||
                   to == kDeviceStateIdle;

        case kDeviceStateFatalError:/*错误状态不允许转换成其他状态*/
            // Cannot transition out of fatal error
            return false;

        default:
            return false;
    }
}

bool DeviceStateMachine::CanTransitionTo(DeviceState target) const {/*状态机外部函数：查询转换是否运行*/
    return IsValidTransition(current_state_.load(), target);
}

bool DeviceStateMachine::TransitionTo(DeviceState new_state) {/*状态机外部函数：状态迁移*/
    DeviceState old_state = current_state_.load();/*读*/
    
    // No-op if already in the target state
    if (old_state == new_state) {/*相同状态跳转，直接返回真*/
        return true;
    }

    // Validate transition
    if (!IsValidTransition(old_state, new_state)) {/*状态转换合法性判断，日志输出并返回假*/
        ESP_LOGW(TAG, "Invalid state transition: %s -> %s",
                 GetStateName(old_state), GetStateName(new_state));
        return false;
    }

    // Perform transition
    current_state_.store(new_state);/*执行状态切换*/
    ESP_LOGI(TAG, "State: %s -> %s",
             GetStateName(old_state), GetStateName(new_state));

    // Notify callback
    NotifyStateChange(old_state, new_state);/*状态变更通知回调函数*/
    return true;
}
/*状态变化通知机制*/
int DeviceStateMachine::AddStateChangeListener(StateCallback callback) {
    std::lock_guard<std::mutex> lock(mutex_);
    int id = next_listener_id_++;
    listeners_.emplace_back(id, std::move(callback));
    return id;
}

void DeviceStateMachine::RemoveStateChangeListener(int listener_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    listeners_.erase(
        std::remove_if(listeners_.begin(), listeners_.end(),
            [listener_id](const auto& p) { return p.first == listener_id; }),
        listeners_.end());
}

void DeviceStateMachine::NotifyStateChange(DeviceState old_state, DeviceState new_state) {
    std::vector<StateCallback> callbacks_copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        callbacks_copy.reserve(listeners_.size());
        for (const auto& [id, cb] : listeners_) {
            callbacks_copy.push_back(cb);
        }
    }
    
    for (const auto& cb : callbacks_copy) {
        cb(old_state, new_state);
    }
}
