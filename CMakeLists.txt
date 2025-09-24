cmake_minimum_required(VERSION 3.20)
project(FinalProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# vcpkg toolchain is configured via .vscode/settings.json
# Uncomment to hardcode it here instead:
# set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base)

# WIN32: build a GUI app (no console)
add_executable(FinalProject WIN32 src/main.cpp)
target_link_libraries(FinalProject PRIVATE wx::core wx::base)

cmake_minimum_required(VERSION 3.10)
project(MyWxApp)

find_package(wxWidgets REQUIRED)
include_directories(${wxWidgets_INCLUDE_DIRS})

add_executable(MyWxApp main.cpp)
target_link_libraries(MyWxApp ${wxWidgets_LIBRARIES})
