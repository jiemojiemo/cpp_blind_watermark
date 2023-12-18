//
// Created by user on 12/16/23.
//

#include <gmock/gmock.h>
#include "my_blind_watermark_core0.h"

using namespace testing;
class ABlindWatermarkCore : public Test {
public:
  cv::Mat createARGBImageArray(size_t rows, size_t cols) {
    cv::Mat img = cv::Mat::ones(rows, cols, CV_8UC3);
    cv::randu(img, cv::Scalar(0, 0, 0),
              cv::Scalar(256, 256, 256)); // 生成[0, 256)范围内的随机数
    return img;
  }

  size_t img_rows = 256;
  size_t img_cols = 256;
};

TEST_F(ABlindWatermarkCore, CanReadARGBImageArray) {
  cv::Mat img = createARGBImageArray(img_rows, img_cols);

  auto core = std::make_unique<my_lib::BindWatermarkCoreV0>();
  core->readImage(img);

  ASSERT_THAT(core->getImageArray().channels(), Eq(3));
  ASSERT_THAT(core->getImageArray().rows, Eq(img_rows));
  ASSERT_THAT(core->getImageArray().cols, Eq(img_cols));
}

TEST_F(ABlindWatermarkCore, CanEmbed)
{
  cv::Mat img = createARGBImageArray(img_rows, img_cols);
  auto wm_bits = std::vector<uint8_t>{1, 0, 1, 0, 1, 0, 1, 0};

  auto core = std::make_unique<my_lib::BindWatermarkCoreV0>();
  core->readImage(img);
  core->readWatermark(wm_bits);
  auto embed_img = core->embed();
}

TEST_F(ABlindWatermarkCore, XXX)
{
  cv::Mat img = createARGBImageArray(4, 4);
  std::vector<cv::Mat> yuv_channels;
  cv::split(img, yuv_channels);

  cv::Mat temp = cv::Mat::ones(4, 4, CV_8UC1);
  temp.copyTo(yuv_channels[0]);
  for(const auto& c : yuv_channels){
    std::cout << c << std::endl;
  }

  std::cout << img << std::endl;
}