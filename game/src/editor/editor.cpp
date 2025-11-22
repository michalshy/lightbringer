#include "editor.h"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include <SDL_events.h>
#include <SDL_mouse.h>
#include <spdlog/fmt/bundled/format.h>

bool Editor::Init(Game* game, SDL_Window* window, SDL_GLContext context)
{
    m_Context = context;
    m_Window = window;
    m_Game = game;

    if(!m_Game || !m_Context || !m_Window)
        return false;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   
    ImGuiStyle& style = ImGui::GetStyle();

    const char* glsl_version = "#version 330";
    
    ImGui_ImplSDL2_InitForOpenGL(window, context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

void Editor::OnFrame(float delta_time)
{
    if(m_CamOpt.free_cam)
        HandleKeyDown(delta_time);

    // ImGui Start of Frame (Your 'New Frame' equivalent)
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Now build the UI tree (Create your windows)
    UpdateUI();

    ImGui::Render();
}

void Editor::EndFrame()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::PostFrame()
{
    
}

void Editor::Exit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Editor::PollEvents(SDL_Event& e)
{
    ImGui_ImplSDL2_ProcessEvent(&e);
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if(m_CamOpt.free_cam)
    {
        if(e.type == SDL_MOUSEWHEEL)
        {
            HandleMouseWheel((float)e.wheel.y, (float)mouse_x, (float)mouse_y);
        }
    }
}

void Editor::UpdateUI()
{
    ImGui::Begin("Tiny Helper");
    ImGui::Text("Welcome to Tiny Helper!");
    ImGui::Text("Simple In-Game tool to view and tweak some values");
    ImGui::Separator();

    ImGui::Text("Game params");
    ImGui::Text("Map seed: %d", m_Game->m_Map->GetSeed());
    ImGui::Text("Frame rate: %.3f", 1.0f/m_Game->m_DeltaTime);
    ImGui::Separator();

    ImGui::Text("Mouse params");
    int x, y; 
    SDL_GetMouseState(&x, &y);
    ImGui::Text("X: %d, Y: %d", x, y);
    ImGui::Separator();
    
    ImGui::Text("Camera params");
    ImGui::Text("ViewProjectile Matrix");
    ShowMatrix(m_Game->m_Camera->GetViewMatrix());

    ImGui::Separator();
    ImGui::Text("Timer");
    ImGui::Text("Delta:");
    ImGui::Text("%f", m_Game->m_DeltaTime);
    ImGui::Text("Global:");
    ImGui::Text("%f", m_Game->m_GlobalTime);
    ImGui::Separator();

    if(ImGui::CollapsingHeader("Camera Options"))
    {
        if(ImGui::Checkbox("Enable free camera", &m_CamOpt.free_cam))
        {
            m_CamOpt.free_cam == true ? m_Game->m_Player->ToggleInput(false) : m_Game->m_Player->ToggleInput(true);
        }
    }

    if(ImGui::CollapsingHeader("Map Visualizer"))
    {
        if(ImGui::Button("Run cycle"))
        {
            m_Game->m_Map->RunCycle();
        }
    }

    if(ImGui::CollapsingHeader("Lights"))
    {
        ImGui::Checkbox("Respect light map", &m_Game->m_Debug.respect_lightmap);
    }

    ImGui::End();
}

void Editor::HandleMouseWheel(float w, float mouse_x, float mouse_y)
{
    m_Game->m_Camera->ProcessMouseScroll(w, mouse_x, mouse_y);
}

void Editor::HandleKeyDown(float delta_time)
{
    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_W])
        m_Game->m_Camera->ProcessKeyboard(UP, delta_time);
    if (state[SDL_SCANCODE_S])
        m_Game->m_Camera->ProcessKeyboard(DOWN, delta_time);
    if (state[SDL_SCANCODE_A])
        m_Game->m_Camera->ProcessKeyboard(LEFT, delta_time);
    if (state[SDL_SCANCODE_D])
        m_Game->m_Camera->ProcessKeyboard(RIGHT, delta_time);
}

void Editor::ShowMatrix(const glm::mat4& mat)
{
    for (int i = 0; i < 4; i++)
    {
        ImGui::Text("%.3f %.3f %.3f %.3f",
            mat[i][0], mat[i][1], mat[i][2], mat[i][3]);
    }
}