cmake_minimum_required(VERSION 3.25.0)
cmake_policy(SET CMP0167 NEW)


project(Battleship)

find_package(Boost 1.86.0 REQUIRED)
find_package(directxtk CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(main WIN32 DXMain.cpp DXApp.cpp)

target_link_libraries(main PRIVATE 
d3d11.lib dxgi.lib dxguid.lib uuid.lib
kernel32.lib user32.lib d3dcompiler.lib d2d1.lib
comdlg32.lib advapi32.lib shell32.lib
ole32.lib oleaut32.lib Microsoft::DirectXTK )