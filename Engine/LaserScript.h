#pragma once
#include "MonoBehaviour.h"

enum class LaserState
{
    LOCKON,
    FIRE,
    END
};

class TextureAnimator;
class LaserScript :
    public MonoBehaviour
{
public:
    MONOBEHAVIOUR(LaserScript)

    void Init();
    virtual ~LaserScript();

    virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override {}
    virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
    virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

    virtual void LateUpdate() override;

    void SetFireTime(float time) { _fireTime = time; };

private:
    LaserState _laserState = LaserState::LOCKON;
    float _fireTime = 10.0f;
    float _scrollSpeed = 1.f;
    
    bool _init = false;

    std::shared_ptr<TextureAnimator> _animator;

    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND
};

