#pragma once
#include "GameObject.h"

class Mesh;
class Material;
class GameObject;
class TransformComponent;
class Visualizer
{
public:
    Visualizer();
    virtual ~Visualizer();

    void SetCubeMesh();
    void SetSphereMesh();

    std::shared_ptr<Mesh> _mesh;
    std::shared_ptr<Material> _material;
    std::shared_ptr<MeshRenderer> _meshRenderer;
    std::shared_ptr<TransformComponent> _transform;

    void FinalUpdate(const Vec3& position);
};