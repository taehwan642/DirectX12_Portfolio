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
#include "Material.h"
#include "Shader.h"
#include "SphereCollider.h"
#include "CubeCollider.h"
#include "Resources.h"
#include "JsonManager.h"

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
    ImGui::ShowDemoWindow();

    ImGui::Render();
}

void ImGuiManager::SetPipeline(std::shared_ptr<GraphicsCommandQueue> cmdqueue)
{
    ID3D12DescriptorHeap* heaps[] = { _srvDescHeap.Get() };
    cmdqueue->GetGraphicsCmdList()->SetDescriptorHeaps(1, heaps);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdqueue->GetGraphicsCmdList().Get());
}

void ImGuiManager::RenderMeshData(std::shared_ptr<Mesh> mesh)
{
    ImGui::Text("Mesh VertexCount : %d", mesh->_vertexCount);
    ImGui::Text("Mesh SubsetCount : %d", mesh->GetSubsetCount());
}

void ImGuiManager::RenderMaterialData(int materialIndex, std::shared_ptr<Material> material)
{
    ImGui::Text("Material Index : %d", materialIndex);

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
}

void ImGuiManager::RenderCameraData(std::shared_ptr<Camera> camera)
{
    switch (camera->_type)
    {
    case PROJECTION_TYPE::PERSPECTIVE:
        ImGui::Text("PERSPECTIVE");
        break;
    case PROJECTION_TYPE::ORTHOGRAPHIC:
        ImGui::Text("ORTHOGRAPHIC");
        break;
    }
    ImGui::InputFloat("Near", &camera->_near);
    ImGui::InputFloat("Far", &camera->_far);
    ImGui::InputFloat("Fov", &camera->_fov);
    ImGui::InputFloat("Scale", &camera->_scale);
    ImGui::InputFloat("Width", &camera->_width);
    ImGui::InputFloat("Height", &camera->_height);
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
    int tempName = 0;
    for (auto& iter : vec)
    {
        std::string tempString = std::to_string(tempName);
        ++tempName;
        if (iter->GetName().empty() == false)
            tempString = std::string(iter->GetName().begin(), iter->GetName().end());

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

        if (ImGui::Button("Save"))
        {
            std::string path; 
            path = "../Output/" + std::string(ws2s(_currentGameObject->GetName()).c_str());
            GET_SINGLE(JsonManager)->Save(path, _currentGameObject);
        }

        // Text 적을 수 있게 해야함.
        static std::string input;
        ImGui::InputText("FileName", const_cast<char*>(input.c_str()), 64);
        if (ImGui::Button("Load"))
        {
            std::string inputText(input.c_str());
            std::string path = std::string("../Output/") + inputText;
            GET_SINGLE(JsonManager)->Load(path, _currentGameObject);
        }
        // Component들 출력

        // TRANSFORM
        if (_currentGameObject->GetTransform() != nullptr)
        {
            // Scale, Transform, Rotation, 부모 출력
            if (ImGui::BeginMenu("Transform"))
            {
                std::shared_ptr<Transform> trans = _currentGameObject->GetTransform();

                ImGui::DragFloat3("Position", reinterpret_cast<float*>(const_cast<Vec3*>(&trans->GetLocalPosition())));

                ImGui::DragFloat3("Rotation", reinterpret_cast<float*>(const_cast<Vec3*>(&trans->GetLocalRotation())), 0.1f);

                ImGui::DragFloat3("Scale", reinterpret_cast<float*>(const_cast<Vec3*>(&trans->GetLocalScale())));

                // TODO : 부모 출력

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
                RenderMeshData(mr->GetMesh());

                int n = 0;
                for (auto& iter : mr->_materials)
                {
                    if(iter->GetShader()->GetShaderType() == SHADER_TYPE::DEFERRED)
                        ImGui::Text("Set Material's int 0 param to 1 for Instancing");

                    RenderMaterialData(n, iter);
                    ++n;
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

                ImGui::EndMenu();
            }
        }

	    // PARTICLE_SYSTEM
        if (_currentGameObject->GetParticleSystem() != nullptr)
        {
            if (ImGui::BeginMenu("Particle System"))
            {
                std::shared_ptr<ParticleSystem> particle = _currentGameObject->GetParticleSystem();
                ImGui::InputInt("Max Particle", reinterpret_cast<int*>(&particle->_maxParticle));
                ImGui::InputFloat("Create Interval", &particle->_createInterval);
                ImGui::Text("Accumulated Time : %f", &particle->_accTime);
                ImGui::InputFloat("Min LifeTime", &particle->_minLifeTime);
                ImGui::InputFloat("Max LifeTime", &particle->_maxLifeTime);
                ImGui::InputFloat("Min Speed", &particle->_minSpeed);
                ImGui::InputFloat("Max Speed", &particle->_maxSpeed);
                ImGui::InputFloat("Start Scale", &particle->_startScale);
                ImGui::InputFloat("End Scale", &particle->_endScale);

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
                    break;
                }
                
                std::string s = (collider->_draw == true ? "Press To Turn Off DrawMode" : "Press To Turn On DrawMode");
                if (ImGui::Button(s.c_str()))
                {
                    collider->_draw = !collider->_draw;
                }
                
                if (ImGui::CollapsingHeader("ColliderMeshData"))
                {
                    if (ImGui::BeginMenu("Mesh"))
                    {
                        RenderMeshData(collider->_mesh);
                        ImGui::EndMenu();
                    }

                    if (ImGui::BeginMenu("Material"))
                    {
                        RenderMaterialData(0, collider->_material);
                        ImGui::EndMenu();
                    }
                }

                ImGui::EndMenu();
            }
        }

	    // ANIMATOR
        if (_currentGameObject->GetAnimator() != nullptr)
        {
            // 이건 일단 보류.
            if (ImGui::BeginMenu("Animator"))
            {
                ImGui::EndMenu();
            }
        }

        for (auto& iter : _currentGameObject->_scripts)
        {
            ImGui::Text("%s", ws2s(iter->_className).c_str());
        }
    }
    else
    {
        ImGui::Text("Selected Object : NONE");
    }
    
    ImGui::End();
}