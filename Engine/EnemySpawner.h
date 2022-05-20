#pragma once
#include "MonoBehaviour.h"
class EnemySpawner :
    public MonoBehaviour
{
public:
    EnemySpawner();

    void Spawn();

    virtual void LateUpdate() override;

    virtual void OnCollisionEnter(CollisionOutput collider) override;
    virtual void OnCollisionStay(CollisionOutput collider) override;
    virtual void OnCollisionExit(CollisionOutput collider) override;
};