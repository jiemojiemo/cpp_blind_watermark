//
// Created by user on 12/13/23.
//
#include "binary_utils.h"
#include "blind_watermark_core0.h"
using namespace re;

void maintest() {
  auto img_path = "/Users/user/Downloads/ori_img.jpeg";
  cv::Mat img = cv::imread(img_path);
  auto wm_bits = BinaryUtils::utf8StrToBinArray("jiemo");
  auto xx = cv::Mat(wm_bits);
  std::cout << "wm_bits size: " << wm_bits.size() << std::endl;

  auto core = std::make_unique<BindWatermarkCoreV0>();
  core->readImage(img);
  core->readWatermark(wm_bits);
  auto embed_img = core->embed();
  std::cout << "embed_img size: " << embed_img.size() << std::endl;

  cv::imwrite("embed_img.png", embed_img);

  std::cout << "-------------------------------------\n";
  cv::Mat emb_img = cv::imread("embed_img.png");
  auto extract_bits = core->extract(emb_img, wm_bits.size());
  auto extract_str = BinaryUtils::binArrayToUtf8Str(extract_bits);
  std::cout << "extract_str: " << extract_str << std::endl;
}

int main() {
  maintest();
  return 0;
}
