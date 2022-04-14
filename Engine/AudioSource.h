#pragma once
#include "Component.h"

class AudioClip : public Object
{
public:
	AudioClip();

	virtual void Load(const std::wstring& path) override;

private:
	void Play(bool loop);

private:
	friend class AudioSource;

	static std::unique_ptr<DirectX::AudioEngine> _audioEngine;

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
	//void SetLoopVolume(float volume);
	//void SetPlayOnAwake(bool playOnAwake);

	void Play();

	virtual void FinalUpdate() override;

private:
	bool _loop = false;
	bool _playOnAwake = false;

	std::shared_ptr<AudioClip> _audioClip;


	RTTR_REGISTRATION_FRIEND
};

