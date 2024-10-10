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



class IDataFrameObserver {
public:
	/*
	 * 文本数据回调接口
	 * 
	 * @param feedid  数据源的id值
	 * @param data 数据地址
	 * @param len 数据长度值
	 *
	 * @discuss 文本数据回调接口
	 *
	 **/
	virtual void onTextData(int64_t feedid, const char *data, int len) = 0;
};