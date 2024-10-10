/***************************************************************************
 *
 * Copyright (c) 2020 Baidu.com, Inc. All Rights Reserved
 *
 * @author: shaohongsheng@baidu.com
 *
 ***************************************************************************/

#pragma once
#include "BaiduExternalVideoCapturerInterface.h"
#include "BaiduExternalVideoRendererInterface.h"
#include "BaiduExternalAudioRendererInterface.h"
#include "BaiduExternalDataRendererInterface.h"
#include "BaiduRtcCommonDefine.h"
#include <string>
#include <unordered_map>

namespace baidurtc {
	class BAIDU_RTC_EXPORT_API BaiduRtcRoomClient {
	public:
		/*
		 * 设置AppID值
		 *
		 * @param AppID 百度云RTC申请的AppID
		 *
		 * @discuss 设置AppID值，百度云RTC申请的AppID
		 *
		 **/
		virtual void setAppID(const char *AppID) = 0;

		/*
		 * 设置RTC媒体服务器地址
		 *
		 * @param MediaServerURL  RTC媒体服务器地址，默认值为: wss://rtc.exp.bcelive.com:8989/janus
		 *
		 * @discuss 设置RTC媒体服务器地址
		 *
		 **/
		virtual void setMediaServerURL(const char * MediaServerURL) = 0;

		/*
		 * 设置ssl证书
		 *
		 * @param cerFile ssl证书文件路径， 用RTC信令服务器的根证书, 已经随SDK提供
		 * @return
		 * @discussion 该函数在loginRoom 前调用，用于设置ssl证书
		 *
		 **/
		virtual void setCER(const char *cerFile) = 0;

		/*
		 * 设置音视频相关的参数
		 *
		 * @param paramSettings 该参数封装了音视频的一些参数,如video分辨率，fps, bitrate，音频采样率等
		 * @param paramType 参数类型，可指定设置某一项，还是所有参数都设置
		 * @return
		 * @discussion 该函数在loginRoom 前调用，主要用于设置音视频采集，编解码相关的参数
		 *
		 **/
		virtual void setParamSettings(RtcParameterSettings* paramSettings,RtcParameterSettings::RtcParamSettingType paramType) = 0;

		/*
		 * 设置自动拉流时的视频种子源
		 *
		 * @param feedId 种子源的ID号
		 * @return
		 * @discussion  设置自动拉流时的视频种子源
		 *
		 **/
		virtual void setFeedId(const char * feedId) = 0;

		/*
		 * 登录房间
		 *
		 * @param roomName 房间名
		 * @param userId 用户ID， 64位的数字字符串
		 * @return true 成功 false 失败
		 *
		 * @discuss 登录的消息回调IRtcMessageListener
		 *
		 **/
		virtual bool loginRoom(const char* roomName, const char* userId, const char* displayName, const char* token) = 0;

		/*
		 * 登出房间
		 *
		 * @return true 成功 false 失败
		 * @discuss 登出房间
		 *
		 **/
		virtual bool logoutRoom() = 0;

		/*
		 * 销毁本对象
		 *
		 *
		 * @discuss 销毁本对象
		 *
		 **/
		virtual void Destory() = 0;

		/*
		 * 发送音频数据
		 *
		 * @param data 数据地址
		 * @param len 数据的长度
		 *
		 * @discuss 发送音频数据, 数据类型PCM数据，需要通过setParamSettings指定输入音频的采样和通道参数
		 *
		 **/
		virtual void sendAudio(const char *data, int len) = 0;

		/*
		 * 发送图像数据
		 *
		 * @param data 数据地址
		 * @param len 数据的长度
		 *
		 * @discuss 发送图像数据, 数据类型可以是none,jpeg,h264,yuv等，需要通过setParamSettings指定,默认是none即自动识别
		 *
		 **/
		virtual void sendImage(const char *data, int len) = 0;

		/*
		 * 发送文本数据
		 *
		 * @param data 文本数据地址
		 * @param len 文本数据的长度
		 *
		 * @discuss 发送文本数据
		 *
		 **/
		virtual void sendData(const char *data, int len) = 0;

		/*
		 *  开始推流
		 *
		 *
		 * @discuss  用户启动手动推流
		 *
		 **/
		virtual void startPublish() = 0;

		/*
		 * 停止推流
		 *
		 *
		 * @discuss 停止手动推流
		 *
		 **/
		virtual void stopPublish() = 0;

