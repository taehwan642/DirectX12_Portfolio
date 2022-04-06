#include "pch.h"
#include "ImGuiManager.h"
#ifdef TOOL
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Transform.h"
#include "TransformComponent.h"
#include "MeshRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "ParticleSystem.h"
#include "Terrain.h"
#include "BaseCollider.h"
#include "Animator.h"
#include "Material.h"
#include "Shader.h"
#include "SphereCollider.h"
#include "CubeCollider.h"
#include "Resources.h"
#include "JsonManager.h"
#include "MeshCollider.h"
#include "MeshData.h"
#include "Visualizer.h"

#include "MonoBehaviour.h"
#include "GameManagerScript.h"
#include "TerrainScript.h"
#include "TestCameraScript.h"
#include "TestDragon.h"
#include "Sea.h"
#include "EnemyBullet.h"
#include "EffectManagerScript.h"
#include "Effect.h"

ImGuiManager::ImGuiManager(HWND hwnd, std::shared_ptr<Device> device)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc = {};
    desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    desc.NumDescriptors = 1;
    desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    if (device->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_srvDescHeap)) != S_OK)
    {
        return;
    }
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
    RenderResources();
    RenderDragAndDrop();
    ImGui::ShowDemoWindow();

    ImGui::Render();
}

void ImGuiManager::SetPipeline(std::shared_ptr<GraphicsCommandQueue> cmdqueue)
{
    ID3D12DescriptorHeap* heaps[] = { _srvDescHeap.Get() };
    cmdqueue->GetGraphicsCmdList()->SetDescriptorHeaps(1, heaps);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdqueue->GetGraphicsCmdList().Get());
}

void ImGuiManager::DragAndDrop(const std::wstring& path)
{
    // inputPath = ws2s(path.substr(0, path.size() - 5));
    inputPath = ws2s(path);
}

void ImGuiManager::RenderMeshData(std::shared_ptr<Mesh> mesh)
{
    ImGui::Text("Mesh VertexCount : %d", mesh->_vertexCount);
    ImGui::Text("Mesh SubsetCount : %d", mesh->GetSubsetCount());
}

