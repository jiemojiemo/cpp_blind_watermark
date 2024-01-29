//
// Created by user on 12/14/23.
//

#include "binary_utils.h"
#include <gmock/gmock.h>

using namespace testing;
using namespace re;

class ABinaryUtils : public Test {
public:
};

TEST_F(ABinaryUtils, CanTransASCIIStringToBinaryArray) {
  auto test_str = "hello world";
  auto array = BinaryUtils::asciiStrToBinArray(test_str);
  auto str = BinaryUtils::binArrayToAsciiStr(array);

  ASSERT_THAT(str, Eq(test_str));
}

TEST_F(ABinaryUtils, CanTransUTF8StringToBinaryArray) {
  auto test_str = "hello world, 你好世界";
  auto array = BinaryUtils::utf8StrToBinArray(test_str);
  auto str = BinaryUtils::binArrayToUtf8Str(array);

  ASSERT_THAT(str, Eq(test_str));
}