		/*
		 * 开始订阅流
		 *
		 * @param feedId 要订阅流的ID号
		 * @param IAudioFrameObserver  音频回调
		 * @param IVideoFrameObserver  视频回调
		 * @param IDataFrameObserver   文本数据回调
		 *
		 * @discuss 开始手动订阅流
		 *
		 **/
		virtual void subscribeStreaming(const char * feedId, IAudioFrameObserver *afo, IVideoFrameObserver *vfo, IDataFrameObserver *dfo) = 0;

		/*
		 * 停止订阅流
		 *
		 * @param feedId 要订阅流的ID号
		 *
		 * @discuss 停止手动订阅流
		 *
		 **/
		virtual void stopSubscribeStreaming(const char * feedId) = 0;

		/*
		 * 注册音频回调
		 *
		 *
		 * @param iAfo[], 音频回调的数组
		 * @param iAfoNum,  音频回调数量
		 *
		 * @discuss 注册用户自动拉流的音频回调函数
		 *
		 **/
		virtual void registerAudioFrameObserver(IAudioFrameObserver* iAfo[], int iAfoNum) = 0;

		/*
		 * 注册视频回调
		 *
		 *
		 * @param iVfo[], 视频回调的数组
		 * @param iVfoNum,  视频回调数量
		 *
		 * @discuss 注册用户自动拉流的视频回调函数
		 *
		 **/
		virtual void registerVideoFrameObserver(IVideoFrameObserver* iVfo[], int iVfoNum) = 0;

		/*
		 * 注册视频采集设备
		 *
		 *
		 * @param videoCd, 视频采集回调
		 *
		 * @discuss 注册视频采集回调函数
		 *
		 **/
		virtual void registerExternalVideoCapturer(std::shared_ptr<IVideoCapturerDevice> videoCd) = 0;

		/*
		 * 注册消息监听函数
		 *
		 *
		 * @param msgListener, 监听SDK 的消息
		 *
		 * @discuss 消息类型定义在BaiduRtcCommonDefine.h
		 *
		 **/
		virtual void registerRtcMessageListener(IRtcMessageListener* msgListener) = 0;

		/*
		 * 设置本地视频镜像模式
		 *
		 * @param isOpenMirror, 是否镜像
		 *
		 **/
		virtual void setLocalVideoMirror(bool isOpenMirror) = 0;

		/*
		 * 打开摄像头
		 *
		 * @param deviceName,摄像头名称
		 * @discuss  打开指定的摄像头
		 *
		 **/
		virtual void openCamera(char* deviceName) = 0;

		/*
		 * 关闭摄像头
		 *
		 * @discuss  关闭摄像头
		 *
		 **/
		virtual void closeCamera() = 0;

		/*
		 * 启动摄像头预览
		 *
		 * @discuss  启动摄像头预览
		 *
		 **/
		virtual void startPreview() = 0;

		/*
		 * 停止摄像头预览
		 *
		 * @discuss  停止摄像头预览
		 *
		 **/
		virtual void stopPreview() = 0;

		/*
		 * 静默摄像头
		 *
		 * @discuss  用户静默摄像头
		 *
		 **/
		virtual void muteCamera(bool isMuted) = 0;

		/*
		 * 静默麦克风
		 *
		 * @discuss  静默麦克风
		 *
		 **/
		virtual void muteMicphone(bool isMuted) = 0;

		/*
		 * 设置音量
		 *
		 * @param volume,设置音量 0 ~ 10
		 * @discuss  设置音量
		 *
		 **/
		virtual void setVolume(int volume) = 0;

		/*
		 * 设置直播推流地址
		 *
		 * @param url  推流地址
		 * @return true 成功，false 失败
		 * @discussion  设置一个可以使用的直播推流地址
		 *
		 **/
		virtual bool setLiveStreamingURL(const char *url) = 0;

		/*
		 * 设置直播推流角色
		 *
		 * @param role  推流角色
		 * @return true 成功，false 失败
		 * @discussion  设置直播推流的角色
		 *
		 **/
		virtual bool setLiveStreamingRole(const char *role) = 0;

		/*
		 * 设置直播混流
		 *
		 * @param mix   是否混流
		 * @return true 成功，false 失败
		 * @discussion  设置直播混流
		 *
		 **/
		virtual bool setLiveStreamingMix(bool mix) = 0;

