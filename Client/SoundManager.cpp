#include <stdio.h>
#include <iostream>
#include <cstring>
#include "SoundManager.h"
#include <map>

/// sound 파일


FMOD_SYSTEM* SoundManager::g_sound_system;

SoundManager* SoundManager::m_instance = nullptr;

SoundManager::SoundManager()
	:m_bool(false)
{
	m_channel = nullptr;
	m_volume = SOUND_DEFAULT;
}


SoundManager::~SoundManager()
{
	for (auto _sound : m_SoundList)
	{
		FMOD_Sound_Release(_sound.second);
	}

	//
	//for (std::map<eSoundName, FMOD_SOUND*>::iterator it = m_SoundList.begin(); it != m_SoundList.end(); it++)
	//{
	//	FMOD_Sound_Release(it->second);
	//}

	//FMOD_Sound_Release(m_sound);
	m_instance->FreeIntance();
}


int SoundManager::Init()
{
	FMOD_System_Create(&g_sound_system);
	FMOD_System_Init(g_sound_system, 32, FMOD_INIT_NORMAL, NULL);

	return 0;
}

int SoundManager::Release()
{
	FMOD_System_Close(g_sound_system);
	FMOD_System_Release(g_sound_system);

	return 0;
}


void SoundManager::LoadAll()
{
	for (int i = 0; i < (int)eSoundName::Max; i++)
	{
		m_NowPlaying[i] = false;
	}

	CreateSound(eSoundName::UI_BGM, "./Data/Sound/game_start.mp3", true);
	CreateSound(eSoundName::Race_BGM, "./Data/Sound/game_ingame.mp3", true);

	CreateSound(eSoundName::UI_Click, "./Data/Sound/UI_click.wav", false);
	CreateSound(eSoundName::UI_Move, "./Data/Sound/UI_move.mp3", false);
	
	CreateSound(eSoundName::Engine0_0, "./Data/Sound/Engine_green_idle.", true);
	CreateSound(eSoundName::Engine0_1, "./Data/Sound/Engine_green100.wav", true);
	CreateSound(eSoundName::Engine0_2, "./Data/Sound/Engine_green105.wav", true);
	CreateSound(eSoundName::Engine0_3, "./Data/Sound/Engine_green110.wav", true);
	CreateSound(eSoundName::Engine0_4, "./Data/Sound/Engine_green115.wav", true);

	CreateSound(eSoundName::Engine1_0, "./Data/Sound/Engine_yellow_idle.wav", true);
	CreateSound(eSoundName::Engine1_1, "./Data/Sound/Engine_yellow100.wav", true);
	CreateSound(eSoundName::Engine1_2, "./Data/Sound/Engine_yellow105.wav", true);
	CreateSound(eSoundName::Engine1_3, "./Data/Sound/Engine_yellow110.wav", true);
	CreateSound(eSoundName::Engine1_4, "./Data/Sound/Engine_yellow115.wav", true);

	CreateSound(eSoundName::Engine2_0, "./Data/Sound/Engine_0.wav", true);
	CreateSound(eSoundName::Engine2_1, "./Data/Sound/Engine_1.wav", true);
	CreateSound(eSoundName::Engine2_2, "./Data/Sound/Engine_2.wav", true);
	CreateSound(eSoundName::Engine2_3, "./Data/Sound/Engine_3.wav", true);
	CreateSound(eSoundName::Engine2_4, "./Data/Sound/Engine_4.wav", true);


	CreateSound(eSoundName::Engine3_0, "./Data/Sound/Engine_0.wav", true);
	CreateSound(eSoundName::Engine3_1, "./Data/Sound/Engine_1.wav", true);
	CreateSound(eSoundName::Engine3_2, "./Data/Sound/Engine_2.wav", true);
	CreateSound(eSoundName::Engine3_3, "./Data/Sound/Engine_3.wav", true);
	CreateSound(eSoundName::Engine3_4, "./Data/Sound/Engine_4.wav", true);

	CreateSound(eSoundName::Booster, "./Data/Sound/booster.wav", false);
	CreateSound(eSoundName::Count_Num, "./Data/Sound/count_number.wav", false);
	CreateSound(eSoundName::Count_Go, "./Data/Sound/count_go.wav", false);
	CreateSound(eSoundName::Drift, "./Data/Sound/drift_01.mp3", false);
	CreateSound(eSoundName::Car_Collide1, "./Data/Sound/crush_steel_01.wav", false);
	CreateSound(eSoundName::Car_Collide2, "./Data/Sound/crush_steel_02.wav", false);
	CreateSound(eSoundName::tire_Collide1, "./Data/Sound/crush_tire_01.wav", false);
	CreateSound(eSoundName::tire_Collide2, "./Data/Sound/crush_tire_02.wav", false);
	CreateSound(eSoundName::Guardrail_Collide, "./Data/Sound/crush_rail_01.wav", false);
}

