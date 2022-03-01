#include "pch.h"
#include "ImGuiManager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "Device.h"
#include "CommandQueue.h"

ImGuiManager::ImGuiManager(HWND hwnd, std::shared_ptr<Device> device)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = 1;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    assert(device->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_srvDescHeap)) == S_OK);

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
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX12_Init(device->GetDevice().Get(), 2,
        DXGI_FORMAT_R8G8B8A8_UNORM, _srvDescHeap.Get(),
        _srvDescHeap->GetCPUDescriptorHandleForHeapStart(),
        _srvDescHeap->GetGPUDescriptorHandleForHeapStart());

    // TEST
    float3* f = new float3();
    positions.push_back(f);
}

ImGuiManager::~ImGuiManager()
{
    // Cleanup
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    delete positions[0];
}

void ImGuiManager::Render()
{
    // Start the Dear ImGui frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    static float f = 0.0f;
    static int counter = 0;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

    ImGui::SliderFloat3("Position", reinterpret_cast<float*>(positions[0]), 0.0f, 10000.f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();

    // Rendering
    ImGui::Render();
}

void ImGuiManager::SetPipeline(std::shared_ptr<GraphicsCommandQueue> cmdqueue)
{
    ID3D12DescriptorHeap* heaps[] = { _srvDescHeap.Get() };
    cmdqueue->GetGraphicsCmdList()->SetDescriptorHeaps(1, heaps);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdqueue->GetGraphicsCmdList().Get());
}