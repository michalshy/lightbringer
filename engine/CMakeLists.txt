project(engine LANGUAGES CXX)

# ---------------------------------------------------------
# IMGUI (FetchContent)
# ---------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG docking
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_SRC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)

target_link_libraries(imgui PUBLIC
    SDL2::SDL2
    OpenGL::GL
    $<IF:$<PLATFORM_ID:Linux>,GLEW::GLEW,glew32s>
)

# ---------------------------------------------------------
# ENGINE SOURCES
# ---------------------------------------------------------
file(GLOB_RECURSE ENGINE_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE ENGINE_HDR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.h)

add_library(engine STATIC ${ENGINE_SRC} ${ENGINE_HDR})

# PUBLIC HEADERS
target_include_directories(engine PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor/entt/single_include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>

      # --- SDL as submodules ----
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor/SDL2/include>
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/vendor/SDL2_image/include>
)

# GLEW library path
target_link_directories(engine PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# ---------------------------------------------------------
# LINK LIBRARIES
# ---------------------------------------------------------
target_link_libraries(engine PUBLIC
      SDL2::SDL2
      SDL2::SDL2main
      SDL2_image::SDL2_image
      Threads::Threads
      OpenGL::GL
      $<IF:$<PLATFORM_ID:Linux>,GLEW::GLEW,glew32s>
      spdlog::spdlog
      glm::glm
)

if(MSVC)
    target_compile_options(engine PRIVATE /W4)
else()
    target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
endif()
