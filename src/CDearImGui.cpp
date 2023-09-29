#include "Process.h"
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
bool lag;
int lagAmount = 10000;
float lagMultiplier = 1.f;
int counter = 0;
int counter2 = 0;
int frameCount = 0;
bool perFrame(int lastFrameTime)
{
    frameCount++;

    // WindowA
    if (isWindowAOpen)
    {
        ImGui::Begin("CDearImGui", &isWindowAOpen);
        
    }

    // WindowB
    if (isWindowBOpen)
    {
        std::string sRenderTime = getRenderTime(lastFrameTime);
        std::string sFps = getFramePerSecond(lastFrameTime);
        const char *renderTime = sRenderTime.c_str();
        const char *fps = sFps.c_str();

        // ImGui Rendering
        ImGui::Begin("Timings Debug", &isWindowBOpen);
        ImGui::Text("Render Time: %s", renderTime);
        ImGui::Text("FPS: %s", fps);
        ImGui::Text("Frame Count: %i", frameCount);
        ImGui::Separator();
        if (ImGui::Button("Lag!"))
            lag = !lag;
        ImGui::SliderInt("Lag Amount", &lagAmount, 10000, 100000);
        ImGui::SliderFloat("Lag Multiplier", &lagMultiplier, 0.5f, 100.f);
        ImGui::End();

        if (lag)
        {
            int value = 0;
            for (int i = 0; i < (lagAmount * lagMultiplier); ++i)
            {
                value = static_cast<int>(std::sqrt(value * i));
            }
        }
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
    Process proccess = Process(perFrame);
    proccess.launch(1, 1, "Hello World!", 0, 0, rgbaVec(0.f, 0.f, 0.f, 0.f));
}

////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    main();

    return 0;
}