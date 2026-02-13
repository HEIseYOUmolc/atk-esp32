#ifndef _DEVICE_STATE_H_
#define _DEVICE_STATE_H_

enum DeviceState {/*设备状态枚举类型定义*/
    kDeviceStateUnknown,/*未知*/
    kDeviceStateStarting,/*启动中*/
    kDeviceStateWifiConfiguring,/*WiFi配网*/
    kDeviceStateIdle,/*空闲状态*/
    kDeviceStateConnecting,/*空闲状态*/
    kDeviceStateListening,/*监听状态*/
    kDeviceStateSpeaking,/*说话状态*/
    kDeviceStateUpgrading,/*升级状态*/
    kDeviceStateActivating,/*激活状态*/
    kDeviceStateAudioTesting,/*音频测试状态*/
    kDeviceStateFatalError/*严重错误状态*/
};

#endif // _DEVICE_STATE_H_ 