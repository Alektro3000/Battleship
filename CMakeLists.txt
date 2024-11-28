cmake_minimum_required(VERSION 3.25.0)
cmake_policy(SET CMP0167 NEW)


project(Battleship)

find_package(Boost 1.86.0 COMPONENTS asio REQUIRED)
find_package(directxtk CONFIG REQUIRED)
find_package(Catch2 3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(BaseCpps Point.cpp Players/Player.cpp Players/PcPlayer.cpp DXApp.cpp Screen/BattleApp.cpp 
Visual/Grid.cpp Visual/Base.cpp Visual/BattleGrid.cpp Visual/BeginGrid.cpp) 

add_executable(main WIN32 Main.cpp ${BaseCpps})

target_link_libraries(main PRIVATE 
d3d11.lib dxgi.lib dxguid.lib uuid.lib
kernel32.lib user32.lib d3dcompiler.lib d2d1.lib
comdlg32.lib advapi32.lib shell32.lib
ole32.lib oleaut32.lib Microsoft::DirectXTK )

include_directories(asio ${Boost_INCLUDE_DIRS})
add_executable(net Asio.cpp)

target_link_libraries(net ${Boost_LIBRARIES})


add_executable(tests Testing/Point.cpp ${BaseCpps})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)