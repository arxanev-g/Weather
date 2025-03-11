cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD             20)
set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/tools/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

set(SOLUTION_SRC)
set(SOLUTION_TEST_SRC test/test_api.cpp)
if (TEST_SOLUTION)
  include_directories(../private/weather)
  set(SOLUTION_SRC ../private/bot/weather/api.cpp weather/client.cpp weather/client.h weather/bot.cpp weather/bot.h)
  set(SOLUTION_TEST_SRC ../private/bot/test/test_api.cpp)
endif()

if (TEST_SOLUTION)
  include_directories(/usr/include/jsoncpp)
elseif (APPLE)
  link_directories(/usr/local/lib)
  link_directories(/usr/local/opt/openssl/lib)

  include_directories(/usr/local/include)
  include_directories(/usr/local/opt/openssl/include)
endif()

add_library(weather
  ${SOLUTION_SRC}
  weather/main.cpp
  weather/map.cpp
  weather/weather.cpp)

target_include_directories(weather PUBLIC .)

target_link_libraries(weather
  PocoNet
  PocoNetSSL
  PocoFoundation
  PocoJSON)


add_executable(run
    weather/main.cpp)

target_link_libraries(run
  weather)

