/***************************************************************************
 *
 * Copyright (c) 2020 Baidu.com, Inc. All Rights Reserved
 *
 * @author: shaohongsheng@baidu.com
 *
 ***************************************************************************/

#pragma once

#define BAIDU_RTC_EXPORT_API


#define BAIDU_RTC_MAX_COMMON_ARRAY_LEN 512
#define BAIDU_RTC_OPTION_ONLY_PULL_VIDEO_STREAMING 0

/*
 *
 *  连接类型定义
 *
 **/
typedef enum BAIDU_RTC_EXPORT_API ConnectionType {
	RTC_SIGNAL_CHANNEL_CONNECTION,
	RTC_MEDIA_CHANNEL_CONNECTION,
}ConnectionType;

/*
 *
 *  网络连接状态
 *
 **/
typedef enum BAIDU_RTC_EXPORT_API ConnectionStats {
	RTC_CONNECTION_STATS_UNKNOW = 400,
	RTC_CONNECTION_STATS_DISCONNECTED,
	RTC_CONNECTION_STATS_CONNECTING,
	RTC_CONNECTION_STATS_CONNECTED,
	RTC_CONNECTION_STATS_RECONNECTING,
	RTC_CONNECTION_STATS_FAILED,
}ConnectionStats;

/*
 *
 * 网络类型定义
 *
 **/
typedef enum BAIDU_RTC_EXPORT_API NetworkType {
	NETWORK_TYPE_UNKNOWN,
	NETWORK_TYPE_UNARRIVAL,
	NETWORK_TYPE_LAN,
	NETWORK_TYPE_WIFI,
}NetworkType;

/*
 * 视频颜色空间类型,RTC SDK 支持下面两种颜色空间的转换
 *
 *VIDEO_COLOR_FORMAT_ARGB 表示RGB颜色空间
 *VIDEO_COLOR_FORMAT_I420 标识YUV420颜色空间
 *
 **/
typedef enum BAIDU_RTC_EXPORT_API VideoColorFormat {
	VIDEO_COLOR_FORMAT_ARGB = 100,
	VIDEO_COLOR_FORMAT_I420,
}VideoColorFormat;



/*
 *
 * 消息类型定义
 *
 **/
