project(CSxDTest)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories(/src)

add_executable(
    CSxDTest
    mocks/MockWeapon.h
    mocks/MockPlayer.h
    mocks/MockGame.h
    mocks/MockGamePlay.h
    WeaponTest.cc
    PlayerTest.cc
    GameTest.cc
    GamePlayTest.cc
    InteractionsTest.cc
)

target_link_libraries(
    CSxDTest
    CSxDLib
    pthread
    ${GTEST_LIBRARIES}
    gmock
    gtest_main
)

gtest_discover_tests(CSxDTest)