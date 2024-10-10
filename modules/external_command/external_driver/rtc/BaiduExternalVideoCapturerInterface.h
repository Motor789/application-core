/***************************************************************************
 *
 * Copyright (c) 2020 Baidu.com, Inc. All Rights Reserved
 *
 * @author: shaohongsheng@baidu.com
 *
 ***************************************************************************/

#pragma once
#include <stdint.h>
#include <memory>
#include "BaiduRtcCommonDefine.h"

/*
 * 视频外部采集数据监听接口
 *
 * @discuss 该接口无需用户实现，sdk 内部实现，并通过IVideoCapturerDevice::allocateAndStart 传给用户。
 *          用户通过 onIncomingCapturedData 接口把数据 push 给sdk;
 *
 **/
class BAIDU_RTC_EXPORT_API IVideoCapturerDataObserver {
public:
	/*
	 * sdk 接收外部采集视频数据接口
	 *
	 * @param data, 外部视频数据buffer;
	 * @param widht, 视频数据宽；
	 * @param height, 视频数据高；
	 * @param format, 视频数据颜色空间，目前支持ARGB/YUV420(I420)两种格式的转换；
	 *
	 * @discuss 外部数据接收接口，在注册外部采集设备后，IVideoCapturerDevice::allocateAndStart 接口会传递
	 *          IVideoCapturerDataObserver 接口的实现给用户。
	 *
	 **/
	virtual void onIncomingCapturedData(uint8_t* data, int width, int height, VideoColorFormat format) = 0;

	virtual ~IVideoCapturerDataObserver() {}
};


/*
 * 外部采集设备接口
 *
 * @discuss 若使用外部采集，该接口需要用户实现，并通过注册函数接口注册给SDK
 *
 **/
class BAIDU_RTC_EXPORT_API IVideoCapturerDevice {
public:
	/*
	 * 启动外部采集设备
	 *
	 * @param vcdo, 外部数据采集数据监听者
	 * 
	 * @discuss  vcdo由sdk内部实现并通过allocateAndStart 函数传递给用户，用户需要保存该值，
	             外部采集的视频数据是通过vcdo->onIncomingCapturedData 传递给sdk的。
	 **/
	virtual void allocateAndStart(std::unique_ptr<IVideoCapturerDataObserver> vcdo) = 0;


	/*
	 * stop外部采集设备
	 *
	 * @discuss 停止外部采集视频数据的输入。
	 *
	 **/
	virtual void stopAndDeallocate() = 0;

	virtual ~IVideoCapturerDevice() {}
};
