#pragma once
#include "MonoBehaviour.h"
#include "Character.h"

enum class LaserState
{
    LOCKON,
    FIRE,
    END
};

class TextureAnimator;
class PathTraceCameraScript;
class LaserScript :
    public MonoBehaviour, public Bullet
{
public:
    MONOBEHAVIOUR(LaserScript)

    virtual ~LaserScript();

    virtual void Spawn(int hp, float speed, int damage) override;

    virtual void OnCollisionEnter(std::shared_ptr<class BaseCollider> collider) override {}
    virtual void OnCollisionStay(std::shared_ptr<class BaseCollider> collider) override {}
    virtual void OnCollisionExit(std::shared_ptr<class BaseCollider> collider) override {}

    virtual void LateUpdate() override;

    void SetFireTime(float time) { _aliveTime = time; };
    void SetLockOnTime(float time) { _lockOnTime = time; };
    void Fire();

    virtual void Move() override;
    virtual void Attack() override {};

    std::weak_ptr<GameObject> _attachedObject;
private:
    LaserState _laserState = LaserState::LOCKON;
    float _lockOnTime = 2.f;
    float _scrollSpeed = 1.f;
    float _maxSizeX = -1.f;
    float _endSpeed = 4.f;
    float _originalScaleZ = 0.f;

    std::shared_ptr<TextureAnimator> _animator;

    std::weak_ptr<GameObject> _camera;
    std::shared_ptr<PathTraceCameraScript> _cameraScript;


    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND
};

