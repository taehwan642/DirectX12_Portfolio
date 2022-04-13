#include "pch.h"
#include "AudioSource.h"
std::unique_ptr<DirectX::AudioEngine> AudioSource::_audioEngine = nullptr;

AudioSource::AudioSource() : Component(COMPONENT_TYPE::AUDIOSOURCE)
{
	if (_audioEngine == nullptr)
	{
		AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
		eflags |= AudioEngine_Debug;
#endif
		_audioEngine = std::make_unique<AudioEngine>(eflags);
	}
}

AudioSource::~AudioSource()
{
}

void AudioSource::LoadAudio(const std::wstring& path)
{
	_audioClip = std::make_unique<SoundEffect>(_audioEngine.get(),
		path.c_str());
}

void AudioSource::SetLoop(bool loop)
{
	_loop = loop;
	if (loop == true && _loopInstance == nullptr)
	{
		_loopInstance = _audioClip->CreateInstance();
	}
}

void AudioSource::SetLoopVolume(float volume)
{
	if (_loop == true)
	{
		_loopInstance->SetVolume(volume);
	}
	else
	{

	}
}

void AudioSource::SetPlayOnAwake(bool playOnAwake)
{
	_playOnAwake = playOnAwake;
}

void AudioSource::Play()
{
	if (_loop == true)
		_loopInstance->Play(true);
	else
		_audioClip->Play();
}

void AudioSource::FinalUpdate()
{
}
