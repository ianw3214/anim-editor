#include <gl/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "imgui-filebrowser/ImGuiFileBrowser.h"

#include <chrono>
#include <string>

#include "graphics/renderer.hpp"

#include "controller.hpp"

#include "viewer/spritesheetViewer.hpp"

// Global data
// TODO: Actually store these variables somewhere
imgui_addons::ImGuiFileBrowser fileDialog;
bool open = false;
bool save = false;
std::string curr_image;

// Viewer data
float x_offset = 0.0;
float y_offset = 0.0;
bool panning = false;

///////////////////////////////////////////////////////////////////////
// File management widget
///////////////////////////////////////////////////////////////////////
void DrawFileWidget()
{
    ImGui::Begin("File", nullptr, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "CTRL+O"))
                open = true;
            if (ImGui::MenuItem("Save", "CTRL+S"))
                save = true;

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (open)
    {
        ImGui::OpenPopup("Open File");
        open = false;
    }
    if (save)
    {
        ImGui::OpenPopup("Save File");
        save = false;
    }

    if (fileDialog.showFileDialog("Open File", imgui_addons::ImGuiFileBrowser::DialogMode::OPEN, { 0, 0 }, ".png,.json"))
    {
        std::string file = fileDialog.selected_fn;     // Name of selected file
        std::string path = fileDialog.selected_path;   // Name of selected directory
        curr_image = file;
        Controller::LoadFreshAnimDataFromImage(path, file);
    }
    if (fileDialog.showFileDialog("Save File", imgui_addons::ImGuiFileBrowser::DialogMode::SAVE))
    {
        fileDialog.selected_fn;     // Name of selected file
        fileDialog.selected_path;   // Name of selected path
        fileDialog.ext;             // file extension
        // ImGui::CloseCurrentPopup();
    }

    ImGui::Text(curr_image.c_str());

    ImGui::End();
}

///////////////////////////////////////////////////////////////////////
// Entry point
///////////////////////////////////////////////////////////////////////
#ifdef PLATFORM_WINDOWS
#include <windows.h>
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main(int argc, char* argv[])
#endif
{
    SDL_Window * window;
    SDL_GLContext context;

    window = SDL_CreateWindow("Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    context = SDL_GL_CreateContext(window);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForOpenGL(window, context);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) return 1;

    Renderer::Init();

    SpritesheetViewer spritesheetViewer;

    double delta = 1 / 60.0;
    bool running = true;
    while (running)
    {
        auto start = std::chrono::system_clock::now();

        SDL_Event e;
        while(SDL_PollEvent(&e))
        {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
            spritesheetViewer.HandleEvent(e);
        }

        Renderer::Clear();

        // Draw the background .
        Renderer::DrawTiledQuad(0.0, 0.0, 1280.0, 720.0);

        spritesheetViewer.Render();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        ImGui::ShowDemoWindow(nullptr);

        DrawFileWidget();

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
        delta = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}