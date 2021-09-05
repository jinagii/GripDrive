#pragma once
#include <fmod.h>
#include <map>

#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

enum class eSoundName
{
	UI_BGM,					//./Data/Sound/BGM1.mp3
	Race_BGM,				//./Data/Sound/BGM2.mp3
	
	UI_Click,				//./Data/Sound/UI_click.wav.mp3
	UI_Move,				//./Data/Sound/UI_move.mp3
	
	Engine0_0,				//. / Data / Sound / Engine_0.wav
	Engine0_1,				//. / Data / Sound / Engine_1.wav
	Engine0_2,				//. / Data / Sound / Engine_2.wav
	Engine0_3,				//. / Data / Sound / Engine_3.wav
	Engine0_4,				//. / Data / Sound / Engine_4.wav

	Engine1_0,
	Engine1_1,
	Engine1_2,
	Engine1_3,
	Engine1_4,

	Engine2_0,				//. / Data / Sound / Engine_0.wav
	Engine2_1,				//. / Data / Sound / Engine_1.wav
	Engine2_2,				//. / Data / Sound / Engine_2.wav
	Engine2_3,				//. / Data / Sound / Engine_3.wav
	Engine2_4,				//. / Data / Sound / Engine_4.wav

	Engine3_0,
	Engine3_1,
	Engine3_2,
	Engine3_3,
	Engine3_4,

	Count_Num,				//./Data/Sound/count_number.wav
	Count_Go,				//./Data/Sound/count_go.wav
	
	Drift,					//./Data/Sound/drift_01.wav
	
	Car_Collide1,			//./Data/Sound/crush_steel_01.wav
	Car_Collide2,			//./Data/Sound/crush_steel_02.wav
	tire_Collide1,			//./Data/Sound/crush_tire_01.wav
	tire_Collide2,			//./Data/Sound/crush_tire_02.wav
	Guardrail_Collide,		//./Data/Sound/crush_rail_01.wav
	
	Booster,				//./Data/Sound/booster.wav
		
	Max,
};


class SoundManager
{
private:
	static SoundManager* m_instance;
public:
	static SoundManager* GetInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new SoundManager();
		}
		return m_instance;
	}
private:
	static FMOD_SYSTEM* g_sound_system;
	
	std::map<eSoundName, FMOD_SOUND*> m_SoundList;

	FMOD_CHANNEL* m_channel;

	float m_volume;
	FMOD_BOOL m_bool;

private:
	SoundManager();

public:
	~SoundManager();

	static int Init();
	static int Release();

	void LoadAll();
	void CreateSound(eSoundName name, std::string path, bool isLoop);

	// 한 번만 플레이하고, 상태를 돌리기 전까지는 플레이 안됨
	int PlayOnce(eSoundName name);

	// 그냥 무조건 소리가 나온다. 주의해서 쓸 것.
	int PlayForce(eSoundName name);
	
	// 채널에서 플레이하지 않으면 출력가능 (자동체크)
	int PlayAuto(eSoundName name);

	// 여부를 따지지 않고 그냥 플레이함. 소리를 끄는 것이 가능
	int PlayEngineSound(eSoundName name, int carNum);
	int StopEngineSound(int carNum);

	// 배경음악을 재생함. 소리를 껐다가 켜야 함.
	int PlayBGM(eSoundName name);
	int StopBGM();



	int pause();
	int resume();
	int stop();
	int volumeUp();
	int volume(float num);
	int volumeDown();
	int Update();

	/// 특정 사운드를 재생 가능한 상태인가?
	bool IsPlaying(eSoundName name);

	/// 특정 사운드를 재상 가능한 상태로 만들어준다.
	void SetSoundPlaying(eSoundName name, bool state);

	void SetEffectSoundPlaying(eSoundName name, bool state);

private:
	bool m_NowPlaying[(int)eSoundName::Max];
	FMOD_CHANNEL* m_Channels[6];

	// 0 - BGM
	// 1 - 엔진(차1)
	// 2 - 엔진(차2)
	// 3 - 엔진(차3)
	// 4 - 엔진(차4)
	// 5 - 모든 이펙트


public:
	static void FreeIntance()
	{
		if (m_instance != nullptr)
		{
			delete m_instance;
			m_instance = nullptr;
		}
	}

};

