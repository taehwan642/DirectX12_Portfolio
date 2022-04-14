#include "pch.h"
#include "AudioSource.h"
#include "Resources.h"

std::unique_ptr<DirectX::AudioEngine> AudioClip::_audioEngine = nullptr;

AudioSource::AudioSource() : Component(COMPONENT_TYPE::AUDIOSOURCE)
{

}

AudioSource::~AudioSource()
{
}

void AudioSource::LoadAudio(const std::wstring& path)
{
	_audioClip = GET_SINGLE(Resources)->Load<AudioClip>(path, path);
	
}

void AudioSource::SetLoop(bool loop)
{
	_loop = loop;
	if (loop == true && _audioClip->_loopInstance == nullptr)
	{
		_audioClip->_loopInstance = _audioClip->_audio->CreateInstance();
	}
}

//void AudioSource::SetLoopVolume(float volume)
//{
//	if (_loop == true)
//	{
//		_loopInstance->SetVolume(volume);
//	}
//	else
//	{
//
//	}
//}
//
//void AudioSource::SetPlayOnAwake(bool playOnAwake)
//{
//	_playOnAwake = playOnAwake;
//}

void AudioSource::Play()
{
	_audioClip->Play(_loop);
}

void AudioSource::FinalUpdate()
{
}

AudioClip::AudioClip() : Object(OBJECT_TYPE::AUDIOCLIP)
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

void AudioClip::Load(const std::wstring& path)
{
	_audio = std::make_unique<SoundEffect>(_audioEngine.get(),
		path.c_str());
}

void AudioClip::Play(bool loop)
{
	if(loop == true)
		_loopInstance->Play(true);
	else
		_audio->Play();
}
