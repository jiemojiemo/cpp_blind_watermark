//
// Created by user on 12/13/23.
//
#include "my_binary_utils.h"
#include "wavelib.h"

#include <opencv2/opencv.hpp>
#include <utility>

class IBindWatermarkCore {
public:
  virtual ~IBindWatermarkCore() = default;

  virtual void readImage(const cv::Mat &img) = 0;
  virtual void readWatermark(const std::vector<uint8_t> &wm_bits) = 0;
  virtual cv::Mat embed() = 0;
  virtual std::vector<uint8_t> extract() = 0;
};

class BindWatermarkCoreV0 : public IBindWatermarkCore {
public:
  BindWatermarkCoreV0(cv::Mat block_shape = cv::Mat(4, 4, CV_32SC1))
      : m_block_shape(std::move(block_shape)) {}
  ~BindWatermarkCoreV0() override = default;
  void readImage(const cv::Mat &img) override {
    readImageWithAlpha(img);
    cv::cvtColor(m_img, m_imgYUV, cv::COLOR_BGR2YUV);
  }
  void readWatermark(const std::vector<uint8_t> &wm_bits) override {
    m_wm_bits = wm_bits;
    m_wm_size = wm_bits.size();
  }
  cv::Mat embed() override { return {}; }
  std::vector<uint8_t> extract() override { return std::vector<uint8_t>(); }

private:
  void readImageWithAlpha(const cv::Mat& img){
    if (img.channels() == 4) {
      cv::cvtColor(img, m_img, cv::COLOR_BGRA2BGR);
    } else {
      m_img = img.clone();
    }

    m_img_width = m_img.cols;
    m_img_height = m_img.rows;
  }

  cv::Mat m_block_shape{4, 4, CV_32SC1};
  int d1 = 36;
  std::vector<uint8_t> m_wm_bits;
  size_t m_wm_size;

  cv::Mat m_img;
  cv::Mat m_imgYUV;
  size_t m_img_width;
  size_t m_img_height;
};

//int main() {
//  auto img_path = "/Users/user/Downloads/Lenna_(test_image).png";
//  cv::Mat img = cv::imread(img_path);
//
//  auto core = std::make_unique<BindWatermarkCoreV0>();
//  core->readImage(img);
//
//  return 0;
//}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "../header/wavelib.h"

double absmax(double *array, int N) {
  double max;
  int i;

  max = 0.0;
  for (i = 0; i < N; ++i) {
    if (fabs(array[i]) >= max) {
      max = fabs(array[i]);
    }
  }

  return max;
}

double generate_rnd() {
  double rnd;

  rnd = (double) (rand() % 100 + 1);

  return rnd;
}

void wavelib_test()
{
  wave_object obj;
  wt2_object wt;
  int i, k, J, rows, cols,N;
  double *inp, *wavecoeffs,*oup,*diff;
  double *cLL;
  int ir, ic;
  double amax;

  rows = 32;
  cols = 30;
  N = rows*cols;

  char *name = "haar";
  obj = wave_init(name);// Initialize the wavelet
  srand(0);
  inp = (double*)calloc(N, sizeof(double));
  oup = (double*)calloc(N, sizeof(double));
  diff = (double*)calloc(N, sizeof(double));

  J = 1;

  wt = wt2_init(obj, "dwt", rows,cols, J);

  for (i = 0; i < rows; ++i) {
    for (k = 0; k < cols; ++k) {
      //inp[i*cols + k] = i*cols + k;
      inp[i*cols + k] = generate_rnd();
      oup[i*cols + k] = 0.0;
    }
  }

  wavecoeffs = dwt2(wt, inp);

  cLL = getWT2Coeffs(wt, wavecoeffs, 1, "A", &ir, &ic);

  dispWT2Coeffs(cLL, ir, ic);
//
//  idwt2(wt, wavecoeffs, oup);
//
//  for (i = 0; i < rows*cols; ++i) {
//    diff[i] = oup[i] - inp[i];
//  }
//
//  amax = absmax(diff, rows*cols);
//
//  wt2_summary(wt);
//
//  printf("Abs Max %g \n", amax);

  wave_free(obj);
  wt2_free(wt);
  free(inp);
  free(wavecoeffs);
  free(oup);
  free(diff);
  return;
}

void wavelib_test2()
{
  wave_object obj;
  wt2_object wt;
  int i, k, J, rows, cols,N;
  double *wavecoeffs;
  double *cLL;
  int ir, ic;
  double amax;

  rows = 4;
  cols = 4;
  N = rows*cols;

  const char *name = "haar";
  obj = wave_init(name);// Initialize the wavelet
  srand(0);
  auto inp = std::vector<double>(N);
  auto oup = std::vector<double>(N);
  auto diff = std::vector<double>(N);

  J = 1;

  wt = wt2_init(obj, "dwt", rows,cols, J);

  for (i = 0; i < rows; ++i) {
    for (k = 0; k < cols; ++k) {
      //inp[i*cols + k] = i*cols + k;
      std::cout << i*cols + k << std::endl;
      inp[i*cols + k] = 1;
      oup[i*cols + k] = 0.0;
    }
  }

  wavecoeffs = dwt2(wt, inp.data());

  cLL = getWT2Coeffs(wt, wavecoeffs, 1, "A", &ir, &ic);

  dispWT2Coeffs(cLL, ir, ic);

//  idwt2(wt, wavecoeffs, oup.data());
//
//  for (i = 0; i < rows*cols; ++i) {
//    diff[i] = oup[i] - inp[i];
//  }
//
//  amax = absmax(diff.data(), rows*cols);
//
//  wt2_summary(wt);
//
//  printf("Abs Max %g \n", amax);

  wave_free(obj);
  wt2_free(wt);
  free(wavecoeffs);
  return;
}

int main() {
//  wavelib_test();
//  std::cout << "------------------" << std::endl;
  wavelib_test2();
}
