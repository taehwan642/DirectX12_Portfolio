#include "pch.h"
#include "GameManagerScript.h"
#include "GameObject.h"
#include "AudioSource.h"

GameManagerScript::~GameManagerScript()
{
}

void GameManagerScript::Awake()
{
	// 비행기 Audio 플레이
	GetGameObject()->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\player\\pl0010_13.wav");
	GetGameObject()->GetAudioSource()->SetLoop(true);
	GetGameObject()->GetAudioSource()->SetVolume(0.06f);
	GetGameObject()->GetAudioSource()->Play();

	// BGM 플레이
	GetGameObject()->GetAudioSource()->LoadAudio(L"..\\Resources\\Audio\\bgm\\BGM.wav");
	GetGameObject()->GetAudioSource()->SetLoop(true);
	GetGameObject()->GetAudioSource()->SetVolume(1.00f);
	GetGameObject()->GetAudioSource()->Play();
}

void GameManagerScript::LateUpdate()
{
	// 피킹 후

}
