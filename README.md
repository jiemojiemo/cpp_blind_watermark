# Blind Watermark Algorithm C/Cpp Implementation
This is a code repository implementing the Blind watermark algorithm using c/cpp, including the algorithm source code and demo examples.
More details in [盲水印、暗水印（Blind Watermark）算法简明教程：算法原理、流程以及基于C/C++ 的代码实现](https://blog.csdn.net/weiwei9363/article/details/136175834)

# Features
+ Written in C and C++ languages
+ Integrated with cmake
+ Based on the code logic of a Python library

# Dependencies
+ conan/2.0
+ OpenCV/4.1.2 
+ base64/0.5.1

# Build
This project uses cmake and conan for building. You need to [install conan](https://docs.conan.io/2/installation.html) first, and then install the dependencies using conan.
You can use the following commands to build:

```bash
git clone https://github.com/jiemojiemo/cpp_blind_watermark
cd cpp_blind_watermark
git submodule update --init --recursive
mkdir build
conan install . --output-folder=./build --build=missing --settings=build_type=Release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=./build/conan_toolchain.cmake -B build -S .
cmake --build build -- -j 10
```

# Usage
You can run the demo to see the implementation effect of the algorithm.

# Contributing
Any form of contribution is welcome, including but not limited to issue reports, code patches, documentation improvements, and feature suggestions.

# Contact
If you have any questions or suggestions, feel free to contact me through issues or email.

# Acknowledgements
Thanks to [guofei9987/blind_watermark](https://github.com/guofei9987/blind_watermark) for providing the code logic.
