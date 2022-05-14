#pragma once
#include "MonoBehaviour.h"
class EnemySpawner :
    public MonoBehaviour
{
public:
    EnemySpawner();

    void Spawn();

    virtual void LateUpdate() override;

    virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override;
    virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override;
    virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override;
};