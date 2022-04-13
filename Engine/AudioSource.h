#pragma once
#include "Component.h"
class AudioSource :
    public Component
{
public:
	AudioSource();
	virtual ~AudioSource();

	void LoadAudio(const std::wstring& path);

	void SetLoop(bool loop);
	void SetLoopVolume(float volume);
	void SetPlayOnAwake(bool playOnAwake);

	void Play();

	virtual void FinalUpdate() override;

private:
	bool _loop = false;
	bool _playOnAwake = false;

	std::unique_ptr<DirectX::SoundEffect> _audioClip;
	std::unique_ptr<DirectX::SoundEffectInstance> _loopInstance;

	static std::unique_ptr<DirectX::AudioEngine> _audioEngine;

	RTTR_REGISTRATION_FRIEND
};

