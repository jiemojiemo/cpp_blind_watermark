//
// Created by user on 12/16/23.
//

#ifndef BLINDWATERMARKTEST_DTW2_UTILS_H
#define BLINDWATERMARKTEST_DTW2_UTILS_H
#include "wavelib.h"
#include <opencv4/opencv2/opencv.hpp>

namespace re {
class DTW2Utils {
public:
  DTW2Utils() : obj_(nullptr, ::wave_free), wt_(nullptr, ::wt2_free) {}
  ~DTW2Utils() { free(coeffs_); }
  void preapre(size_t rows, size_t cols) {
    std::unique_ptr<::wave_set, decltype(&::wave_free)> obj(::wave_init("haar"),
                                                            ::wave_free);
    std::unique_ptr<::wt2_set, decltype(&::wt2_free)> wt(
        ::wt2_init(obj.get(), "dwt", rows, cols, J), ::wt2_free);

    obj_ = std::move(obj);
    wt_ = std::move(wt);
  }

  int dwt2(const cv::Mat &input) {
    if (input.type() != CV_64F) {
      printf("input type is not CV_64F\n");
      return -1;
    }

    if (input.rows % 2 != 0 || input.cols % 2 != 0) {
      printf("input size is not even\n");
      return -1;
    }

    if (wt_ == nullptr || obj_ == nullptr) {
      printf("wt_ or obj_ is not initialized\n");
      return -1;
    }

    auto *input_data_ptr = reinterpret_cast<double *>(input.data);
    coeffs_ = ::dwt2(wt_.get(), input_data_ptr);

    return 0;
  }

  int idwt2(double *coeffs, cv::Mat &output) {
    if (wt_ == nullptr || obj_ == nullptr) {
      printf("wt_ or obj_ is not initialized\n");
      return -1;
    }

    output = cv::Mat::zeros(wt_->rows, wt_->cols, CV_64F);

    auto *output_data_ptr = reinterpret_cast<double *>(output.data);
    ::idwt2(wt_.get(), coeffs, output_data_ptr);

    return 0;
  }

  double *getCoeffs() { return coeffs_; }

  int getCaRef(cv::Mat &cA) {
    getWT2Coeffs(cA, "A");
    return 0;
  }

  int getChRef(cv::Mat &cH) {
    getWT2Coeffs(cH, "H");
    return 0;
  }

  int getCvRef(cv::Mat &cV) {
    getWT2Coeffs(cV, "V");
    return 0;
  }

  int getCdRef(cv::Mat &cD) {
    getWT2Coeffs(cD, "D");
    return 0;
  }

private:
  // no copy
  void getWT2Coeffs(cv::Mat &output, char *type) {
    int rows, cols;
    auto *ca_coeff = ::getWT2Coeffs(wt_.get(), coeffs_, 1, type, &rows, &cols);
    output = cv::Mat(rows, cols, CV_64F, ca_coeff);
  }

  constexpr static int J = 1;
  std::unique_ptr<::wave_set, decltype(&::wave_free)> obj_;
  std::unique_ptr<::wt2_set, decltype(&::wt2_free)> wt_;
  double *coeffs_{nullptr};
};
} // namespace my_lib


#endif // BLINDWATERMARKTEST_DTW2_UTILS_H
