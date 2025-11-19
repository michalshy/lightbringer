project(game LANGUAGES CXX)

file(GLOB_RECURSE GAME_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gameplay/*.cpp
)

add_executable(game
    src/main.cpp
    ${GAME_SRC}
)

target_link_libraries(game PRIVATE
    engine
    SDL2::SDL2
    SDL2::SDL2main
    imgui
)

target_include_directories(game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(MSVC)
    target_compile_options(game PRIVATE /W4)
else()
    target_compile_options(game PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copy resources near binary
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:game>/res
    COMMENT "Copying resources for game"
)
