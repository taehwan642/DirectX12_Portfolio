#include "pch.h"
#include "ImGuiManager.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "MonoBehaviour.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "BaseCollider.h"
#include "Animator.h"

ImGuiManager::ImGuiManager(HWND hwnd, std::shared_ptr<Device> device)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = 1;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    assert(device->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_srvDescHeap)) == S_OK);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsClassic();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX12_Init(device->GetDevice().Get(), 2,
        DXGI_FORMAT_R8G8B8A8_UNORM, _srvDescHeap.Get(),
        _srvDescHeap->GetCPUDescriptorHandleForHeapStart(),
        _srvDescHeap->GetGPUDescriptorHandleForHeapStart());
}

ImGuiManager::~ImGuiManager()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::Render()
{
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    RenderClientData();
    RenderHierarchy();
    RenderInspector();

    ImGui::Render();
}

void ImGuiManager::SetPipeline(std::shared_ptr<GraphicsCommandQueue> cmdqueue)
{
    ID3D12DescriptorHeap* heaps[] = { _srvDescHeap.Get() };
    cmdqueue->GetGraphicsCmdList()->SetDescriptorHeaps(1, heaps);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdqueue->GetGraphicsCmdList().Get());
}

void ImGuiManager::RenderClientData()
{
    ImGui::Begin("Client Data");

    ImGui::Text("Draw Calls : %d", _drawCall);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::End();
}

void ImGuiManager::RenderHierarchy()
{
    ImGui::Begin("Hierarchy");

    const std::vector<std::shared_ptr<GameObject>>& vec = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
    for (auto& iter : vec)
    {
        std::string tempString = std::string(iter->GetName().begin(), iter->GetName().end());
        if (ImGui::Button(tempString.c_str()))
        {
            _currentGameObject = iter;
        }
    }

    ImGui::End();
}

void ImGuiManager::RenderInspector()
{
    ImGui::Begin("Inspector");

    if (_currentGameObject != nullptr)
    {
        std::string tempString = std::string(_currentGameObject->GetName().begin(), _currentGameObject->GetName().end());
        ImGui::Text("Selected Object : %s", tempString.c_str());


        // Component들 출력

        // TRANSFORM
        if (_currentGameObject->GetTransform() != nullptr)
        {
            // Scale, Transform, Rotation, 부모 출력
            if (ImGui::BeginMenu("Transform"))
            {
                std::shared_ptr<Transform> trans = _currentGameObject->GetTransform();
                float position[3];
                Vec3 pos = trans->GetLocalPosition();
                position[0] = pos.x;
                position[1] = pos.y;
                position[2] = pos.z;

                ImGui::InputFloat3("Position", position);

                float rotation[3];
                Vec3 rot = trans->GetLocalRotation();
                rotation[0] = rot.x;
                rotation[1] = rot.y;
                rotation[2] = rot.z;

                ImGui::InputFloat3("Rotation", rotation);

                float scale[3];
                Vec3 sc = trans->GetLocalScale();
                scale[0] = sc.x;
                scale[1] = sc.y;
                scale[2] = sc.z;

                ImGui::InputFloat3("Scale", scale);


                ImGui::EndMenu();
            }
        }

	    // MESH_RENDERER
        if (_currentGameObject->GetMeshRenderer() != nullptr)
        {
            // Material, Mesh, Shader 출력
        }

	    // CAMERA
        if (_currentGameObject->GetCamera() != nullptr)
        {
            // Transform, Rotation, Fov, width, height, orthographic/perspective 출력
        }

	    // LIGHT
        if (_currentGameObject->GetLight() != nullptr)
        {
            // light type 출력, light position, rotation 출력
        }

	    // PARTICLE_SYSTEM
        if (_currentGameObject->GetParticleSystem() != nullptr)
        {
            // 멤버변수 모두 출력
        }

	    // TERRAIN
        if (_currentGameObject->GetTerrain() != nullptr)
        {
            // width, height, 사용된 heightmap 가능하다면 출력
        }

	    // COLLIDER
        if (_currentGameObject->GetCollider() != nullptr)
        {
            // Collider 렌더할지 출력, Radius, Center 등 출력
        }

	    // ANIMATOR
        if (_currentGameObject->GetAnimator() != nullptr)
        {
            // 이건 일단 보류.
        }

        for (auto& iter : _currentGameObject->_scripts)
        {
            ImGui::Text("%s", typeid(iter).name());
        }
    }
    else
    {
        ImGui::Text("Selected Object : NONE");

    }
    
    ImGui::End();
}