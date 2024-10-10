#pragma once

#include "modules/external_command/external_driver/rtc/rtc_client.h"
#include <typeinfo>

namespace apollo {
namespace external_command {

void RtcClient::SetListener(baidurtc::BaiduRtcRoomClient* client, MyListener& listener) {
    client->registerRtcMessageListener(&listener);
}

bool RtcClient::CreateClient(const ExternalDriverConfig& config) {
    void* handle = dlopen(
            "/apollo_workspace/modules/external_command/external_driver/rtc/lib/libbaidurtc.so",
            RTLD_LAZY | RTLD_DEEPBIND);
    if (handle == nullptr) {
        AERROR << "dlopen libbaidurtc.so failed";
        return false;
    }

    f_createBaiduRtcRoomClient* Client
            = (f_createBaiduRtcRoomClient*)dlsym(handle, "_ZN8baidurtc24createBaiduRtcRoomClientEv");
    g_BrtcClient = Client();
    s.HasData = true;
    s.HasVideo = true;
    s.HasAudio = true;
    s.AudioINChannel = 1;
    s.AudioINFrequency = 16000;
    s.ImageINType = RTC_IMAGE_TYPE_JPEG;

    s.AsPublisher = true;
    s.AsListener = false;
    s.AutoPublish = true;

    s.VideoFps = 15;

    s.VideoMaxkbps = config.video_maxkbps();
    s.VideoWidth = config.image_width();
    s.VideoHeight = config.image_height();
    cer_path = config.cer_path();
    app_id = config.app_id();
    car_id = config.car_id();

    g_BrtcClient->setParamSettings(&s, s.RTC_PARAM_SETTINGS_ALL);
    g_BrtcClient->setAppID(app_id.c_str());
    g_BrtcClient->setMediaServerURL("wss://rtc.exp.bcelive.com/janus");
    g_BrtcClient->setCER(cer_path.c_str());

    std::string uid;
    std::ostringstream os;
    os << 1234500000 + rand() / 100000;
    uid = os.str();

    g_BrtcClient->loginRoom("2131", uid.c_str(), car_id.c_str(), "token");
    SetListener(g_BrtcClient, g_mylistener);
    return true;
}
void MyListener::OnRtcMessage(RtcMessage& msg) {
    msg_type = msg.msgType;
    if (msg.msgType == RtcMessageType::RTC_ROOM_EVENT_ON_USER_MESSAGE){
        recieve_msg = msg.extra_info;
        re_mark = true;
        AERROR << "recieve_msg_onrtc:" << msg.extra_info;
    }else{
        recieve_msg = "";
    }
}

}  // namespace external_command
}  // namespace apollo