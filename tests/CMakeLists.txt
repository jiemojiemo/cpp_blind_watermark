if (NOT TARGET gmock)
    add_subdirectory(googletest EXCLUDE_FROM_ALL)
endif ()

add_executable(unit_test test_base64_codec_utils.cpp)
target_link_libraries(unit_test PRIVATE gmock_main my_lib)

add_test(NAME unit_test
        WORKING_DIRECTORY ./
        COMMAND unit_test)