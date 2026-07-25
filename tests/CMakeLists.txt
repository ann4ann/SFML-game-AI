add_executable(test_main
    test_main.cpp
    test_movement.cpp
    test_collision.cpp
    test_score.cpp
    test_spawn.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/MovementSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/CollisionSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/systems/EnemySpawnSystem.cpp
)
target_link_libraries(test_main PRIVATE Catch2::Catch2WithMain sfml-graphics sfml-window sfml-system sfml-audio)
target_include_directories(test_main PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_features(test_main PRIVATE cxx_std_17)

# Подключаем тесты к CTest
include(Catch)
catch_discover_tests(test_main)
