#pragma once
#include "MonoBehaviour.h"
class TextureAnimator;
class Effect :
    public MonoBehaviour
{
public:
	MONOBEHAVIOUR(Effect)

	virtual ~Effect();

	void Init(const std::wstring& path);

	virtual void LateUpdate() override;
	void Spawn(const std::string& path, const Vec3& worldPosition);


#ifdef TOOL
	virtual void DragAndDrop(size_t hash) override;
#endif

	size_t _cameraObject;
	std::shared_ptr<GameObject> _camera;

	std::shared_ptr<TextureAnimator> _animator;

	std::string _animationTag = "";

	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
};

