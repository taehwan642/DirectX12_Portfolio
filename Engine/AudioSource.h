#pragma once
#include "Component.h"

class AudioClip : public Object
{
public:
	AudioClip();

	virtual void Load(const std::wstring& path) override;

private:
	void Play(float volume, bool loop);

private:
	friend class AudioSource;

	std::unique_ptr<DirectX::SoundEffect> _audio;
	std::unique_ptr<DirectX::SoundEffectInstance> _loopInstance;
};

class AudioSource :
    public Component
{
public:
	AudioSource();
	virtual ~AudioSource();

	void LoadAudio(const std::wstring& path);

	void SetLoop(bool loop);
	void SetVolume(float volume);
	//void SetPlayOnAwake(bool playOnAwake);

	void Play();

	virtual void FinalUpdate() override;

private:
	friend class ImGuiManager;
	friend class RTTRAudioSourceValue;

	bool _loop = false;
	bool _playOnAwake = false;
	float _volume = 1.f;

	std::shared_ptr<AudioClip> _audioClip = nullptr;


	RTTR_REGISTRATION_FRIEND
};