typedef enum BAIDU_RTC_EXPORT_API RtcMessageType {
	// //正在建立信令通道
	// RTC_MESSAGE_ESTABLISHING_SIGNAL_CHENNAL = 200,

	// //建立信令通道成功
	// RTC_MESSAGE_ESTABLISH_SIGNAL_CHENNAL_SUCCESS,

	// //建立信令通道失败
	// RTC_MESSAGE_ESTABLISH_SIGNAL_CHENNAL_FAIL,

	// //信令通道链接断开
	// RTC_MESSAGE_SIGNAL_CHENNAL_DISCONNECTED,

	// //信令通道建连超时
	// RTC_MESSAGE_SIGNAL_CHENNAL_TIMEOUT,

	// //登录房间成功
	// RTC_MESSAGE_LOGIN_ROOM_SUCCESS,

	// //登录房间失败
	// RTC_MESSAGE_LOGIN_ROOM_FAILED,

	// //用户离开房间
	// RTC_MESSAGE_USER_LEAVING_ROOM,

	// //用户加入房间
	// RTC_MESSAGE_USER_JOINED_ROOM,


	// //正在建立媒体通道
	// RTC_MESSAGE_ESTABLISHING_MEDIA_CHANNEL = 300,

	// //建立媒体通道成功
	// RTC_MESSAGE_ESTABLISH_MEDIA_CHANNEL_SUCCESS,

	// //建立媒体通道失败
	// RTC_MESSAGE_ESTABLISH_MEDIA_CHANNEL_FAILED,

	// //媒体通道链接断开
	// RTC_MESSAGE_ESTABLISH_MEDIA_CHANNEL_DISCONNECTED,

	// //媒体通道建连超时
	// RTC_MESSAGE_ESTABLISH_MEDIA_CHANNEL_TIMEOUT,

	// //媒体通道关闭
	// RTC_MESSAGE_MEDIA_CHANNEL_CLOSED,

	// //开始订阅流
	// RTC_MESSAGE_MEDIA_SUBSCRIBING_STREAMING = 500,

	// //媒体流到达
	// RTC_MESSAGE_MEDIA_STREAMING_ARRIVAL,

	// //数据流到达
	// RTC_MESSAGE_DATA_STREAMING_ARRIVAL,

	// //首帧视频到达
	// RTC_MESSAGE_VIDEO_STREAMING_FIRST_FRAME,

	// //媒体链路不稳定
	// RTC_MESSAGE_NETWORK_LINK_INSTABILITY,

	// //网络不可达
	// RTC_MESSAGE_BE_UNABLE_TO_NETWORK,

	// //camera采集开始
	// RTC_MESSAGE_LOCAL_VIDEO_CAPTURER_START = 600,

	// //camera采集停止
	// RTC_MESSAGE_LOCAL_VIDEO_CAPTURER_STOP,

	RTC_MESSAGE_ROOM_EVENT_LOGIN_OK                    = 100,
	RTC_MESSAGE_ROOM_EVENT_LOGIN_TIMEOUT               = 101,
	RTC_MESSAGE_ROOM_EVENT_LOGIN_ERROR                 = 102,
	RTC_MESSAGE_ROOM_EVENT_CONNECTION_LOST             = 103,
	RTC_MESSAGE_ROOM_EVENT_REMOTE_COMING               = 104,
	RTC_MESSAGE_ROOM_EVENT_REMOTE_LEAVING              = 105,
	RTC_MESSAGE_ROOM_EVENT_REMOTE_RENDERING            = 106,
	RTC_MESSAGE_ROOM_EVENT_REMOTE_GONE                 = 107,
	RTC_MESSAGE_ROOM_EVENT_SERVER_ERROR                = 108,

	RTC_ROOM_EVENT_AVAILABLE_SEND_BITRATE      		   = 200,
    RTC_ROOM_EVENT_FORCE_KEY_FRAME          		   = 201,

	RTC_ROOM_EVENT_ON_USER_JOINED_ROOM                 = 300,
	RTC_ROOM_EVENT_ON_USER_LEAVING_ROOM                = 301,
	RTC_ROOM_EVENT_ON_USER_MESSAGE             		   = 302,
	RTC_ROOM_EVENT_ON_USER_ATTRIBUTE                   = 303,

	RTC_MESSAGE_STATE_STREAM_UP                        = 2000,
	RTC_MESSAGE_STATE_SENDING_MEDIA_OK                 = 2001,
	RTC_MESSAGE_STATE_SENDING_MEDIA_FAILED             = 2002,
	RTC_MESSAGE_STATE_STREAM_DOWN                      = 2003,
	RTC_STATE_STREAM_SLOW_LINK_NACKS                   = 2100,
}RtcMessageType;


/*
 *
 * 消息结构体
 *
 * @discuss 当注册了消息监听接口给sdk后，sdk会抛消息上来，消息的结构如下；
 *
 *
 **/
typedef struct BAIDU_RTC_EXPORT_API RtcMessage {
	RtcMessageType msgType;
	union {
		int64_t feedId;
		int64_t streamId;
		int64_t errorCode;
	}data;
	const char* extra_info;
	int frameObserverIndex;
}RtcMessage;


/*
 *
 * 消息监听接口定义
 *
 * @discuss 需要开发者实现，并注册给rtc sdk;
 *
 *
 **/
class BAIDU_RTC_EXPORT_API IRtcMessageListener {
public:
	virtual void OnRtcMessage(RtcMessage &msg) = 0;
};

/*
 *
 * 视频设备信息结构体
 *
 * @discuss 管理视频设备;
 *
 *
 **/