		/*
		 * 设置录制
		 *
		 * @param rec   是否录制
		 * @return true 成功，false 失败
		 * @discussion   设置录制
		 *
		 **/
		virtual bool setRecording(bool rec) = 0;

		/*
		 * 设置UE4的地址，进行UE4直连
		 *
		 * @param ue4 UE4的服务器地址和端口号
		 * @return
		 * @discussion  设置UE4的地址，进行UE4直连
		 *
		 **/
		virtual void setUE4(const char *ue4) = 0;

		/*
		 * 设置UE4的DRC，动态分辨率控制策略
		 *
		 * @param ue4drc UE4的动态分辨率控制策略 eg: "0:1500:1080x1440:15,10:1000:600x800:15,20:500:600x800:15"
		 * 			     是一个5元组列表NACKs:kbps:WxH:fps
		 * @return
		 * @discussion 设置UE4的DRC，动态分辨率控制策略
		 *
		 **/
		virtual void setUE4DRC(const char *ue4drc) = 0;

		/*
		 * 设置AS的grpc服务，已经废弃
		 *
		 * @param as grpc服务器监听端口
		 * @return
		 * @discussion 设置AS的grpc服务，已经废弃
		 *
		 **/
		virtual void setAS(const char *as) = 0;

		/*
		 * 设置DHA的服务器地址，已经废弃
		 *
		 * @param dha 要连接的grpc服务器
		 * @return
		 * @discussion 设置DHA的服务器地址，已经废弃
		 *
		 **/
		virtual void setDHA(const char *dha) = 0;

		/*
		 * 设置输出的音频数据dump路径，调试用
		 *
		 * @param fileName 要保存的文件名
		 * @return
		 * @discussion 设置输出的音频数据dump路径，调试用
		 *
		 **/
		virtual void setDumpAudioOutput(const char *fileName) = 0;

		/*
		 * 设置输出的音频数据dump路径，调试用
		 *
		 * @param fileName 要保存的文件名
		 * @return
		 * @discussion 设置输出的音频数据dump路径，调试用
		 *
		 **/
		virtual void setDumpVideoOutput(const char *fileName) = 0;

		/*
		 * 通过RTCPlatform查询获得RTC媒体服务器地址
		 *
		 * @param RtcPlatform Platform的查询URL，格式为: http://10.99.200.227:8910/internal/v1/nodeapply?appId=xxx&roomName=yyyy
		 * @param defaultRtcServer 默认的RTC媒体服务器地址
		 * @return  得到的RTC媒体服务器地址
		 * @discussion 通过RTCPlatform查询获得RTC媒体服务器地址
		 *
		 **/
		virtual const char* getRtcServerFromPlatform(const char *RtcPlatform,const char *defaultRtcServer) = 0;

		/*
		 * 启动健康服务功能
		 *
		 * @param port 健康服务的监听端口号
		 * @return
		 * @discussion 启动健康服务功能
		 *
		 **/
		virtual void startHealthyServer(const char *port) = 0;

		/*
		 * 停止健康服务功能
		 *
		 * @return
		 * @discussion 停止健康服务功能
		 *
		 **/
		virtual void stopHealthyServer() = 0;

		/*
		 * 获得房间状态信息
		 *
		 * @param
		 * @discuss 获得房间状态信息
		 *
		 **/
		virtual void getRoomStates(RtcRoomUserInfo** userInfoList, int& userNumber) = 0;

		/*
		 * 获得网络连接信息
		 *
		 * @param ConnectionType,
		 * @return ConnectionStats,
		 * @discuss 获得网络连接信息
		 *
		 **/
		virtual ConnectionStats getConnectionStats(ConnectionType connectionType) = 0;

		/*
		 * 注册文本数据回调
		 *
		 *
		 * @param iDfo[], 文本数据回调的数组
		 * @param iDfoNum,  文本数据回调数量
		 *
		 * @discuss 注册用户自动拉流的文本回调函数
		 *
		 **/
		virtual void registerDataFrameObserver(IDataFrameObserver* iDfo[], int iDfoNum) = 0;

		/*
		 * 设置候选IP，即UDP代理IP地址
		 *
		 * @param candidateIP, 候选IP
		 *
		 * @discuss 设置候选IP，即UDP代理IP地址
		 *
		 **/
		virtual void setCandidateIP(const char *candidateIP) = 0;

