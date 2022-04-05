#pragma once

class Texture;
class TextureAnimator
{
public:
	TextureAnimator();
	virtual ~TextureAnimator();

	void UpdateAnimation();

	bool LoadAnimation(const std::wstring& path);
	bool LoadAnimation(const std::wstring& key, const std::wstring& path, size_t count);

	void SetInterval(float interval) { _interval = interval; }
	float GetInterval() { return _interval; }

	void SetLoop(bool loop) { _isLoop = loop; }
	bool GetLoop() { return _isLoop; }

	std::shared_ptr<Texture> GetTexture(int index);
	std::shared_ptr<Texture> GetCurrentAnimationTexture();

	size_t GetAnimationSize() { return _animationSize; }

	void SetCurrentIndex(int index) { _currentIndex = index; }

private:
	friend class ImGuiManager;

	float _interval = 0.1f;
	bool _isLoop = true;
	int _currentIndex = 0;
	size_t _animationSize = 1;

	std::vector<std::wstring> _tags;
	std::vector<std::shared_ptr<Texture>> _textures;
};