void ImGuiManager::RenderMaterialData(int materialIndex, std::shared_ptr<Material> material)
{
    char buffer[256];
    sprintf_s(buffer, "Material Index : %d", materialIndex);

    if (ImGui::BeginMenu(buffer))
    {
        if (ImGui::BeginMenu("Textures"))
        {
            int item_current_idx[MATERIAL_ARG_COUNT]{};

            std::vector<std::string> stringVec;
            stringVec.push_back("NONE");
            int value = 1; // stringVec의 size는 이 때 1이니까.
            for (auto& iter : GET_SINGLE(Resources)->_resources[static_cast<int>(OBJECT_TYPE::TEXTURE)])
            {
                for (int i = 0; i < MATERIAL_ARG_COUNT; ++i)
                {
                    if (material->_textures[i] == nullptr)
                        continue;
                    if (material->_textures[i] == iter.second)
                    {
                        item_current_idx[i] = value;
                    }
                }
                stringVec.push_back(ws2s(iter.first));
                ++value;
            }

            for (int i = 0; i < MATERIAL_ARG_COUNT; ++i)
            {
                std::string combo_preview_value = stringVec[item_current_idx[i]];  // Pass in the preview value visible before opening the combo (it could be anything)
                std::string comboName = "Selected Texture " + std::to_string(i);
                if (ImGui::BeginCombo(comboName.c_str(), combo_preview_value.c_str()))
                {
                    for (int n = 0; n < stringVec.size(); n++)
                    {
                        const bool is_selected = (item_current_idx[i] == n);
                        if (ImGui::Selectable(stringVec[n].c_str(), is_selected))
                        {
                            item_current_idx[i] = n;
                            if (n != 0)
                            {
                                std::shared_ptr<Object> obj = GET_SINGLE(Resources)->_resources[static_cast<int>(OBJECT_TYPE::TEXTURE)].find(s2ws(stringVec[n]))->second;
                                material->_textures[i] = std::static_pointer_cast<Texture>(obj);
                                material->_params.texOnParams[i] = 1;
                            }
                            else
                            {
                                material->_params.texOnParams[i] = 0;
                            }
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }
            ImGui::EndMenu();
        }
            

        ImGui::InputInt4("Int Params", material->_params.intParams.data());
        ImGui::InputFloat4("Float Params", material->_params.floatParams.data());
        ImGui::InputInt4("Texture On Params", material->_params.texOnParams.data());

        if (ImGui::BeginMenu("Vector2"))
        {
            ImGui::InputFloat2("0", reinterpret_cast<float*>(&material->_params.vec2Params[0]));
            ImGui::InputFloat2("1", reinterpret_cast<float*>(&material->_params.vec2Params[1]));
            ImGui::InputFloat2("2", reinterpret_cast<float*>(&material->_params.vec2Params[2]));
            ImGui::InputFloat2("3", reinterpret_cast<float*>(&material->_params.vec2Params[3]));
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Vector4"))
        {
            ImGui::InputFloat4("0", reinterpret_cast<float*>(&material->_params.vec4Params[0]));
            ImGui::InputFloat4("1", reinterpret_cast<float*>(&material->_params.vec4Params[1]));
            ImGui::InputFloat4("2", reinterpret_cast<float*>(&material->_params.vec4Params[2]));
            ImGui::InputFloat4("3", reinterpret_cast<float*>(&material->_params.vec4Params[3]));
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Matrix"))
        {
            if (ImGui::BeginMenu("Matrix1"))
            {
                ImGui::Text("%f, %f, %f, %f", 
                    material->_params.matrixParams[0]._11,
                    material->_params.matrixParams[0]._12,
                    material->_params.matrixParams[0]._13,
                    material->_params.matrixParams[0]._14);
                ImGui::Text("%f, %f, %f, %f", 
                    material->_params.matrixParams[0]._21,
                    material->_params.matrixParams[0]._22,
                    material->_params.matrixParams[0]._23,
                    material->_params.matrixParams[0]._24);
                ImGui::Text("%f, %f, %f, %f", 
                    material->_params.matrixParams[0]._31,
                    material->_params.matrixParams[0]._32,
                    material->_params.matrixParams[0]._33,
                    material->_params.matrixParams[0]._34);
                ImGui::Text("%f, %f, %f, %f", 
                    material->_params.matrixParams[0]._41,
                    material->_params.matrixParams[0]._42,
                    material->_params.matrixParams[0]._43,
                    material->_params.matrixParams[0]._44);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Matrix2"))
            {
                ImGui::Text("%f, %f, %f, %f", 
                    material->_params.matrixParams[1]._11,
                    material->_params.matrixParams[1]._12,
                    material->_params.matrixParams[1]._13,
                    material->_params.matrixParams[1]._14);
                ImGui::Text("%f, %f, %f, %f",      
                    material->_params.matrixParams[1]._21,
                    material->_params.matrixParams[1]._22,
                    material->_params.matrixParams[1]._23,
                    material->_params.matrixParams[1]._24);
                ImGui::Text("%f, %f, %f, %f",      
                    material->_params.matrixParams[1]._31,
                    material->_params.matrixParams[1]._32,
                    material->_params.matrixParams[1]._33,
                    material->_params.matrixParams[1]._34);
                ImGui::Text("%f, %f, %f, %f",      
                    material->_params.matrixParams[1]._41,
                    material->_params.matrixParams[1]._42,
                    material->_params.matrixParams[1]._43,
                    material->_params.matrixParams[1]._44);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Matrix3"))
            {
                ImGui::Text("%f, %f, %f, %f", 
                    material->_params.matrixParams[2]._11,
                    material->_params.matrixParams[2]._12,
                    material->_params.matrixParams[2]._13,
                    material->_params.matrixParams[2]._14);
                ImGui::Text("%f, %f, %f, %f",      
                    material->_params.matrixParams[2]._21,
                    material->_params.matrixParams[2]._22,
                    material->_params.matrixParams[2]._23,
                    material->_params.matrixParams[2]._24);
                ImGui::Text("%f, %f, %f, %f",      
                    material->_params.matrixParams[2]._31,
                    material->_params.matrixParams[2]._32,
                    material->_params.matrixParams[2]._33,
                    material->_params.matrixParams[2]._34);
                ImGui::Text("%f, %f, %f, %f",      
                    material->_params.matrixParams[2]._41,
                    material->_params.matrixParams[2]._42,
                    material->_params.matrixParams[2]._43,
                    material->_params.matrixParams[2]._44);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Matrix4"))
            {
                ImGui::Text("%f, %f, %f, %f", 
                    material->_params.matrixParams[3]._11,
                    material->_params.matrixParams[3]._12,
                    material->_params.matrixParams[3]._13,
                    material->_params.matrixParams[3]._14);
                ImGui::Text("%f, %f, %f, %f",      
                    material->_params.matrixParams[3]._21,
                    material->_params.matrixParams[3]._22,
                    material->_params.matrixParams[3]._23,
                    material->_params.matrixParams[3]._24);
                ImGui::Text("%f, %f, %f, %f",      
                    material->_params.matrixParams[3]._31,
                    material->_params.matrixParams[3]._32,
                    material->_params.matrixParams[3]._33,
                    material->_params.matrixParams[3]._34);
                ImGui::Text("%f, %f, %f, %f",      
                    material->_params.matrixParams[3]._41,
                    material->_params.matrixParams[3]._42,
                    material->_params.matrixParams[3]._43,
                    material->_params.matrixParams[3]._44);
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Shader"))
        {
            int item_current_idx{};

            std::vector<std::string> stringVec;
            int value = 0; // stringVec의 size는 이 때 1이니까.

            for (auto& iter : GET_SINGLE(Resources)->_resources[static_cast<int>(OBJECT_TYPE::SHADER)])
            {
                if (material->_shader == iter.second)
                {
                    item_current_idx = value;
                }
                stringVec.push_back(ws2s(iter.first));
                ++value;
            }

            std::string combo_preview_value = stringVec[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
            std::string comboName = "Selected Material";
            if (ImGui::BeginCombo(comboName.c_str(), combo_preview_value.c_str()))
            {
                for (int n = 0; n < stringVec.size(); n++)
                {
                    const bool is_selected = (item_current_idx == n);
                    if (ImGui::Selectable(stringVec[n].c_str(), is_selected))
                    {
                        item_current_idx = n;
                        std::shared_ptr<Object> obj = GET_SINGLE(Resources)->_resources[static_cast<int>(OBJECT_TYPE::SHADER)].find(s2ws(stringVec[n]))->second;
                        material->_shader = std::static_pointer_cast<Shader>(obj);
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // ShaderInfo 출력
            ImGui::Text("Shader Info :");
            ImGui::SameLine();
            switch (material->GetShader()->GetShaderType())
            {
            case SHADER_TYPE::DEFERRED:
                ImGui::Text("Deffered");
                break;
            case SHADER_TYPE::FORWARD:
                ImGui::Text("Forward");
                break;
            case SHADER_TYPE::LIGHTING:
                ImGui::Text("Lighting");
                break;
            case SHADER_TYPE::PARTICLE:
                ImGui::Text("Particle");
                break;
            case SHADER_TYPE::COMPUTE:
                ImGui::Text("Compute");
                break;
            case SHADER_TYPE::SHADOW:
                ImGui::Text("Shadow");
                break;
            }

            if (ImGui::BeginMenu("Shader Pipeline"))
            {
                if (material->GetShader()->_vsBlob != nullptr)
                    ImGui::Text("Vertex Shader");
                if (material->GetShader()->_hsBlob != nullptr)
                    ImGui::Text("Hull Shader");
                if (material->GetShader()->_dsBlob != nullptr)
                    ImGui::Text("Domain Shader");
                if (material->GetShader()->_gsBlob != nullptr)
                    ImGui::Text("Geometry Shader");
                if (material->GetShader()->_psBlob != nullptr)
                    ImGui::Text("Pixel Shader");
                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenu();
    }
}

void ImGuiManager::RenderCameraData(std::shared_ptr<Camera> camera)
{
    {
        std::vector<std::string> projTypes;
        projTypes.push_back("PERSPECTIVE");
        projTypes.push_back("ORTHOGRAPHIC");

        std::string combo_preview_value = projTypes[static_cast<int>(camera->GetProjectionType())];  // Pass in the preview value visible before opening the combo (it could be anything)
        // Layer 출력
        std::string comboName = "Projection Type";
    
        if (ImGui::BeginCombo(comboName.c_str(), combo_preview_value.c_str()))
        {
            for (int n = 0; n < projTypes.size(); n++)
            {
                const bool is_selected = (static_cast<int>(camera->GetProjectionType()) == n);
                if (ImGui::Selectable(projTypes[n].c_str(), is_selected))
                {
                    camera->_type = static_cast<PROJECTION_TYPE>(n);
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }
    
    ImGui::InputFloat("Near", &camera->_near);
    ImGui::InputFloat("Far", &camera->_far);
    ImGui::InputFloat("Fov", &camera->_fov);
    ImGui::InputFloat("Scale", &camera->_scale);
    ImGui::InputFloat("Width", &camera->_width);
    ImGui::InputFloat("Height", &camera->_height);

    static int mode = 0;
    if (ImGui::RadioButton("Shoot all but one layer", mode == 0)) { mode = 0; } ImGui::SameLine();
    if (ImGui::RadioButton("Shoot one layer", mode == 1)) { mode = 1; }

    // Layer 출력
    static uint8 layerIDX = 0;

    std::string combo_preview_value = ws2s(GET_SINGLE(SceneManager)->IndexToLayerName(layerIDX).c_str()).c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
    std::string comboName = "Layer";

    if (ImGui::BeginCombo(comboName.c_str(), combo_preview_value.c_str()))
    {
        std::array<std::wstring, MAX_LAYER>& layerArray = GET_SINGLE(SceneManager)->_layerNames;
        for (int n = 0; n < layerArray.size(); n++)
        {
            if (layerArray[n].empty())
                break;
            const bool is_selected = (layerIDX == n);
            if (ImGui::Selectable(ws2s(layerArray[n].c_str()).c_str(), is_selected))
            {
                layerIDX = n;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    if (ImGui::Button("Create LayerMask"))
    {
        uint8 layerIndex = layerIDX;
        if (mode == 0)
        {
            camera->SetCullingMaskLayerOnOff(layerIndex, true);
        }
        else if (mode == 1)
        {
            camera->SetCullingMaskAll();
            camera->SetCullingMaskLayerOnOff(layerIndex, false);
        }
    }
}

void ImGuiManager::RenderSphereColliderData(std::shared_ptr<SphereCollider> sphereCollider)
{
    ImGui::Text("Type : Sphere");

    ImGui::Text("Radius : %f", sphereCollider->_boundingSphere.Radius);
    ImGui::Text("Center : %f %f %f", sphereCollider->_boundingSphere.Center.x, sphereCollider->_boundingSphere.Center.y, sphereCollider->_boundingSphere.Center.z);
}

void ImGuiManager::RenderCubeColliderData(std::shared_ptr<CubeCollider> cubeCollider)
{
    ImGui::Text("Type : Cube");

    ImGui::Text("Center : %f %f %f", cubeCollider->_boundingBox.Center.x, cubeCollider->_boundingBox.Center.y, cubeCollider->_boundingBox.Center.z);
    ImGui::Text("Scale : %f %f %f", 
        cubeCollider->_boundingBox.Extents.x, 
        cubeCollider->_boundingBox.Extents.y,
        cubeCollider->_boundingBox.Extents.z);
}

void ImGuiManager::RenderMeshColliderData(std::shared_ptr<MeshCollider> meshCollider)
{
    ImGui::Text("Type : Mesh");

    ImGui::Text("Triangle Count : %d", meshCollider->_triCount);
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

    static std::string input;
    // Text 적을 수 있게 해야함.
    ImGui::InputText("FileName", const_cast<char*>(input.c_str()), 64);
    if (ImGui::Button("Save"))
    {
        std::string inputText(input.c_str());
        std::string path = std::string("../Output/") + inputText;
        GET_SINGLE(JsonManager)->SaveScene(path.c_str(), GET_SINGLE(SceneManager)->GetActiveScene());
    }
    ImGui::SameLine();
    if (ImGui::Button("Load"))
    {
        std::string inputText(input.c_str());
        std::string path = std::string("../Output/") + inputText;
        GET_SINGLE(SceneManager)->LoadScene(s2ws(path).c_str());
    }
    ImGui::SameLine();
    if (ImGui::Button("Set Camera Same"))
    {
        // TOOL CAMERA, Main Camera 검색
        std::shared_ptr<GameObject> toolCamera = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"ToolCamera");
        std::shared_ptr<GameObject> mainCamera = GET_SINGLE(SceneManager)->GetActiveScene()->FindGameObject(L"Main_Camera");

        std::shared_ptr<TransformComponent> toolCameraTransform = toolCamera->GetTransform();
        std::shared_ptr<TransformComponent> mainCameraTransform = mainCamera->GetTransform();
        mainCameraTransform->SetWorldPosition(toolCameraTransform->GetWorldPosition());
        mainCameraTransform->SetWorldRotation(toolCameraTransform->GetWorldRotation());
        mainCameraTransform->SetWorldScale(toolCameraTransform->GetWorldScale());

        std::shared_ptr<Camera> toolCameraCom = toolCamera->GetCamera();
        std::shared_ptr<Camera> mainCameraCom = mainCamera->GetCamera();
        mainCameraCom->_cullingMask = toolCameraCom->_cullingMask;
        mainCameraCom->_far = toolCameraCom->_far;
        mainCameraCom->_near = toolCameraCom->_near;
        mainCameraCom->_width = toolCameraCom->_width;
        mainCameraCom->_height = toolCameraCom->_height;
        mainCameraCom->_type = toolCameraCom->_type;
    }
    ImGui::Separator();

    static int val = 0;
    if (ImGui::InputInt("Scene Index", &val))
    {
        GET_SINGLE(SceneManager)->SetScene(val);
    }

    ImGui::Separator();
    if (GET_SINGLE(SceneManager)->GetActiveScene() == nullptr)
    {
        ImGui::End();
        return;
    }

    const std::vector<std::shared_ptr<GameObject>>& vec = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

    int tempName = 0;
    for (auto& iter : vec)
    {
        // weak_ptr에는 nullptr이 들어가지 못하기 때문에, shared_ptr을 끌고와 체크.
        if (iter->GetTransform()->GetParent().lock() != nullptr)
            continue;

        ImGui::PushID(iter->_hash);
        std::string tempString = std::to_string(tempName);
        ++tempName;
        if (iter->GetName().empty() == false)
            tempString = std::string(iter->GetName().begin(), iter->GetName().end());

        bool treeopen = ImGui::TreeNodeEx(tempString.c_str(), ImGuiTreeNodeFlags_AllowItemOverlap);
        ImGui::SameLine();
        if (ImGui::Button("+")) 
            _currentGameObject = iter;
        // Our buttons are both drag sources and drag targets here!
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
        {
            // Set payload to carry the index of our item (could be anything)
            ImGui::SetDragDropPayload("DND_DEMO_CELL", &iter->_hash, sizeof(size_t));

            // Display preview (could be anything, e.g. when dragging an image we could decide to display
            // the filename and a small preview of the image, etc.)
            ImGui::Text("Set Child %s", ws2s(iter->GetName().c_str()).c_str());
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
            {
                IM_ASSERT(payload->DataSize == sizeof(size_t));
                size_t payload_n = *(const size_t*)payload->Data;

                // find hash
                for (int i = 0; i < vec.size(); ++i)
                {
                    if (vec[i]->_hash == payload_n)
                    {
                        vec[i]->GetTransform()->SetParent(iter->GetTransform());
                    }
                }

            }
            ImGui::EndDragDropTarget();
        }

        if (treeopen)
        {
            
            // TODO : Iter의 Child 재귀
            for (int i = 0; i < iter->GetTransform()->_childVector.size(); ++i)
            {
                RenderChild(iter, i);
            }

            ImGui::TreePop();
        }
        
        ImGui::PopID();
    }
    ImGui::Separator();

    if (ImGui::Button("Add GameObject"))
    {
        std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
        _currentGameObject = gameObject;
        _currentGameObject->AddComponent(std::make_shared<TransformComponent>());
        GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(gameObject);
    }

    ImGui::End();
}

void ImGuiManager::RenderInspector()
{
    ImGui::Begin("Inspector");

    if (_currentGameObject != nullptr)
    {
        static std::string textinput;
        // Text 적을 수 있게 해야함.
        ImGui::InputText("FileName", const_cast<char*>(textinput.c_str()), 64);
        if (ImGui::Button("Save Prefab"))
        {
            std::string inputText(textinput.c_str());
            std::string path = std::string("../Resources/FBX/") + inputText;

            std::shared_ptr<Scene> sceneOnlyForSave = std::make_shared<Scene>();

            // 현재 게임오브젝트부터 자식들까지 모두 Scene에 담기
            AddSceneChild(sceneOnlyForSave, _currentGameObject);

            // 저장
            std::string finalPath = path + "_Prefab";
            GET_SINGLE(JsonManager)->SaveScene(finalPath.c_str(), sceneOnlyForSave);
        }

        std::string input = ws2s(_currentGameObject->GetName()).c_str();
        // Text 적을 수 있게 해야함.
        if (ImGui::InputText("Name", const_cast<char*>(input.c_str()), 64))
        {
            _currentGameObject->SetName(s2ws(input).c_str());
        }

        std::string tempString = std::string(_currentGameObject->GetName().begin(), _currentGameObject->GetName().end());
        ImGui::Text("Selected Object : %s", tempString.c_str());

        std::string frustumCheckLabel = (_currentGameObject->_checkFrustum == true) ? "Press to disable FrustumCulling" : "Press to enable FrustumCulling";
        if (ImGui::Button(frustumCheckLabel.c_str()))
        {
            _currentGameObject->SetCheckFrustum(!_currentGameObject->_checkFrustum);
        }

        std::string shadowCheckLabel = (_currentGameObject->_static == true) ? "Press to enable Shadow" : "Press to disable Shadow";
        if (ImGui::Button(shadowCheckLabel.c_str()))
        {
            _currentGameObject->SetCheckFrustum(!_currentGameObject->_static);
        }

        std::string hashValue = std::to_string(_currentGameObject->_hash);
        ImGui::Text(("Hash Value : " + hashValue).c_str());

        ImGui::SameLine();
        if (ImGui::Button("Generate Hash"))
        {
            _currentGameObject->GenerateHash();
        }

        if (ImGui::Button("Active"))
        {
            _currentGameObject->SetActive(!_currentGameObject->GetActive());
        }
        ImGui::SameLine();
        if (ImGui::Button("Visualize Frustum Radius"))
        {
            _currentGameObject->_drawFrustumRaidusVisualizer = !_currentGameObject->_drawFrustumRaidusVisualizer;
        }
        ImGui::SameLine();
        ImGui::DragFloat("Frustum Radius", &_currentGameObject->_frustumCheckRadius);

        // Layer 출력
        std::string combo_preview_value = ws2s(GET_SINGLE(SceneManager)->IndexToLayerName(_currentGameObject->_layerIndex).c_str()).c_str();  // Pass in the preview value visible before opening the combo (it could be anything)
        std::string comboName = "Layer";
        if (ImGui::BeginCombo(comboName.c_str(), combo_preview_value.c_str()))
        {
            std::array<std::wstring, MAX_LAYER>& layerArray = GET_SINGLE(SceneManager)->_layerNames;
            for (int n = 0; n < layerArray.size(); n++)
            {
                if (layerArray[n].empty())
                    break;
                const bool is_selected = (_currentGameObject->_layerIndex == n);
                if (ImGui::Selectable(ws2s(layerArray[n].c_str()).c_str(), is_selected))
                {
                    _currentGameObject->_layerIndex = n;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // Component들 출력

        // TRANSFORM
        if (_currentGameObject->GetTransform() != nullptr)
        {
            // Scale, Transform, Rotation, 부모 출력
            if (ImGui::BeginMenu("Transform"))
            {
                std::shared_ptr<TransformComponent> trans = _currentGameObject->GetTransform();
                std::shared_ptr<Transform> worldTrans = trans->GetWorldTransform();
                std::shared_ptr<Transform> localTrans = trans->GetLocalTransform();

                if (ImGui::DragFloat3("World Position", reinterpret_cast<float*>(const_cast<Vec3*>(&worldTrans->_position))))
                    trans->UpdateLocal();
                if (ImGui::DragFloat3("World Rotation", reinterpret_cast<float*>(const_cast<Vec3*>(&worldTrans->_rotation)), 0.1f))
                    trans->UpdateLocal();
                if (ImGui::DragFloat3("World Scale", reinterpret_cast<float*>(const_cast<Vec3*>(&worldTrans->_scale))))
                    trans->UpdateLocal();

                if (ImGui::DragFloat3("Local Position", reinterpret_cast<float*>(const_cast<Vec3*>(&localTrans->_position))))
                    trans->UpdateWorld();
                if (ImGui::DragFloat3("Local Rotation", reinterpret_cast<float*>(const_cast<Vec3*>(&localTrans->_rotation)), 0.1f))
                    trans->UpdateWorld();
                if (ImGui::DragFloat3("Local Scale", reinterpret_cast<float*>(const_cast<Vec3*>(&localTrans->_scale))))
                    trans->UpdateWorld();



                if (trans->_parent.lock() != nullptr)
                {
                    std::string txt = ws2s(trans->_parent.lock()->GetGameObject()->GetName());
                    ImGui::Text(txt.c_str());

                    if (ImGui::Button("Remove Parent"))
                    {
                        trans->RemoveParent();
                    }
                }

                ImGui::EndMenu();
            }
        }

	    // MESH_RENDERER
        if (_currentGameObject->GetMeshRenderer() != nullptr)
        {
            // Material, Mesh, Shader 출력
            if (ImGui::BeginMenu("MeshRenderer"))
            {
                std::shared_ptr<MeshRenderer> mr = _currentGameObject->GetMeshRenderer();
                
                if (ImGui::BeginMenu("Mesh"))
                {
                    int item_current_idx{};

                    std::vector<std::string> stringVec;
                    stringVec.push_back("NONE");
                    int value = 1; // stringVec의 size는 이 때 1이니까.
                    for (auto& iter : GET_SINGLE(Resources)->_resources[static_cast<int>(OBJECT_TYPE::MESH)])
                    {
                        if (mr->_mesh == iter.second)
                        {
                            item_current_idx = value;
                        }
                        stringVec.push_back(ws2s(iter.first));
                        ++value;
                    }

                    std::string combo_preview_value = stringVec[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
                    std::string comboName = "Selected Mesh";
                    if (ImGui::BeginCombo(comboName.c_str(), combo_preview_value.c_str()))
                    {
                        for (int n = 0; n < stringVec.size(); n++)
                        {
                            const bool is_selected = (item_current_idx == n);
                            if (ImGui::Selectable(stringVec[n].c_str(), is_selected))
                            {
                                item_current_idx = n;
                                if (n != 0)
                                {
                                    std::shared_ptr<Object> obj = GET_SINGLE(Resources)->_resources[static_cast<int>(OBJECT_TYPE::MESH)].find(s2ws(stringVec[n]))->second;
                                    mr->_mesh = std::static_pointer_cast<Mesh>(obj);
                                }
                                else
                                {
                                    mr->_mesh = nullptr;
                                }
                            }

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    if (mr->GetMesh())
                        RenderMeshData(mr->GetMesh());

                    ImGui::EndMenu();
                }

                int n = 0;
                for (auto& iterMaterial : mr->_materials)
                {
                    int item_current_idx{};

                    std::vector<std::string> stringVec;
                    stringVec.push_back("NONE");
                    int value = 1; // stringVec의 size는 이 때 1이니까.
                    for (auto& iter : GET_SINGLE(Resources)->_resources[static_cast<int>(OBJECT_TYPE::MATERIAL)])
                    {
                        if (iterMaterial == iter.second)
                        {
                            item_current_idx = value;
                        }
                        stringVec.push_back(ws2s(iter.first));
                        ++value;
                    }

                    std::string combo_preview_value = stringVec[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
                    std::string comboName = "Selected Material";
                    if (ImGui::BeginCombo(comboName.c_str(), combo_preview_value.c_str()))
                    {
                        for (int n = 0; n < stringVec.size(); n++)
                        {
                            const bool is_selected = (item_current_idx == n);
                            if (ImGui::Selectable(stringVec[n].c_str(), is_selected))
                            {
                                item_current_idx = n;
                                if (n != 0)
                                {
                                    std::shared_ptr<Object> obj = GET_SINGLE(Resources)->_resources[static_cast<int>(OBJECT_TYPE::MATERIAL)].find(s2ws(stringVec[n]))->second;
                                    iterMaterial = std::static_pointer_cast<Material>(obj);
                                }
                                else
                                {
                                    iterMaterial = nullptr;
                                }
                            }

                            if (is_selected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    if (iterMaterial->GetShader() != nullptr)
                    {
                        if (iterMaterial->GetShader()->GetShaderType() == SHADER_TYPE::DEFERRED)
                            ImGui::Text("Set Material's int 0 param to 1 for Instancing");

                        RenderMaterialData(n, iterMaterial);
                    }
                   
                    ++n;
                }

                if (ImGui::Button("Add Material"))
                {
                    mr->_materials.push_back(std::make_shared<Material>());
                }

                if (ImGui::Button("Delete Component"))
                {
                    _currentGameObject->_meshRenderer = nullptr;
                }

                ImGui::EndMenu();
            }
        }

	    // CAMERA
        if (_currentGameObject->GetCamera() != nullptr)
        {
            // Transform, Rotation, Fov, width, height, orthographic/perspective 출력
            if (ImGui::BeginMenu("Camera"))
            {
                std::shared_ptr<Camera> camera = _currentGameObject->GetCamera();
                
                RenderCameraData(camera);
                
                if (ImGui::Button("Delete Component"))
                {
                    _currentGameObject->_camera = nullptr;
                }

                ImGui::EndMenu();
            }
        }

	    // LIGHT
        if (_currentGameObject->GetLight() != nullptr)
        {
            // light type 출력, light position, rotation 출력
            if (ImGui::BeginMenu("Light"))
            {
                std::shared_ptr<Light> light = _currentGameObject->GetLight();

                int item_current_idx = static_cast<int>(light->GetLightType());

                std::vector<std::string> stringVec;
                stringVec.push_back("Directional Light");
                stringVec.push_back("Point Light");
                stringVec.push_back("Spot Light");

                std::string combo_preview_value = stringVec[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
                std::string comboName = "Light Type";
                if (ImGui::BeginCombo(comboName.c_str(), combo_preview_value.c_str()))
                {
                    for (int n = 0; n < stringVec.size(); n++)
                    {
                        const bool is_selected = (item_current_idx == n);
                        if (ImGui::Selectable(stringVec[n].c_str(), is_selected))
                        {
                            item_current_idx = n;
                            light->SetLightType(static_cast<LIGHT_TYPE>(item_current_idx));
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }


                std::string meshType = "";
                switch (light->GetLightType())
                {
                case LIGHT_TYPE::DIRECTIONAL_LIGHT:
                    ImGui::Text("Directional Light");
                    meshType = "Rectangle";
                    break;
                case LIGHT_TYPE::POINT_LIGHT:
                    ImGui::Text("Point Light");
                    meshType = "Sphere";
                    break;
                case LIGHT_TYPE::SPOT_LIGHT:
                    ImGui::Text("Spot Light");
                    meshType = "Sphere";
                    break;
                }

                ImGui::Text("LightIndex : %d", light->_lightIndex);
                if (ImGui::BeginMenu("LightInfo"))
                {
                    if (ImGui::BeginMenu("LightColor"))
                    {
                        ImGui::DragFloat4("Diffuse", reinterpret_cast<float*>(&light->_lightInfo.color.diffuse), 0.1f);

                        ImGui::DragFloat4("Ambient", reinterpret_cast<float*>(&light->_lightInfo.color.ambient), 0.1f);

                        ImGui::DragFloat4("Specular", reinterpret_cast<float*>(&light->_lightInfo.color.specular), 0.1f);

                        ImGui::EndMenu();
                    }

                    ImGui::Text("Position : %f %f %f", light->_lightInfo.position.x, light->_lightInfo.position.y, light->_lightInfo.position.z);

                    static float direction[3];
                    direction[0] = light->_lightInfo.direction.x;
                    direction[1] = light->_lightInfo.direction.y;
                    direction[2] = light->_lightInfo.direction.z;

                    ImGui::DragFloat3("Direction", direction, 0.1f);
                    // ONLY FOR DEBUG
                    light->SetLightDirection(Vec3(direction[0], direction[1], direction[2]));

                    ImGui::DragFloat("Range", &light->_lightInfo.range, 0.1f);
                    ImGui::DragFloat("Angle", &light->_lightInfo.angle, 0.1f);

                    ImGui::EndMenu();
                }

                // Material
                if (ImGui::BeginMenu("Light Material"))
                {
                    RenderMaterialData(0, light->_lightMaterial);
                    ImGui::EndMenu();
                }

                // Camera
                if (ImGui::BeginMenu("Shadow Camera"))
                {
                    RenderCameraData(light->_shadowCamera->GetCamera());
                    ImGui::EndMenu();
                }

                // Mesh
                ImGui::Text("MeshType : %s", meshType.c_str());
                RenderMeshData(light->_volumeMesh);

                if (ImGui::Button("Delete Component"))
                {
                    _currentGameObject->_light = nullptr;
                }

                ImGui::EndMenu();
            }
        }

	    // PARTICLE_SYSTEM
        if (_currentGameObject->GetParticleSystem() != nullptr)
        {
            if (ImGui::BeginMenu("Particle System"))
            {
                std::shared_ptr<ParticleSystem> particle = _currentGameObject->GetParticleSystem();

                int item_current_idx = static_cast<int>(particle->_mode);

                std::vector<std::string> stringVec;
                stringVec.push_back("RandomPos_RandomDir");
                stringVec.push_back("RandomPos_SetDir");

                std::string combo_preview_value = stringVec[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
                std::string comboName = "Particle Mode";
                if (ImGui::BeginCombo(comboName.c_str(), combo_preview_value.c_str()))
                {
                    for (int n = 0; n < stringVec.size(); n++)
                    {
                        const bool is_selected = (item_current_idx == n);
                        if (ImGui::Selectable(stringVec[n].c_str(), is_selected))
                        {
                            item_current_idx = n;
                            particle->_mode = static_cast<ParticleMode>(item_current_idx);
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }


                ImGui::InputInt("Max Particle", reinterpret_cast<int*>(&particle->_maxParticle));
                ImGui::InputFloat("Create Interval", &particle->_createInterval);
                ImGui::Text("Accumulated Time : %f", &particle->_accTime);
                ImGui::InputFloat("Min LifeTime", &particle->_minLifeTime);
                ImGui::InputFloat("Max LifeTime", &particle->_maxLifeTime);
                ImGui::InputFloat("Min Speed", &particle->_minSpeed);
                ImGui::InputFloat("Max Speed", &particle->_maxSpeed);
                ImGui::InputFloat("Start Scale", &particle->_startScale);
                ImGui::InputFloat("End Scale", &particle->_endScale);

                ImGui::DragFloat3("Range", reinterpret_cast<float*>(&particle->_ranges), 1.f);
                ImGui::DragFloat3("Direction", reinterpret_cast<float*>(&particle->_direction), 1.f);

                RenderMeshData(particle->_mesh);
                if (ImGui::BeginMenu("Particle Material"))
                {
                    RenderMaterialData(0, particle->_material);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Compute Material"))
                {
                    RenderMaterialData(0, particle->_computeMaterial);
                    ImGui::EndMenu();
                }

                if (ImGui::Button("Delete Component"))
                {
                    _currentGameObject->_particleSystem = nullptr;
                }

                ImGui::EndMenu();
            }
        }

	    // TERRAIN
        if (_currentGameObject->GetTerrain() != nullptr)
        {
            // width, height, 사용된 heightmap 가능하다면 출력
            if (ImGui::BeginMenu("Terrain"))
            {
                std::shared_ptr<Terrain> terrain = _currentGameObject->GetTerrain();
                ImGui::Text("Vertex Num X : %d", terrain->_sizeX);
                ImGui::Text("Vertex Num Z : %d", terrain->_sizeZ);
                ImGui::Text("Max Tesselation : %f", terrain->_maxTesselation);

                if (ImGui::Button("Delete Component"))
                {
                    _currentGameObject->_terrain = nullptr;
                }

                ImGui::EndMenu();
            }
        }

	    // COLLIDER
        if (_currentGameObject->GetCollider() != nullptr)
        {
            // Collider 렌더할지 출력, Radius, Center 등 출력
            if (ImGui::BeginMenu("Collider"))
            {
                std::shared_ptr<BaseCollider> collider = _currentGameObject->GetCollider();
                
                switch (collider->_colliderType)
                {
                case ColliderType::Sphere:
                {
                    std::shared_ptr<SphereCollider> sphereCollider = std::static_pointer_cast<SphereCollider>(collider);
                    RenderSphereColliderData(sphereCollider);
                }
                    break;
                case ColliderType::Cube:
                {
                    std::shared_ptr<CubeCollider> cubeCollider = std::static_pointer_cast<CubeCollider>(collider);
                    RenderCubeColliderData(cubeCollider);
                }

                case ColliderType::Mesh:
                {
                    std::shared_ptr<MeshCollider> meshCollider = std::static_pointer_cast<MeshCollider>(collider);
                    RenderMeshColliderData(meshCollider);
                }
                    break;
                }
                
                std::string s = (collider->_draw == true ? "Press To Turn Off DrawMode" : "Press To Turn On DrawMode");
                if (ImGui::Button(s.c_str()))
                {
                    collider->_draw = !collider->_draw;
                }
                ImGui::SameLine();
                ImGui::DragFloat3("Collider Scale", reinterpret_cast<float*>(const_cast<Vec3*>(&collider->_colliderVisualizer->_transform->_worldTransform->_scale)));
                
                if (ImGui::CollapsingHeader("ColliderMeshData"))
                {
                    if (ImGui::BeginMenu("Mesh"))
                    {
                        RenderMeshData(collider->_colliderVisualizer->_meshRenderer->_mesh);
                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Material"))
                    {
                        RenderMaterialData(0, collider->_colliderVisualizer->_meshRenderer->_materials[0]);
                        ImGui::EndMenu();
                    }
                }

                if (ImGui::Button("Delete Component"))
                {
                    _currentGameObject->_baseCollider = nullptr;
                }

                ImGui::EndMenu();
            }
        }

	    // ANIMATOR
        if (_currentGameObject->GetAnimator() != nullptr)
        {
            if (ImGui::BeginMenu("Animator"))
            {
                std::shared_ptr<Animator> animator = _currentGameObject->GetAnimator();

                if (ImGui::Button("Play/Pause"))
                {
                    animator->paused = !animator->paused;
                }


                int item_current_idx{};

                std::vector<std::string> stringVec;

                for (int i = 0; i < animator->_animClips->size(); ++i)
                {
                    if (animator->_clipIndex == i)
                    {
                        item_current_idx = i;
                    }
                    stringVec.push_back((*animator->_animClips)[i].animName);
                }

                std::string combo_preview_value = stringVec[item_current_idx];  // Pass in the preview value visible before opening the combo (it could be anything)
                std::string comboName = "Selected Animation";
                if (ImGui::BeginCombo(comboName.c_str(), combo_preview_value.c_str()))
                {
                    for (int n = 0; n < stringVec.size(); n++)
                    {
                        const bool is_selected = (item_current_idx == n);
                        if (ImGui::Selectable(stringVec[n].c_str(), is_selected))
                        {
                            animator->_clipIndex = n;
                            animator->Play(animator->_clipIndex);
                            animator->_frame = 0;
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }

                const AnimClipInfo& animClip = animator->_animClips->at(animator->_clipIndex);
                ImGui::SliderInt("Frame", &animator->_frame, 0, animClip.frameCount - 1);
                ImGui::Text("Frame Ratio : %d", animator->_frameRatio);

                if (ImGui::Button("Delete Component"))
                {
                    _currentGameObject->_animator = nullptr;
                }

                ImGui::EndMenu();
            }
        }

        // MONOBEHAVIOUR
        for (auto& iter : _currentGameObject->_scripts)
        {
            ImGui::PushID(ws2s(iter->_className).c_str());

            ImGui::Text("%s", ws2s(iter->_className).c_str());

            // Our buttons are both drag sources and drag targets here!
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
            {
                // Set payload to carry the index of our item (could be anything)
                ImGui::SetDragDropPayload("DND_DEMO_CELL", NULL, 0);

                // Display preview (could be anything, e.g. when dragging an image we could decide to display
                // the filename and a small preview of the image, etc.)
                ImGui::Text("Set Child %s", ws2s(iter->GetName().c_str()).c_str());
                ImGui::EndDragDropSource();
            }
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
                {
                    IM_ASSERT(payload->DataSize == sizeof(size_t));
                    size_t payload_n = *(const size_t*)payload->Data;

                    // find hash
                    iter->DragAndDrop(payload_n);

                }
                ImGui::EndDragDropTarget();
            }
            ImGui::PopID();

        }

        if (ImGui::BeginMenu("AddComponent"))
        {
            if (_currentGameObject->GetMeshRenderer() == nullptr && ImGui::Button("MeshRenderer"))
            {
                _currentGameObject->AddComponent(std::make_shared<MeshRenderer>());
                std::shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
                _currentGameObject->GetMeshRenderer()->SetMaterial(material);
                std::shared_ptr<Mesh> mesh = GET_SINGLE(Resources)->LoadCubeMesh();
                _currentGameObject->GetMeshRenderer()->SetMesh(mesh);
            }

            if (_currentGameObject->GetMeshRenderer() == nullptr &&
                _currentGameObject->GetAnimator() == nullptr &&
                ImGui::Button("MeshData"))
            {
                // _currentGameObject->AddComponent(std::make_shared<MeshRenderer>());
            }

            if (_currentGameObject->GetCamera() == nullptr && ImGui::Button("Camera"))
            {
                GET_SINGLE(SceneManager)->GetActiveScene()->RemoveGameObject(_currentGameObject);
                _currentGameObject->AddComponent(std::make_shared<Camera>());
                GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(_currentGameObject);
            }

            if (_currentGameObject->GetLight() == nullptr && ImGui::Button("Light"))
            {
                GET_SINGLE(SceneManager)->GetActiveScene()->RemoveGameObject(_currentGameObject);
                _currentGameObject->AddComponent(std::make_shared<Light>());
                _currentGameObject->GetLight()->SetLightType(LIGHT_TYPE::DIRECTIONAL_LIGHT);
                GET_SINGLE(SceneManager)->GetActiveScene()->AddGameObject(_currentGameObject);
            }

            if (_currentGameObject->GetParticleSystem() == nullptr && ImGui::Button("Particle System"))
            {
                _currentGameObject->AddComponent(std::make_shared<ParticleSystem>());
            }

            if (_currentGameObject->GetTerrain() == nullptr && ImGui::Button("Terrain"))
            {
                _currentGameObject->AddComponent(std::make_shared<Terrain>());
                _currentGameObject->GetTerrain()->Init(64, 64);
            }

            if (ImGui::BeginMenu("Collider"))
            {
                if (_currentGameObject->GetCollider() == nullptr && ImGui::Button("Sphere Collider"))
                {
                    _currentGameObject->AddComponent(std::make_shared<SphereCollider>());
                }

                if (_currentGameObject->GetCollider() == nullptr && ImGui::Button("Cube Collider"))
                {
                    _currentGameObject->AddComponent(std::make_shared<CubeCollider>());
                }

                if (_currentGameObject->GetCollider() == nullptr && ImGui::Button("Mesh Collider"))
                {
                    std::shared_ptr<MeshCollider> mc = std::make_shared<MeshCollider>();
                    _currentGameObject->AddComponent(mc);
                    mc->Init();
                }
                ImGui::EndMenu();
            }
           

            if (_currentGameObject->GetAnimator() == nullptr && ImGui::Button("Animator"))
            {
                std::shared_ptr<Animator> animator = std::make_shared<Animator>();
                _currentGameObject->AddComponent(animator);
                animator->SetBones(_currentGameObject->_meshRenderer->_mesh->GetBones());
                animator->SetAnimClip(_currentGameObject->_meshRenderer->_mesh->GetAnimClip());
            }

            IMGUIADDMONOBEHAVIOUR(GameManagerScript);
            IMGUIADDMONOBEHAVIOUR(TerrainScript);
            IMGUIADDMONOBEHAVIOUR(TestCameraScript);
            IMGUIADDMONOBEHAVIOUR(TestDragon);
            IMGUIADDMONOBEHAVIOUR(Sea);
            IMGUIADDMONOBEHAVIOUR(EnemyBullet);
            IMGUIADDMONOBEHAVIOUR(EffectManagerScript);
            IMGUIADDMONOBEHAVIOUR(Effect);

            ImGui::EndMenu();
        }

        if (ImGui::Button("Delete GameObject"))
        {
            _currentGameObject->GetTransform()->RemoveParent();
            GET_SINGLE(SceneManager)->GetActiveScene()->RemoveGameObject(_currentGameObject);
            _currentGameObject = nullptr;
        }
    }
    else
    {
        ImGui::Text("Selected Object : NONE");
    }
    
    ImGui::End();
}

void ImGuiManager::RenderResources()
{
    ImGui::Begin("Resources");

    static std::string input;
    // Text 적을 수 있게 해야함.
    ImGui::InputText("FileName", const_cast<char*>(input.c_str()), 64);
    if (ImGui::Button("Resource to prefab (FBX)"))
    {
        std::string inputText(input.c_str());
        std::string path = std::string("../Resources/FBX/") + inputText;

        std::shared_ptr<Scene> sceneOnlyForSave = std::make_shared<Scene>();

        // MeshData 불러오기
        std::shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(s2ws(path.c_str()).c_str());

        // 불러온 MeshData Prefab으로 뽑기
        std::shared_ptr<GameObject> meshDataObject = std::make_shared<GameObject>();
        meshDataObject->SetName(s2ws(inputText.c_str()));
        meshDataObject->GenerateHash();
        sceneOnlyForSave->AddGameObject(meshDataObject);
        std::shared_ptr<GameObject> mesh_root = std::make_shared<GameObject>();
        mesh_root->SetName(L"mesh_root");
        mesh_root->GenerateHash();
        sceneOnlyForSave->AddGameObject(mesh_root);
        std::vector<std::shared_ptr<GameObject>> gameObjects = meshData->Instantiate();

        meshDataObject->AddComponent(std::make_shared<TransformComponent>());
        mesh_root->AddComponent(std::make_shared<TransformComponent>());

        mesh_root->GetTransform()->SetParent(meshDataObject->GetTransform());
        for (int i = 0; i < gameObjects.size(); ++i)
        {
            gameObjects[i]->GetTransform()->SetParent(mesh_root->GetTransform());
            sceneOnlyForSave->AddGameObject(gameObjects[i]);
        }

        std::string finalPath = path + "_Prefab";
        GET_SINGLE(JsonManager)->SaveScene(finalPath.c_str(), sceneOnlyForSave);
    }

    if (ImGui::Button("Get prefab to scene"))
    {
        int tempNum = 0;
        std::string inputText(input.c_str());
        std::shared_ptr<Scene> currentScene = GET_SINGLE(SceneManager)->GetActiveScene();

        // 씬에 같은 이름이 존재한다면? tempNum을 ++해서 다시 찾기.
        auto& vec = currentScene->GetGameObjects();
        auto iter = std::find_if(vec.begin(), vec.end(), [=](std::shared_ptr<GameObject> obj) {return obj->GetName() == s2ws(inputText + std::to_string(tempNum)); });
        while (iter != vec.end())
        {
            ++tempNum;
            iter = std::find_if(vec.begin(), vec.end(), [=](std::shared_ptr<GameObject> obj) {return obj->GetName() == s2ws(inputText + std::to_string(tempNum)); });
        }

        std::string path = std::string("../Resources/FBX/") + inputText + "_Prefab";

        std::shared_ptr<Scene> sceneOnlyForLoad = std::make_shared<Scene>();
        GET_SINGLE(JsonManager)->LoadScene(path.c_str(), sceneOnlyForLoad);

        // 불러온 임시 씬 속 프리팹 정보를 현재 씬에 넘겨주기
        for (auto& iter : sceneOnlyForLoad->GetGameObjects())
        {
            iter->SetName(iter->GetName() + std::to_wstring(tempNum));
            iter->GenerateHash();
            currentScene->AddGameObject(iter);
        }
    }

    ImGui::End();
}

void ImGuiManager::RenderDragAndDrop()
{
    ImGui::Begin("Drag and Drop");
    ImGui::Text(inputPath.c_str());

    if (ImGui::Button("Resource to prefab"))
    {

        std::shared_ptr<Scene> sceneOnlyForSave = std::make_shared<Scene>();
        
        // MeshData 불러오기
        std::shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(s2ws(inputPath.c_str()).c_str());

        // 주소에서 obj 이름만 가져오려면, 맨 마지막 위치에서 //를 만나기 전까지.
        std::string objString = inputPath;
        if (size_t pos = inputPath.find_last_of("\\"); pos != std::string::npos)
        {
            objString = inputPath.substr(pos + 1, inputPath.size());
        }

        // obj 이름은 맨 처음 "_"를 만나기 전.
        std::string objName = objString;
        if (size_t pos = objString.find_last_of("_"); pos != std::string::npos)
        {
            objName = objString.substr(0, pos);
        }

        std::string path = std::string("../Resources/FBX/") + objName;

        // 불러온 MeshData Prefab으로 뽑기
        std::shared_ptr<GameObject> meshDataObject = std::make_shared<GameObject>();
        meshDataObject->AddComponent(std::make_shared<TransformComponent>());
        meshDataObject->SetName(s2ws(objName.c_str()));
        meshDataObject->GenerateHash();
        sceneOnlyForSave->AddGameObject(meshDataObject);
        std::shared_ptr<GameObject> mesh_root = std::make_shared<GameObject>();
        mesh_root->AddComponent(std::make_shared<TransformComponent>());
        mesh_root->SetName(s2ws(objName) + L"_mesh_root");
        mesh_root->GenerateHash();
        sceneOnlyForSave->AddGameObject(mesh_root);
        std::vector<std::shared_ptr<GameObject>> gameObjects = meshData->Instantiate();


        mesh_root->GetTransform()->SetParent(meshDataObject->GetTransform());
        for (int i = 0; i < gameObjects.size(); ++i)
        {
            gameObjects[i]->GetTransform()->SetParent(mesh_root->GetTransform());
            sceneOnlyForSave->AddGameObject(gameObjects[i]);
        }

        std::string finalPath = std::string(path.c_str()) + "_Prefab";
        GET_SINGLE(JsonManager)->SaveScene(finalPath.c_str(), sceneOnlyForSave);
    }

    // 프리팹 버튼
    if (ImGui::Button("Get prefab to scene"))
    {
        std::string finalInputPath = (inputPath.substr(0, inputPath.size() - 6));

        int tempNum = 0;
        std::shared_ptr<Scene> currentScene = GET_SINGLE(SceneManager)->GetActiveScene();

        // 주소에서 obj 이름만 가져오려면, 맨 마지막 위치에서 //를 만나기 전까지.
        std::string objString = finalInputPath;
        if (size_t pos = finalInputPath.find_last_of("\\"); pos != std::string::npos)
        {
            objString = finalInputPath.substr(pos + 1, finalInputPath.size());
        }

        // obj 이름은 맨 처음 "_"를 만나기 전.
        std::string objName = objString;
        if (size_t pos = objString.find_last_of("_"); pos != std::string::npos)
        {
            objName = objString.substr(0, pos);
        }

        // 씬에 같은 이름이 존재한다면? tempNum을 ++해서 다시 찾기.
        auto& vec = currentScene->GetGameObjects();
        auto iter = std::find_if(vec.begin(), vec.end(), [=](std::shared_ptr<GameObject> obj) {return obj->GetName() == s2ws(objName + std::to_string(tempNum)); });
        while (iter != vec.end())
        {
            ++tempNum;
            iter = std::find_if(vec.begin(), vec.end(), [=](std::shared_ptr<GameObject> obj) {return obj->GetName() == s2ws(objName + std::to_string(tempNum)); });
        }

        std::shared_ptr<Scene> sceneOnlyForLoad = std::make_shared<Scene>();
        GET_SINGLE(JsonManager)->LoadScene(finalInputPath.c_str(), sceneOnlyForLoad);

        // 불러온 임시 씬 속 프리팹 정보를 현재 씬에 넘겨주기
        for (auto& iter : sceneOnlyForLoad->GetGameObjects())
        {
            iter->SetName(iter->GetName() + std::to_wstring(tempNum));
            iter->GenerateHash();
            currentScene->AddGameObject(iter);
        }
    }

    // 씬 버튼
    if (ImGui::Button("Load"))
    {
        std::string finalInputPath = (inputPath.substr(0, inputPath.size() - 6));
        GET_SINGLE(SceneManager)->LoadScene(s2ws(finalInputPath).c_str());
    }

    // 리소스 버튼
    if (ImGui::Button("Add MeshData"))
    {
        // MeshData 불러오기
        std::shared_ptr<MeshData> meshData = GET_SINGLE(Resources)->LoadFBX(s2ws(inputPath.c_str()).c_str());
        meshData->Instantiate();
    }
    ImGui::SameLine();
    if (ImGui::Button("Add Texture"))
    {
        std::shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(s2ws(inputPath.c_str()).c_str(), s2ws(inputPath.c_str()).c_str());
    }

    static std::string paths = "";
    if (ImGui::Button("Add To SceneSet"))
    {
        // 입력받은 path를 받되, string에서 한 줄 내리고 받는다.
        /*
            경로 1 \n
            경로 2 \n ...
        */
        std::string finalInputPath = (inputPath.substr(0, inputPath.size() - 6));
        paths += finalInputPath + "\n";
    }
    ImGui::SameLine();
    if (ImGui::Button("Output SceneSet")) // 뭔가 따로 UI를 파기에는 좀 아까운 공간이기 때문에, 일단 Drag and Drop 구간에 넣었다.
    {
        std::string txtString = paths;
        std::ofstream ost("../Output/Scenes.txt");
        ost << txtString;
        paths.clear();
    }

    static std::string texturepaths = "";
    ImGui::InputText("FileName", const_cast<char*>(texturepaths.c_str()), 64);
    static std::string textureOutputpaths = "";
    if (ImGui::Button("Add To TextureSet"))
    {
        textureOutputpaths += inputPath + "\n";
    }
    ImGui::SameLine();
    if (ImGui::Button("Output TextureSet"))
    {
        std::string txtString = textureOutputpaths;
        std::string txtName = texturepaths;
        std::string finaltxtName = "../Resources/Animation/";
        finaltxtName += txtName.c_str();
        finaltxtName += ".txt";
        std::ofstream ost(finaltxtName);
        ost << txtString;
        textureOutputpaths.clear();
    }
    
    ImGui::End();
}

void ImGuiManager::RenderChild(std::shared_ptr<GameObject> parent, int i)
{
    const std::vector<std::shared_ptr<GameObject>>& vec = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();
    
    std::shared_ptr<GameObject> child = parent->GetTransform()->GetChild(i)->GetGameObject();
    ImGui::PushID(child->_hash);

    bool treeopen = ImGui::TreeNodeEx(ws2s(child->GetName().c_str()).c_str(), ImGuiTreeNodeFlags_AllowItemOverlap);
    ImGui::SameLine();
    if (ImGui::Button("+")) 
        _currentGameObject = child;
    // Our buttons are both drag sources and drag targets here!
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        // Set payload to carry the index of our item (could be anything)
        ImGui::SetDragDropPayload("DND_DEMO_CELL", &child->_hash, sizeof(size_t));

        // Display preview (could be anything, e.g. when dragging an image we could decide to display
        // the filename and a small preview of the image, etc.)
        ImGui::Text("Set Child %s", ws2s(child->GetName().c_str()).c_str());
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
        {
            IM_ASSERT(payload->DataSize == sizeof(size_t));
            size_t payload_n = *(const size_t*)payload->Data;

            // find hash
            for (int i = 0; i < vec.size(); ++i)
            {
                if (vec[i]->_hash == payload_n)
                {
                    vec[i]->GetTransform()->SetParent(child->GetTransform());
                }
            }
        }
        ImGui::EndDragDropTarget();
    }

    if (treeopen)
    {
        // Child의 Child들 재귀로 뽑기
        for (int i = 0; i < child->GetTransform()->_childVector.size(); ++i)
        {
            RenderChild(child, i);
        }
        
        ImGui::TreePop();
    }
    ImGui::PopID();
}

void ImGuiManager::AddSceneChild(std::shared_ptr<Scene> scene, std::shared_ptr<GameObject> parent)
{
    scene->AddGameObject(parent);
    for (int i = 0; i < parent->GetTransform()->GetChildCount(); ++i)
    {
        AddSceneChild(scene, parent->GetTransform()->GetChild(i)->GetGameObject());
    }
}
#endif