//
// Created by user on 12/15/23.
//
#include <gmock/gmock.h>
#include "my_dtw2_utils.h"

using namespace testing;
using namespace my_lib;
class AWaveletUtils : public Test {
public:
  DTW2Utils u;
};

TEST_F(AWaveletUtils, CanDoDWT2) {
  cv::Mat input = cv::Mat::ones(4, 4, CV_64F);
  u.preapre(input.rows, input.cols);

  auto ret = u.dwt2(input);
  ASSERT_THAT(ret, Eq(0));

  cv::Mat output;
  ret = u.idwt2(u.getCoeffs(), output);
  ASSERT_THAT(ret, Eq(0));
}

TEST_F(AWaveletUtils, dtw2FailedIfNeverPrepare) {
  auto ret = u.dwt2(cv::Mat::ones(4, 4, CV_64F));
  ASSERT_THAT(ret, Eq(-1));
}

TEST_F(AWaveletUtils, dtw2FailedIfInputSizeNotEven) {
  cv::Mat input = cv::Mat::ones(3, 3, CV_64F);
  u.preapre(input.rows, input.cols);

  auto ret = u.dwt2(input);
  ASSERT_THAT(ret, Eq(-1));
}