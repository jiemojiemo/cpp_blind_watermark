//
// Created by user on 12/16/23.
//

#ifndef BLINDWATERMARKTEST_MY_I_BLIND_WATERMARK_CORE_H
#define BLINDWATERMARKTEST_MY_I_BLIND_WATERMARK_CORE_H
#include <opencv4/opencv2/opencv.hpp>
#include <vector>
namespace my_lib
{
class IBindWatermarkCore {
public:
  virtual ~IBindWatermarkCore() = default;

  virtual void readImage(const cv::Mat &img) = 0;
  virtual void readWatermark(const std::vector<uint8_t> &wm_bits) = 0;
  virtual cv::Mat embed() = 0;
  virtual std::vector<uint8_t> extract(const cv::Mat& img, int wm_size) = 0;
};
}

#endif // BLINDWATERMARKTEST_MY_I_BLIND_WATERMARK_CORE_H
