# Blind Watermark Algorithm C/Cpp Implementation
This is a code repository implementing the Blind watermark algorithm using c/cpp, including the algorithm source code and demo examples.

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
mkdir build
conan install . --output-folder=./build --build=missing --settings=build_type=Debug
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

# Usage
You can run the demo to see the implementation effect of the algorithm.

# Contributing
Any form of contribution is welcome, including but not limited to issue reports, code patches, documentation improvements, and feature suggestions.

# Contact
If you have any questions or suggestions, feel free to contact me through issues or email.

# Acknowledgements
Thanks to [guofei9987/blind_watermark](https://github.com/guofei9987/blind_watermark) for providing the code logic.