//
// Created by user on 12/16/23.
//

#ifndef BLINDWATERMARKTEST_MY_BLIND_WATERMARK_CORE0_H
#define BLINDWATERMARKTEST_MY_BLIND_WATERMARK_CORE0_H
#include "my_dtw2_utils.h"
#include "my_i_blind_watermark_core.h"
namespace my_lib {
class BindWatermarkCoreV0 : public IBindWatermarkCore {
public:
  BindWatermarkCoreV0(std::array<int, 2> block_shape = {4, 4})
      : m_block_shape(std::move(block_shape)) {}
  ~BindWatermarkCoreV0() override = default;
  void readImage(const cv::Mat &img) override {
    readImageWithAlpha(img);
    preprocessForYUV();
    dwt2Process();
  }
  void readWatermark(const std::vector<uint8_t> &wm_bits) override {
    m_wm_bits = wm_bits;
    m_wm_size = wm_bits.size();
  }
  cv::Mat embed() override {
    auto block_num = ca_block_shape[0] * ca_block_shape[1];
    if (m_wm_size > block_num) {
      printf("水印数据太大，嵌入失败，最多可嵌入 %d kb，水印数据大小 %d kb",
             block_num / 1000, m_wm_size / 1000);
      return {};
    }

    std::vector<cv::Mat> yuv_channels;
    cv::split(m_imgYUV, yuv_channels);
    for (int c = 0; c < m_imgYUV.channels(); c++) {
      auto &ca_channel = ca[c];
      auto block_index = 0;

      // 遍历每个 ca block
      for (int x = 0; x < ca_block_shape[0];++x) {
        for (int y = 0; y < ca_block_shape[1]; ++y) {
          auto x_inx = x * m_block_shape[0];
          auto y_inx = y * m_block_shape[1];
          auto rect = cv::Rect(x_inx, y_inx, m_block_shape[0], m_block_shape[1]);
          auto ca_block = ca_channel(rect);
          auto add_wm_block = blockAddWm(ca_block, block_index++);
          if(x == 220 && y == 24 && c == 2){
            blockAddWm(ca_block, block_index);
          }
          add_wm_block.copyTo(ca_channel(rect));
        }
      }

      std::cout << "embed block_index:" << block_index << std::endl;

      cv::Mat idwt2_result;
      dwt2UtilsArray[c]->idwt2(dwt2UtilsArray[c]->getCoeffs(), idwt2_result);
      idwt2_result.copyTo(yuv_channels[c]);
    }
    cv::Mat embed_YUV;
    cv::merge(yuv_channels, embed_YUV);

    //    printType(embed_YUV);

    cv::Mat embed_img;
    embed_YUV.convertTo(embed_YUV, CV_8UC3);
    //    printYUV(embed_YUV, "embed_YUV");
    cv::cvtColor(embed_YUV, embed_img, cv::COLOR_YUV2BGR);
    //    printYUV(embed_img, "embed_img_bgr");

    return embed_img;
  }

  std::vector<uint8_t> extract(const cv::Mat &img, int wm_size) override {
    cv::Mat wm_block_bits = extractRaw(img, wm_size);
    std::cout << "wm_block_bits:\n" << wm_block_bits << std::endl;
    cv::Mat wm_block_avg = extract_avg(wm_block_bits, wm_size);
    std::cout << "wm_block_avg:\n" << wm_block_avg << std::endl;
    return std::vector<uint8_t>();
  }

  const cv::Mat &getImageArray() const { return m_img; }
  const cv::Mat &getImageYUVArray() const { return m_imgYUV; }

private:
  void printYUV(cv::Mat &yuv_img, const std::string &prefix) {
    std::vector<cv::Mat> yuv_channels;
    cv::split(yuv_img, yuv_channels);
    for (auto &channel : yuv_channels) {
      std::cout << prefix + "yuv:\n"
                << channel.rowRange(0, 5).colRange(0, 5) << std::endl;
    }
  }
  void printType(cv::Mat &m) {
    if (m.type() == CV_8UC3) {
      std::cout << "The matrix is a 3-channel image (CV_8UC3)" << std::endl;
    } else if (m.type() == CV_8UC1) {
      std::cout << "The matrix is a single-channel image (CV_8UC1)"
                << std::endl;
    } else {
      int depth = CV_MAT_DEPTH(m.type());
      int channels = CV_MAT_CN(m.type());
      std::cout << "The matrix has depth " << depth << " and " << channels
                << " channels" << std::endl;
    }
  }
  cv::Mat extract_avg(const cv::Mat &wm_block_bit, int wm_size) {
    cv::Mat wm_avg = cv::Mat::zeros(wm_size, 1, CV_32F);
    for (int i = 0; i < wm_size; i++) {
      cv::Mat sub_mat;
      for (int j = i; j < wm_block_bit.cols; j += wm_size) {
        sub_mat.push_back(wm_block_bit.col(j));
      }
      cv::Scalar avg_scalar = cv::mean(sub_mat);
      wm_avg.at<double>(i, 0) = avg_scalar[0];
    }
    return wm_avg;
  }

