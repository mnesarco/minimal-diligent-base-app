#include "main.hpp"
#include "imgui.h"

namespace Diligent
{

Application* CreateDiligentApplication()
{
    return new MarzdUI();
}

void MarzdUI::ProcessCommandLine(const char* CmdLine)
{
    Application::ProcessCommandLine(CmdLine);
}

void MarzdUI::Initialize(const ApplicationInitInfo& InitInfo)
{
    Application::Initialize(InitInfo);
}

MarzdUI::~MarzdUI()
{
}

// Render a frame
void MarzdUI::Render()
{
    auto* pRTV = m_pSwapChain->GetCurrentBackBufferRTV();
    auto* pDSV = m_pSwapChain->GetDepthBufferDSV();

    // Clear the back buffer
    const float ClearColor[] = {0.032f, 0.032f, 0.032f, 1.0f};
    m_pImmediateContext->ClearRenderTarget(pRTV, ClearColor, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);
    m_pImmediateContext->ClearDepthStencil(pDSV, CLEAR_DEPTH_FLAG, 1.f, 0, RESOURCE_STATE_TRANSITION_MODE_TRANSITION);

}


void MarzdUI::Update(double CurrTime, double ElapsedTime)
{

    Application::Update(CurrTime, ElapsedTime);

    static float value = 0.0;
    static unsigned int clicks = 0;

    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Test", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Hello, world %d", 123);
        if (ImGui::Button("Test Button"))
            ++clicks;
        ImGui::Text("Click count %d", clicks);
        ImGui::SliderFloat("Test slider", &value, 0.0f, 100.0f);
    }
    ImGui::End();

}

} // namespace Diligent
