//
// Created by user on 12/13/23.
//
#include "my_binary_utils.h"
#include "my_blind_watermark_core0.h"
using namespace my_lib;

void maintest() {
  auto img_path = "/Users/user/Downloads/ori_img.jpeg";
  cv::Mat img = cv::imread(img_path);
  auto wm_bits = BinaryUtils::asciiStrToBinArray("hello world");
  auto wm_bits2 = std::vector<uint8_t>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  auto xx = cv::Mat(wm_bits2);
//  std::cout << xx << std::endl;
  //  std::cout << wm_bits.size() << std::endl;

  auto core = std::make_unique<BindWatermarkCoreV0>();
  core->readImage(img);
  core->readWatermark(wm_bits2);
  auto embed_img = core->embed();
  std::cout << "embed_img size: " << embed_img.size() << std::endl;

  cv::imwrite("embed_img.png", embed_img);

  std::cout << "-------------------------------------\n";
  cv::Mat emb_img = cv::imread("embed_img.png");
  core->extract(emb_img, wm_bits.size());
}

void yuvtest() {
  auto img_path = "/Users/user/Downloads/Lenna_(test_image).png";
  cv::Mat img = cv::imread(img_path);

  cv::Mat yuv;
  cv::cvtColor(img, yuv, cv::COLOR_BGR2YUV);

  auto yuv_channels = std::vector<cv::Mat>();
  cv::split(yuv, yuv_channels);
  auto y = yuv_channels[0];
  auto u = yuv_channels[1];
  auto v = yuv_channels[2];
  std::cout << y.rowRange(0, 5).colRange(0, 5) << std::endl;
  std::cout << u.rowRange(0, 5).colRange(0, 5) << std::endl;
  std::cout << v.rowRange(0, 5).colRange(0, 5) << std::endl;

  cv::Mat temp;
  cv::cvtColor(yuv, temp, cv::COLOR_YUV2BGR);

  cv::imwrite("yuv.png", temp);
}

int main() {
//  yuvtest();
  maintest();
  return 0;
}
