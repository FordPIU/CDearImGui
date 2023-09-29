#include "Process.h"
#include "Timer.h"

#include <windows.h>
#include <vector>

////////////////////////////////////////////////////////////

class FrameStats
{
public:
    FrameStats(int bufferSize) : bufferSize(bufferSize)
    {
        fpsBuffer.resize(bufferSize, 0.0);
        renderTimeBuffer.resize(bufferSize, 0.0);
    }

    void Update(float renderTime, double currentFPS)
    {
        // Update FPS and render time buffers
        fpsBuffer[currentIndex] = currentFPS;
        renderTimeBuffer[currentIndex] = renderTime / 1000.0f;

        // Move to the next index in the circular buffer
        currentIndex = (currentIndex + 1) % bufferSize;
    }

    void RenderGraphs()
    {
        ImGui::PlotLines("FPS Over Time", fpsBuffer.data(), bufferSize, currentIndex, "FPS", 0.0, 100.0, ImVec2(0, 80));
        ImGui::PlotLines("Render Time Over Time", renderTimeBuffer.data(), bufferSize, currentIndex, "ms", 0.0, 50.0, ImVec2(0, 80));
    }

private:
    int bufferSize;
    int currentIndex = 0;
    std::vector<float> fpsBuffer;
    std::vector<float> renderTimeBuffer;
};

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
FrameStats frameStats = FrameStats(1000);
bool perFrame(int lastFrameTime)
{
    frameStats.Update(lastFrameTime, 1000000.0 / lastFrameTime);
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
        ImGui::Checkbox("Lag!", &lag);
        ImGui::SliderInt("Lag Amount", &lagAmount, 100000, 1000000);
        ImGui::SliderFloat("Lag Multiplier", &lagMultiplier, 0.5f, 100.f);
        frameStats.RenderGraphs();
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