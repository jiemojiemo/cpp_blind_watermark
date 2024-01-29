//
// Created by user on 12/14/23.
//

#include "base64codec_utils.h"
#include <gmock/gmock.h>
using namespace testing;
class ABase64CodecUtils : public Test {
public:
};

TEST_F(ABase64CodecUtils, CanEncodeAndDecode) {
  auto test_str = "hello world，你好世界";
  auto encoded_str = re::Base64CodecUtils::Encode(test_str);
  auto decoded_str = re::Base64CodecUtils::Decode(encoded_str);
  ASSERT_THAT(decoded_str, Eq(test_str));
}