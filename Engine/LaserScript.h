#pragma once
#include "MonoBehaviour.h"

enum class LaserState
{
    LOCKON,
    FIRE,
    END
};

class TextureAnimator;
class PathTraceCameraScript;
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
    void Fire();

private:
    LaserState _laserState = LaserState::LOCKON;
    float _fireTime = 10.0f;
    float _scrollSpeed = 1.f;
    
    std::shared_ptr<TextureAnimator> _animator;

    std::weak_ptr<GameObject> _camera;
    std::shared_ptr<PathTraceCameraScript> _cameraScript;

    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND
};