  cv::Mat extractRaw(const cv::Mat &img, int wm_size) {
    readImage(img);
    auto block_num = ca_block_shape[0] * ca_block_shape[1];
    cv::Mat wm_block_bits =
        cv::Mat::zeros(m_imgYUV.channels(), block_num, CV_8U);

    for (int c = 0; c < m_imgYUV.channels(); c++) {
      auto &ca_channel = ca[c];
      auto block_index = 0;
      // 遍历每个 ca block
      for (int x = 0; x < ca_block_shape[0];++x) {
        for (int y = 0; y < ca_block_shape[1]; ++y) {
          auto x_inx = x * m_block_shape[0];
          auto y_inx = y * m_block_shape[1];
          auto rect = cv::Rect(x_inx, y_inx, m_block_shape[0], m_block_shape[1]);
          auto ca_block = ca_channel(rect);
          auto block_bit = blockGetWm(ca_block);
          if(block_bit == 0){
            std::cout << "x, y, rect: " << x << ", " << y << ", " << rect << std::endl;
            blockGetWm(ca_block);
          }
          wm_block_bits.at<uint8_t>(c, block_index++) = block_bit;
        }
      }
      std::cout << "block_index:" << block_index << std::endl;
    }

    return wm_block_bits;
  }
  cv::Mat blockAddWm(const cv::Mat &block, int block_index) {
    auto b = m_wm_bits[block_index % m_wm_size];
    if (block_index < 10) {
      //      printf("xx %d %d\n", block_index, b);
    }
    cv::Mat dct_block;
    cv::dct(block, dct_block);

    cv::Mat u, s, vt;
    cv::SVD::compute(dct_block, s, u, vt); // 计算SVD

    auto s0 = s.at<double>(0, 0);
    auto s00 = (std::floor(s0 / d1) + 0.25 + 0.5 * b) * d1;
    s.at<double>(0, 0) = s00;


    cv::Mat diag_s = cv::Mat::diag(s); // 创建对角矩阵
    cv::Mat result = u * diag_s * vt;  // 矩阵乘法

    cv::Mat idct_result;
    cv::idct(result, idct_result); // 逆DCT变换
    return idct_result;
  }

  uint8_t blockGetWm(const cv::Mat &block) {
    cv::Mat dct_block, u, s, v;
    cv::dct(block, dct_block);            // 对块进行 DCT 变换
    cv::SVD::compute(dct_block, s, u, v); // 对 DCT 变换后的块进行 SVD 分解
    bool bb = false;
    if (bb) {
      std::cout << block << std::endl;
    }
    auto t0 = s.at<double>(0, 0);
    double s0_mod_d1 = std::fmod(t0, d1);
    uint8_t wm = (s0_mod_d1 > d1 / 2) ? 1 : 0;
    return wm;
  }

  void preprocessForYUV() {
    cv::cvtColor(m_img, m_imgYUV, cv::COLOR_BGR2YUV);

    // 将 YUV 大小填充至偶数
    int top = 0;
    int bottom = m_img.rows % 2;
    int left = 0;
    int right = m_img.cols % 2;

    cv::copyMakeBorder(m_imgYUV, m_imgYUV, top, bottom, left, right,
                       cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));

    m_imgYUV.convertTo(m_imgYUV, CV_64F);
    //    printYUV(m_imgYUV, "yuv no change:");
  }
  void dwt2Process() {
    auto ca_shape_rows = m_imgYUV.rows / 2;
    auto ca_shape_cols = m_imgYUV.cols / 2;
    ca_block_shape = cv::Vec4i{ca_shape_rows / m_block_shape[0],
                               ca_shape_cols / m_block_shape[1],
                               m_block_shape[0], m_block_shape[1]};

    std::vector<cv::Mat> channels;
    cv::split(m_imgYUV, channels);

    ca.resize(m_imgYUV.channels());
    ch.resize(m_imgYUV.channels());
    cv.resize(m_imgYUV.channels());
    cd.resize(m_imgYUV.channels());

    dwt2UtilsArray.resize(m_imgYUV.channels());
    for (int c = 0; c < m_imgYUV.channels(); c++) {
      // 每个 channel 都创建一个 dwt2Utils
      // 每个 dwt2Utils 存放着 coeffs
      // 后续所有 mat 都是 coeffs 的引用
      dwt2UtilsArray[c] = std::make_unique<DTW2Utils>();
      dwt2UtilsArray[c]->preapre(m_imgYUV.rows, m_imgYUV.cols);

      dwt2UtilsArray[c]->dwt2(channels[c]);
      //      std::cout << "yuv:\n" << channels[c].rowRange(0, 5).colRange(0, 5)
      //      << std::endl;

      dwt2UtilsArray[c]->getCaRef(ca[c]);
      std::cout << "ca:\n" << ca[c].rowRange(0, 5).colRange(0, 5) << std::endl;
      printType(ca[c]);
      dwt2UtilsArray[c]->getChRef(ch[c]);
      dwt2UtilsArray[c]->getCvRef(cv[c]);
      dwt2UtilsArray[c]->getCdRef(cd[c]);
    }
  }

  void readImageWithAlpha(const cv::Mat &img) {
    if (img.channels() == 4) {
      cv::cvtColor(img, m_img, cv::COLOR_BGRA2BGR);
    } else {
      m_img = img.clone();
    }

    m_img_width = m_img.cols;
    m_img_height = m_img.rows;
  }

  std::vector<std::unique_ptr<DTW2Utils>> dwt2UtilsArray;

  std::array<int, 2> m_block_shape;
  int d1 = 36;
  std::vector<uint8_t> m_wm_bits;
  size_t m_wm_size;

  cv::Mat m_img;
  cv::Mat m_imgYUV;
  size_t m_img_width;
  size_t m_img_height;

  cv::Vec4i ca_block_shape;
  std::vector<cv::Mat> ca;
  std::vector<cv::Mat> ch;
  std::vector<cv::Mat> cv;
  std::vector<cv::Mat> cd;
};
} // namespace my_lib

#endif // BLINDWATERMARKTEST_MY_BLIND_WATERMARK_CORE0_H
