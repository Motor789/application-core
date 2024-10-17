#pragma once
namespace apollo{
namespace drivers{
namespace camera{

class YUYV2RGB{
	public:
		void yuyv2rgb_cuda(char *YUV, char *RGB, int num_blocks, int block_size);
	};






}
}
}