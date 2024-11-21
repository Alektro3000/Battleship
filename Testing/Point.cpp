#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include "../Players/Player.h"

TEST_CASE( "saving", "[battleship]" ) {
    BattleShip ship{{1,2},3,Rotation::Left};
    CHECK(ship.getX() == 1);
    CHECK(ship.getY() == 2);
    CHECK(ship.getLength() == 3);
    CHECK(ship.getRotation() == Rotation::Left);
}


TEST_CASE( "intersections", "[battleship]" ) {
    BattleShip ship{{0,0},1,Rotation::Down};
    CHECK(ship.IntersectionPosition({0,0}) == 0);
    CHECK(ship.IntersectionPosition({1,0}) == -1);

    ship = BattleShip{{0,0},2,Rotation::Up};
    CHECK(ship.IntersectionPosition({0,0}) == 0);
    CHECK(ship.IntersectionPosition({0,1}) == 1);
    CHECK(ship.IntersectionPosition({0,2}) == -1);
    CHECK(ship.IntersectionPosition({1,0}) == -1);
    CHECK(ship.IntersectionPosition({0,-2}) == -1);
    CHECK(ship.IntersectionPosition({0,-1}) == -1);
    
    ship = BattleShip{{0,0},2,Rotation::Down};
    CHECK(ship.IntersectionPosition({0,0}) == 0);
    CHECK(ship.IntersectionPosition({0,-1}) == 1);
    CHECK(ship.IntersectionPosition({0,-2}) == -1);
    CHECK(ship.IntersectionPosition({1,0}) == -1);
    CHECK(ship.IntersectionPosition({0,2}) == -1);
    CHECK(ship.IntersectionPosition({0,1}) == -1);
    
    ship = BattleShip{{0,0},2,Rotation::Right};
    CHECK(ship.IntersectionPosition({0,0}) == 0);
    CHECK(ship.IntersectionPosition({1,0}) == 1);
    CHECK(ship.IntersectionPosition({2,0}) == -1);
    CHECK(ship.IntersectionPosition({0,1}) == -1);
    CHECK(ship.IntersectionPosition({-2,0}) == -1);
    CHECK(ship.IntersectionPosition({-1,0}) == -1);
    
    ship = BattleShip{{0,0},2,Rotation::Left};
    CHECK(ship.IntersectionPosition({0,0}) == 0);
    CHECK(ship.IntersectionPosition({-1,0}) == 1);
    CHECK(ship.IntersectionPosition({-2,0}) == -1);
    CHECK(ship.IntersectionPosition({0,1}) == -1);
    CHECK(ship.IntersectionPosition({2,0}) == -1);
    CHECK(ship.IntersectionPosition({1,0}) == -1);
    
    ship = BattleShip{{0,0},5,Rotation::Right};
    CHECK(ship.IntersectionPosition({0,0}) == 0);
    CHECK(ship.IntersectionPosition({1,0}) == 1);
    CHECK(ship.IntersectionPosition({2,0}) == 2);
    CHECK(ship.IntersectionPosition({3,0}) == 3);
    CHECK(ship.IntersectionPosition({4,0}) == 4);
    CHECK(ship.IntersectionPosition({5,0}) == -1);
    CHECK(ship.IntersectionPosition({-2,0}) == -1);
    CHECK(ship.IntersectionPosition({-1,0}) == -1);
}
