set(TEST_NAME LoggerTest)

add_executable(
  ${TEST_NAME}
  LoggerTest.cpp
)

target_link_libraries(
  ${TEST_NAME}
  GTest::gtest_main
)
    
include(GoogleTest)
gtest_discover_tests(${TEST_NAME})