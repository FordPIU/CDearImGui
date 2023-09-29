#include "ProcessManager.h"
#include "Timer.h"

#include <windows.h>

////////////////////////////////////////////////////////////

std::string getRenderTime(int lastFrameTime)
{
    std::string renderTime_String = TimerFuncs::convertTime(lastFrameTime);

    return renderTime_String;
}

std::string getFramePerSecond(int lastFrameTime)
{
    double lastFrameFPS = 1000000.0 / lastFrameTime;
    std::string fps_String = std::to_string(lastFrameFPS);

    return fps_String;
}

////////////////////////////////////////////////////////////

bool isWindowAOpen = true;
bool isWindowBOpen = true;
int counter = 0;
int counter2 = 0;
bool perFrame(int lastFrameTime)
{
    // WindowA
    if (isWindowAOpen)
    {
        ImGui::Begin("Window A", &isWindowAOpen);
        ImGui::Text("Hello, world!");
        if (ImGui::Button("Count!"))
            counter++;
        ImGui::Text("counter = %d", counter);
        ImGui::End();
    }

    // WindowB
    if (isWindowBOpen)
    {
        std::string sRenderTime = getRenderTime(lastFrameTime);
        std::string sFps = getFramePerSecond(lastFrameTime);
        const char *renderTime = sRenderTime.c_str();
        const char *fps = sFps.c_str();

        // ImGui Rendering
        ImGui::Begin("Window B", &isWindowBOpen);
        ImGui::Text("Hello, world 2!");
        if (ImGui::Button("Count!"))
            counter2 += 5;
        ImGui::Text("counter = %d", counter2);
        ImGui::Text("Render Time: %s", renderTime);
        ImGui::Text("FPS: %s", fps);
        ImGui::End();
    }

    // Check if both windows are closed
    if (!isWindowAOpen && !isWindowBOpen)
    {
        return true;
    }

    return false;
}

////////////////////////////////////////////////////////////

int main()
{
    ProcessManager proccess = ProcessManager(perFrame);
    proccess.launch(1, 1, "Hello World!", 0, 0, rgbaVec(0.f, 0.f, 0.f, 0.f));
}

////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    main();

    return 0;
}