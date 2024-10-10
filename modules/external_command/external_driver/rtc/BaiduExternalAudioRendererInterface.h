/***************************************************************************
 *
 * Copyright (c) 2020 Baidu.com, Inc. All Rights Reserved
 *
 * @author: shaohongsheng@baidu.com
 *
 ***************************************************************************/

#pragma once
#include <stdint.h>
#include "BaiduRtcCommonDefine.h"


class BAIDU_RTC_EXPORT_API IAudioFrameObserver {
public:
	/*
	 * 音频数据回调
	 *
	 * @param feedid  音频源的id值
	 * @param audio 音频PCM数据
	 * @param len 音频数据长度值
	 * @param samlplerate 音频采样率
	 * @param channels 音频通道数
	 * 
	 * @discuss 音频数据回调
	 *
	 **/
	virtual void onAudioData(int64_t feedid, const char *audio, int len, int samlplerate, int channels) = 0;
};