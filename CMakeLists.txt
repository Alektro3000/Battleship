cmake_minimum_required(VERSION 3.25.0)
cmake_policy(SET CMP0167 NEW)

project(Battleship)

find_package(Boost 1.86.0 COMPONENTS asio REQUIRED)
find_package(directx-headers CONFIG REQUIRED)
find_package(Catch2 3 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(BattleCpps BattleWidget.cpp SelectWidget.cpp Grids/BattleGrid.cpp Grids/OpponentGrid.cpp Grids/PlayerGrid.cpp Grids/BeginGrid.cpp Grids/Grid.cpp)
list(TRANSFORM BattleCpps PREPEND "Battle/")

set(VisualCpps Base.cpp Menu.cpp Net/MakeServer.cpp Net/ServerList.cpp Net/ServerNode.cpp Net/ServerConnections.cpp Base/TextBox.cpp ${BattleCpps}) 
list(TRANSFORM VisualCpps PREPEND "Widget/")
set(BaseCpps Players/Player.cpp Players/PcPlayer.cpp Players/NetPlayer.cpp Players/BattleShip.cpp DXApp.cpp ${VisualCpps}) 

add_executable(main WIN32 Main.cpp ${BaseCpps})
target_link_libraries(main PRIVATE iphlpapi ws2_32 d2d1.lib dwrite.lib)

set(resources 1.png 2.png 3.png 4.png back.png)
list(TRANSFORM resources PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/res/)
file(COPY ${resources}
     DESTINATION ${CMAKE_BINARY_DIR}/Debug)

include_directories(asio ${Boost_INCLUDE_DIRS})


add_executable(tests Testing/Point.cpp ${BaseCpps})
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE iphlpapi ws2_32 d2d1.lib dwrite.lib)

include(CTest)
include(Catch)