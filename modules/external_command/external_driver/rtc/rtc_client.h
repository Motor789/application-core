#pragma once

#include <string>
#include <dlfcn.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <cstring>
#include <inttypes.h>
#include <unistd.h>
#include "modules/external_command/external_driver/rtc/BaiduRtcInterface.h"
#include "modules/external_command/external_driver/proto/external_driver_config.pb.h"

#include "cyber/cyber.h"
namespace apollo {
namespace external_command {

class MyListener : public IRtcMessageListener {
public:
    MyListener() = default;
    ~MyListener() = default;
    int msg_type;
    std::string recieve_msg = "";
    bool re_mark = false;

public:
    void OnRtcMessage(RtcMessage& msg) override;
};

class RtcClient {
public:
    RtcClient() = default;
    virtual ~RtcClient() = default;
    void SetListener(baidurtc::BaiduRtcRoomClient* client, MyListener& listener);
    bool CreateClient(const ExternalDriverConfig& config);
    MyListener g_mylistener;
    baidurtc::BaiduRtcRoomClient* g_BrtcClient;

private:
    std::string cer_path;
    std::string app_id;
    std::string lib_path;
    std::string car_id;
    typedef baidurtc::BaiduRtcRoomClient* f_createBaiduRtcRoomClient();

    RtcParameterSettings s;

    // RemoteConfig config;
};

}  
}// namespace apollo