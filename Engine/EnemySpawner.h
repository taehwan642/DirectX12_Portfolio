#pragma once
#include "MonoBehaviour.h"
class EnemySpawner :
    public MonoBehaviour
{
public:
    void Spawn();

    virtual void LateUpdate() override;
};