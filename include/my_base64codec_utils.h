//
// Created by user on 12/14/23.
//

#ifndef BLINDWATERMARKTEST_MY_BASE64CODEC_UTILS_H
#define BLINDWATERMARKTEST_MY_BASE64CODEC_UTILS_H
#include "libbase64.h"
#include <string>
namespace my_lib {
class Base64CodecUtils {
public:
  static std::string Encode(const std::string &str) {
    auto encode_size = static_cast<size_t>(str.length() * 1.5);
    std::unique_ptr<char[]> out(new char[encode_size]);
    size_t outlen = 0;
    base64_encode(str.c_str(), str.length(), out.get(), &outlen, 0);
    return {out.get(), outlen};
  }

  static std::string Decode(const std::string &str) {
    auto decode_size = static_cast<size_t>(str.length() * 0.75);
    std::unique_ptr<char[]> out(new char[decode_size]);
    size_t outlen = 0;
    auto ret = base64_decode(str.c_str(), str.length(), out.get(), &outlen, 0);
    if(ret != 1){
      printf("base64_decode failed\n");
      return "";
    }
    return {out.get(), outlen};
  }
};
} // namespace my_lib


#endif // BLINDWATERMARKTEST_MY_BASE64CODEC_UTILS_H
