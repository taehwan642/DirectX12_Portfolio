#pragma once
#include "MonoBehaviour.h"
class EnemySpawner :
    public MonoBehaviour
{
public:
    EnemySpawner();

    void Spawn();

    virtual void LateUpdate() override;

    virtual void OnCollisionEnter(const CollisionOutput& collider) override;
    virtual void OnCollisionStay(const CollisionOutput& collider) override;
    virtual void OnCollisionExit(const CollisionOutput& collider) override;
};