void SoundManager::CreateSound(eSoundName name, std::string path, bool isLoop)
{
	FMOD_SOUND* _sound;

	if (isLoop == true)
	{
		FMOD_System_CreateSound(g_sound_system, path.c_str(), FMOD_LOOP_NORMAL, 0, &_sound);
	}
	else
	{
		FMOD_System_CreateSound(g_sound_system, path.c_str(), FMOD_DEFAULT, 0, &_sound);
	}
	m_SoundList.insert({ name, _sound });
}

int SoundManager::PlayOnce(eSoundName name)
{
	if (m_NowPlaying[(int)name] == false)
	{
		FMOD_System_PlaySound(g_sound_system, m_SoundList[name], NULL, false, &m_channel);

		// 방금 재생을 했으므로, 특정한 처리를 해 주기 전까지는 또다시 플레이가 되면 안된다.
		m_NowPlaying[(int)name] = true;
	}

	return 0;
}

int SoundManager::PlayForce(eSoundName name)
{
	FMOD_System_PlaySound(g_sound_system, m_SoundList[name], NULL, false, &m_channel);

	return 0;
}

// FMOD상에서 출력 중이 아니면 출력
int SoundManager::PlayAuto(eSoundName name)
{
	FMOD_BOOL _isPlaying = 0;
	FMOD_Channel_IsPlaying(m_channel, &_isPlaying);
	if (_isPlaying == false)
	{
		FMOD_System_PlaySound(g_sound_system, m_SoundList[name], NULL, false, &m_channel);
	}

	return 0;
}

int SoundManager::PlayEngineSound(eSoundName name, int carNum)
{
	FMOD_System_PlaySound(g_sound_system, m_SoundList[name], NULL, false, &m_Channels[carNum]);
	m_NowPlaying[(int)name] = true;

	return 0;
}

int SoundManager::StopEngineSound(int carNum)
{
	FMOD_Channel_Stop(m_Channels[carNum]);

	return 0;
}

int SoundManager::PlayBGM(eSoundName name)
{
	FMOD_System_PlaySound(g_sound_system, m_SoundList[name], NULL, false, &m_Channels[5]);
	m_NowPlaying[(int)name] = true;

	return 0;
}

int SoundManager::StopBGM()
{
	FMOD_Channel_Stop(m_Channels[5]);

	return 0;
}

int SoundManager::pause()
{
	FMOD_Channel_SetPaused(m_channel, true);

	return 0;
}

int SoundManager::resume()
{
	FMOD_Channel_SetPaused(m_channel, false);

	return 0;
}

int SoundManager::stop()
{
	FMOD_Channel_Stop(m_channel);

	return 0;
}

int SoundManager::volumeUp()
{
	if (m_volume < SOUND_MAX)
	{
		m_volume += SOUND_WEIGHT;
	}

	FMOD_Channel_SetVolume(m_channel, m_volume);

	return 0;
}

int SoundManager::volume(float num)
{
	// 속도가 45인데 맥스속도가 200 이라 치자
	float _volumesize = num / 300;

	if (m_volume < SOUND_MAX)
	{
		m_volume = _volumesize;
	}

	FMOD_Channel_SetVolume(m_channel, m_volume);

	return 0;
}

int SoundManager::volumeDown()
{
	if (m_volume > SOUND_MIN)
	{
		m_volume -= SOUND_WEIGHT;
	}

	FMOD_Channel_SetVolume(m_channel, m_volume);

	return 0;
}


int SoundManager::Update()
{
	FMOD_Channel_IsPlaying(m_channel, &m_bool);

	if (m_bool)
	{
		FMOD_System_Update(g_sound_system);
	}

	return 0;
}

bool SoundManager::IsPlaying(eSoundName name)
{
	return m_NowPlaying[(int)name];
}

void SoundManager::SetSoundPlaying(eSoundName name, bool state)
{
	m_NowPlaying[(int)name] = state;
}

