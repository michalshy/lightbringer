# Lightbringer — 2D OpenGL Game & Engine (SDL2 + Premake)

Lightbringer is a small, educational 2D game project built on a modular game engine written in C++. It demonstrates a compact engine architecture with a tile/mesh-based renderer, resource manager, scene/system components, basic AI and lightmaps, and an optional in-editor UI.

This repo includes the engine, a lightweight game that uses it, and vendored 3rd-party libraries (GLAD, GLM, SDL2, ImGui, spdlog, EnTT, etc.) to make it easy to build and run on Windows and Linux using Premake.

---

## Highlights
- Engine: Shared library (`Engine`) with rendering, resource management, scene & entities.
- Game: `Lightbringer` game using the engine: procedural map generation (cellular automata), player, lights, enemies.
- Renderer: OpenGL renderer using instanced quads and sprite sheets.
- Tools: Optional ImGui-based Editor (compile-time flag `BUILD_WITH_EDITOR`).
- Build system: Premake 5 + Visual Studio (Windows) or GNU Make (Linux).

---

## Repository Layout

Top-level folders and key files:
- `engine/` — Engine source & headers (shared library). Contains renderer, resource manager, scene, etc.
- `game/` — Game source & headers using the engine. Holds `main`, `App`, `Game`, `Map`, `Player`, and optional Editor.
- `libs/` — Vendored third-party libraries and Premake includes for building them.
- `res/` — Runtime resources: `res/shaders`, `res/sprites` and tilesets.
- `premake5.lua` — Workspace configuration used to generate Visual Studio / Make build files.
- `Lightbringer.sln`, `Engine.vcxproj` — Visual Studio projects (check `build/` after generating projects).

---

## Requirements
- Premake5 (recommended) — https://premake.github.io/
- Visual Studio 2019/2022 with C++ workload (Windows) or a modern GCC/Clang toolchain (Linux)
- A working OpenGL driver

Third-party libraries are included in `libs/` and are configured as part of the Premake workspace; you do not need to install them system-wide.

---

## Build Instructions

Note: the repository vendors the 3rd-party dependencies and contains premake files for building them as part of the workspace.

Windows (Visual Studio):

1. Open an elevated command prompt or Developer Command Prompt for VS.
2. Generate a Visual Studio solution (prebuilt `premake5.exe` is included):

```cmd
cd c:\Dev\Projects\lightbringer
.\premake5.exe vs2019
```

3. Open `Lightbringer.sln` in Visual Studio and build the `Lightbringer` project (Debug/Release as desired).

4. The executable will be placed in:

```
build\game\bin\Debug\Lightbringer.exe
```

When building on Windows, the workspace includes post-build steps to copy resource files (`res/`) and required DLLs (Engine DLL and SDL2 DLLs) into the game's output directory.

Linux (GNU Make):

1. Generate Makefiles using Premake and build:

```bash
cd /path/to/lightbringer
premake5 gmake2
make -j$(nproc)
```

2. The binaries will be written to `build/game/bin/<config>/` (e.g. `build/game/bin/Debug/`).

---

## Run the Game

From the build directory, run the executable:

```bash
./build/game/bin/Debug/Lightbringer
```

On Windows you can run the `Lightbringer.exe` directly, or run it from Visual Studio.

Controls
- WASD — move player
- `Backquote` (`) toggles fullscreen
- `Escape` — quit

---

## Build Variants & Optional Editor

- Debug/Release: Build configurations are available via Premake and your IDE.
- Editor UI: To enable the in-game ImGui editor, add a preprocessor definition `BUILD_WITH_EDITOR` to the `Lightbringer` project. With premake, you can edit `premake5.lua` and add `defines { "BUILD_WITH_EDITOR" }` for the `Lightbringer` project configuration or set the define in your IDE.

---

## Project Architecture (High Level)

- Engine (`engine/`):
  - Renderer: OpenGL instanced quad renderer (`engine/src/renderer`).
  - ResourceManager: loads sprite sheets and textures (`engine/src/resources`).
  - Scene/Entity model: ECS-like approach and components (`engine/src/scene`).
  - LightManager: simple light map management for game lighting (`engine/src/renderer/` and `engine/src/renderer/light_manager.*`).
  - Window: SDL2 based window/context wrapper (`engine/src/window`).

- Game (`game/`):
  - App: top-level application lifecycle (init/run/exit) using Engine services.
  - Game code: `Map` (procedural generated levels), `Player`, `Game` class for updating/drawing.
  - Optional Editor: ImGui-based tooling for dev-time UI.

---

## Resources
- The game uses the `TILESET` resource (`game/src/config_params.h`) and loads `res/sprites/sheets/tileset.png`.
- Shaders are in `res/shaders/` and are automatically packaged/copied by the Premake post-build step to the binary folder.

---

## Development & Contributing

- To add a new feature, create a branch from `main` and open a pull request.
- Keep changes small and focused: separate engine changes from game-specific changes when possible.
- Follow simple C++17 coding conventions; the project uses `C++17` and expects modern toolchains.

Suggested items for contributors:
- Add unit tests and examples where reasonable
- Improve/extend engine APIs (Rendering, Resources, Scene/Components)
- Add CI to build/validate cross-platform builds.

---

## Licensing

This project currently vendors third-party libraries, each with its own license; check individual `libs/*` subdirectories for license files.

- NOTE: There is no top-level license file in this repo. If you intend to use or redistribute this project, consider adding a top-level LICENSE (e.g. MIT) and ensure you comply with the licenses of vendored libraries.

---

If you need help building or running the project, provide the platform (Windows/Linux), which toolchain you're using (Visual Studio / GCC / Clang), and any error messages you see — I can help debug build issues and refine the README further.

Happy hacking! 💡

++ End README# OpenGL Premake5 Boilerplate

This project a starting point for any application using GLFW, GLAD and GLM. It uses Premake5 to generate the build files. 
It provides the 'engine.h' header file which imports the libraries and there is a basic GLFW example in 'main.cpp'.

## Getting Started

Since this project builds all libraries from source and includes these as submodules, don't forget to clone the project recursively.

### Prerequisites
You will need premake5 to build this project. Download it [HERE](https://premake.github.io/download/html#v5).


### Building
Building the project is very easy. Use premake to generate the project files for your platform. Then use the correct build system to build the project.

#### Linux

```
premake5 gmake
make -j$(nproc)
```

#### Windows

```
premake5 vs2019
```

## Built With
- GLFW [Github](https://github.com/glfw/glfw)
- GLAD [Github](https://github.com/Dav1dde/glad)
- GLM  [Github](https://github.com/g-truc/glm)

## License

This project is licensed under the MIT License.
