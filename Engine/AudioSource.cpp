#include "pch.h"
#include "AudioSource.h"
#include "Resources.h"
#include "SceneManager.h"
#include "Engine.h"

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
}

void AudioSource::SetVolume(float volume)
{
	_volume = volume;
	if (_loop)
		_audioClip->_loopInstance->SetVolume(volume);
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
	_audioClip->Play(_volume, _loop);
}

void AudioSource::FinalUpdate()
{
}

AudioClip::AudioClip() : Object(OBJECT_TYPE::AUDIOCLIP)
{
	
}

void AudioClip::Load(const std::wstring& path)
{
	_audio = std::make_unique<SoundEffect>(GET_SINGLE(Resources)->_audioEngine.get(),
		path.c_str());

	_loopInstance = _audio->CreateInstance();

	if (std::find(GET_SINGLE(SceneManager)->GetLoadedAudioTagVector().begin(), GET_SINGLE(SceneManager)->GetLoadedAudioTagVector().end(),
		path) == GET_SINGLE(SceneManager)->GetLoadedAudioTagVector().end())
	{
		GET_SINGLE(SceneManager)->GetLoadedAudioTagVector().push_back(path);
	}
}

void AudioClip::Play(float volume, bool loop)
{
	if(loop == true)
		_loopInstance->Play(true);
	else
		_audio->Play(volume, 0, 0);
}
