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



class IVideoFrameObserver {
public:
	/*
	 * 视频回调接口
	 *
	 * @param feedid  视频源的id值
	 * @param img 视频数据地址
	 * @param len 长度值
	 * @param imgtype 视频数据类型
	 * @param width 视频的宽度
	 * @param height 视频的高度
	 *
	 * @discuss 视频回调接口
	 *
	 **/
	virtual void onFrame(int64_t feedid, const char *img, int len, RtcImageType imgtype, int width, int height) = 0;
};