typedef struct BAIDU_RTC_EXPORT_API RtcVideoDeviceInfo {
	char deviceId[BAIDU_RTC_MAX_COMMON_ARRAY_LEN];
	char deviceName[BAIDU_RTC_MAX_COMMON_ARRAY_LEN];
}RtcVideoDeviceInfo;

/*
 *
 * 房间用户角色
 *
 * @param RTC_ROOM_USER_ROLE_PUBLISHER, 角色为流发布者
 * @param RTC_ROOM_USER_ROLE_LISTENER, 角色为流监听者
 *
 **/
typedef enum BAIDU_RTC_EXPORT_API RtcRoomUserRole {
	RTC_ROOM_USER_ROLE_PUBLISHER,
	RTC_ROOM_USER_ROLE_LISTENER,
}RtcRoomUserRole;


/*
 *
 * 房间用户信息
 *
 * @param userRole, 用户角色
 * @param userId, 用户id
 * @param roomId, 房间id
 *
 **/
typedef struct BAIDU_RTC_EXPORT_API RtcRoomUserInfo {
	RtcRoomUserRole userRole;
	int64_t userId;
	int64_t roomId;
}RtcRoomUserInfo;

enum RtcImageType {
	RTC_IMAGE_TYPE_NONE,
	RTC_IMAGE_TYPE_JPEG,
	RTC_IMAGE_TYPE_H264,
	RTC_IMAGE_TYPE_I420P,
	RTC_IMAGE_TYPE_RGB,
    RTC_IMAGE_TYPE_VP8,
};

class RtcParameterSettings {
    public:
		bool 	HasVideo   = true;
		bool 	HasAudio   = true;
		bool 	HasData    = false;
		int     AudioINFrequency = 48000;
		int     AudioINChannel = 1;
		int     AudioOUTFrequency = 16000;
		int     AudioOUTChannel = 1;
		int		VideoWidth = 640;
		int		VideoHeight = 480;
		int     VideoFps = 20;
		int     VideoMaxkbps = 1500;
		int     VideoMinkbps = 1000;
		int     ImageINType = RtcImageType::RTC_IMAGE_TYPE_NONE;
		int     ImageOUTType = RtcImageType::RTC_IMAGE_TYPE_H264;
		int     ConnectionTimeoutMs = 5000;
		int     ReadTimeoutMs = 5000;
		bool	AutoPublish   = true;
		bool	AutoSubscribe = true;
		bool	AsPublisher   = true;
		bool	AsListener = true;
    static RtcParameterSettings* getDefaultSettings() {
        return new RtcParameterSettings();
    }

    enum RtcParamSettingType {
        RTC_AUDIO_PARAM_SETTINGS_ONLY_AUDIO,
        RTC_AUDIO_PARAM_SETTINGS_AEC_DUMP,
        RTC_AUDIO_PARAM_SETTINGS_LEVEL_CONTROL,
        RTC_AUDIO_PARAM_SETTINGS_MANUAL_CONFIG,
        RTC_AUDIO_PARAM_SETTINGS_EXPORT_RECORD,
        RTC_AUDIO_PARAM_SETTINGS_EXPORT_PLAYOUT,
        RTC_AUDIO_PARAM_SETTINGS_EXPORT_RECORD_PLAYOUT_MIX,
        RTC_VIDEO_PARAM_SETTINGS_FPS,
        RTC_VIDEO_PARAM_SETTINGS_RESOLUTION,
        RTC_VIDEO_PARAM_SETTINGS_BITRATE,
        RTC_VIDEO_PARAM_SETTINGS_CODECTYPE,
        RTC_VIDEO_PARAM_SETTINGS_SESSION_MODE,
        RTC_VIDEO_PARAM_SETTINGS_CAPTURE_MODE,
        RTC_VIDEO_PARAM_SETTINGS_RENDER_MODE,
        RTC_PARAM_SETTINGS_ALL,
        RTC_PARAM_SETTINGS_OTHERS
    };
};