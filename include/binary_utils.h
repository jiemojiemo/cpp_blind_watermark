//
// Created by user on 12/15/23.
//

#ifndef BLINDWATERMARKTEST_BINARY_UTILS_H
#define BLINDWATERMARKTEST_BINARY_UTILS_H

#include "base64codec_utils.h"
#include <bitset>
#include <climits>
#include <string>
#include <vector>

namespace re {
class BinaryUtils {
public:
  static std::vector<uint8_t> asciiStrToBinArray(const std::string &str) {
    auto result = std::vector<uint8_t>();
    for (const char &c : str) {
      std::bitset<CHAR_BIT> b(c);
      for (int i = CHAR_BIT - 1; i >= 0; --i) {
        result.push_back(b[i]);
      }
    }
    return result;
  }

  static std::string binArrayToAsciiStr(const std::vector<uint8_t> &bin_array) {
    std::string ascii_str;
    for (size_t i = 0; i < bin_array.size(); i += CHAR_BIT) {
      std::bitset<CHAR_BIT> bits;
      for (size_t j = 0; j < CHAR_BIT; ++j) {
        bits.set(CHAR_BIT - 1 - j, bin_array[i + j]);
      }
      ascii_str.push_back(static_cast<char>(bits.to_ulong()));
    }
    return ascii_str;
  }

  static std::vector<uint8_t> utf8StrToBinArray(const std::string &str) {
    auto base64_str = Base64CodecUtils::Encode(str);
    return asciiStrToBinArray(base64_str);
  }

  static std::string binArrayToUtf8Str(const std::vector<uint8_t> &bin_array) {
    auto ascii_str = binArrayToAsciiStr(bin_array);
    return Base64CodecUtils::Decode(ascii_str);
  }
};
} // namespace my_lib

#endif // BLINDWATERMARKTEST_BINARY_UTILS_H
