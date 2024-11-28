#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include "../Players/Player.h"

TEST_CASE( "PointI", "[PointI]" ) {
    CHECK( PointI(2,2) + PointF(2.2,2.2) == PointF(4.2,4.2));
    CHECK( PointF(2.2,2.2) + PointI(2,2) == PointF(4.2,4.2));

    CHECK( PointI(2,2) * PointF(2.2,2.2) == PointF(4.4,4.4));
    CHECK( PointF(2.2,2.2) * PointI(2,2) == PointF(4.4,4.4));
    
    CHECK( PointF(2.2,2.2) / PointI(2,2) == PointF(1.1,1.1));
    CHECK( PointI(2.2,2.2) / PointI(2,2) == PointI(1,1));

    CHECK( PointF(2.2,2.2) / 2 == PointF(1.1,1.1));
    CHECK( PointI(2.2,2.2) / 2 == PointI(1.1,1.1));
}


TEST_CASE( "saving", "[battleship]" ) {
    BattleShip ship{{1,2},3,Rotation::Left};
    CHECK(ship.getX() == 1);
    CHECK(ship.getY() == 2);
    CHECK(ship.getLength() == 3);
    CHECK(ship.getRotation() == Rotation::Left);
    CHECK(sizeof(BattleShip) == 4);
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


TEST_CASE( "rects", "[battleship]" ) {
    BattleShip ship = BattleShip{{0,0},5,Rotation::Right};
    CHECK( (ship.getRect() == RectI{{0,0},{4,0}}));
    BattleShip ship1 = BattleShip{{0,0},5,Rotation::Left};
    BattleShip ship2 = BattleShip{{1,0},5,Rotation::Right};
    auto rect = ship1.getRect() ;
    CHECK( rect == RectI{{-4,0},{0,0}});
    CHECK( ship.hasIntersection(ship1));
    CHECK( !ship1.hasIntersection(ship2));
    CHECK( ship1.hasIntersectionCorner(ship2));
    CHECK( ship.hasIntersection(ship2));

    SECTION("ADJ + Corner")
    {
        CHECK( !ship.hasIntersection(PointI{0,1}));
        CHECK( !ship.hasIntersection(PointI{1,1}));
        CHECK( ship.hasIntersectionAdj(PointI{0,1}));
        CHECK( ship.hasIntersectionAdj(PointI{1,1}));
        CHECK( !ship.hasIntersectionAdj(PointI{5,1}));
        CHECK( ship.hasIntersectionCorner(PointI{5,1}));
    }
}