		/*
		 * 选择媒体服务器地址，即接收UDP数据的媒体服务器地址
		 *
		 * @param mediaserverIP, 媒体服务器地址IP，即代理指向的地址
		 *
		 * @discuss 主动选择媒体服务器地址，即接收UDP数据的媒体服务器地址
		 *
		 **/
		virtual void setMediaServerIP(const char *mediaserverIP) = 0;

		/*
		 * 发送消息给特定id用户
		 *
		 * @param msg, 发送的消息内容
		 *
		 * @param id, 用户ID的字符串
		 *
		 * @discuss 当id为"0"时， 表示广播消息
		 *
		 **/
		virtual void sendMessageToUser(const char *msg, const char * id) = 0;

		/*
		 * 设置用户属性
		 *
		 * @param a, 用户自定义属性
		 *
		 * @discuss 设置用户自定义属性
		 *
		 **/
		virtual void setUserAttribute(const char * a) = 0;

		/*
		 * 获得用户属性
		 *
		 * @param id, 用户的id号
		 *
		 * @discuss 获得用户的属性, 结果以事件通知返回
		 *
		 **/
		virtual void getUserAttribute(const char * id) = 0;

		/*
		 * 设置房间视频编码器类型
		 *
		 * @param vc, 视频编码器, 可取值为: "h264", "vp8", "vp9", "h263", "av1", "h265", "h266"
		 *
		 * @discuss 设置房间中使用的视频编码器, 默认值是"h264", 房间级别的配置, 房间中每个客户端的视频编码器需要一致.
		 *          需要在login之前调用.
		 **/
		virtual void setVideoCodec(const char * vc) = 0;

        /*
        * 把用户禁言或解除禁言
        *
        * @param userId, 用户userId号.
        * @param shutup, 禁言标记, true为禁言, false为解除禁言.
        *
        * @discuss 把用户禁言或解除禁言
        *
        **/
        virtual void shutupUser(const char * userId, bool shutup) = 0;

        /*
        * 把用户踢出房间
        *
        * @param userId, 用户userId号.
        *
        * @discuss 以用户的userid号把用户踢出房间.
        *
        **/
        virtual void kickoutUser(const char * userId) = 0;

        /*
        * 解散房间
        *
        *
        * @discuss 解散已经登录的房间, 房间中的所有用户都会被强制退出.
        *
        **/
        virtual void disbandRoom() = 0;

        /*
        * 设置房间音频编码器类型
        *
        * @param ac, 音频编码器, 可取值为: "opus", "pcmu", "pcma"
        *
        * @discuss 设置房间中使用的音频编码器, 默认值是"opus", 房间级别的配置, 房间中每个客户端的音频编码器需要一致.
        *          需要在login之前调用.
        **/
        virtual void setAudioCodec(const char * ac) = 0;
	};

	BAIDU_RTC_EXPORT_API BaiduRtcRoomClient* createBaiduRtcRoomClient();

	/*
	 * 获得SDK的版本号
	 *
	 * @return  版本号字符串， 比如: 1.1.0
	 * @discuss 获得百度RTC的版本号
	 *
	 **/
	extern "C" const char* getBaiduRtcSdkVersion();

	/*
	 * 打开log
	 *
	 * @param, isEnable, 为true,   打开log
	 *                   为false， 关闭log
	 *
	 * @discuss SDK log 选项
	 *
	 **/
	BAIDU_RTC_EXPORT_API void enableBaiduRtcLog(bool isEnable);

	/*
	 * 获得视频设备列表
	 *
	 * @param deviceCount, 数量
	 * @return RtcVideoDeviceInfo*, ָ视频设备信息
	 * @discuss 获得视频设备列表
	 *
	 **/
	BAIDU_RTC_EXPORT_API RtcVideoDeviceInfo* getVideoDeviceList(int & deviceCount);

	/*
	 * 释放视频设备列表
	 *
	 * @param deviceInfoList 视频设备列表
	 * @discuss  释放由getVideoDeviceList得到的列表
	 *
	 **/
	BAIDU_RTC_EXPORT_API void freeVideoDeviceList(RtcVideoDeviceInfo* deviceInfoList);

	/*
	 * 设置视频设备
	 *
	 * @param deviceId,视频设备名称
	 * @discuss getVideoDeviceList中的值
	 *
	 **/
	BAIDU_RTC_EXPORT_API void setVideoDevice(const char* deviceId);

}//namespace baidurtc

