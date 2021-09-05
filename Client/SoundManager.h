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

	// �� ���� �÷����ϰ�, ���¸� ������ �������� �÷��� �ȵ�
	int PlayOnce(eSoundName name);

	// �׳� ������ �Ҹ��� ���´�. �����ؼ� �� ��.
	int PlayForce(eSoundName name);
	
	// ä�ο��� �÷������� ������ ��°��� (�ڵ�üũ)
	int PlayAuto(eSoundName name);

	// ���θ� ������ �ʰ� �׳� �÷�����. �Ҹ��� ���� ���� ����
	int PlayEngineSound(eSoundName name, int carNum);
	int StopEngineSound(int carNum);

	// ��������� �����. �Ҹ��� ���ٰ� �Ѿ� ��.
	int PlayBGM(eSoundName name);
	int StopBGM();



	int pause();
	int resume();
	int stop();
	int volumeUp();
	int volume(float num);
	int volumeDown();
	int Update();

	/// Ư�� ���带 ��� ������ �����ΰ�?
	bool IsPlaying(eSoundName name);

	/// Ư�� ���带 ��� ������ ���·� ������ش�.
	void SetSoundPlaying(eSoundName name, bool state);

	void SetEffectSoundPlaying(eSoundName name, bool state);

private:
	bool m_NowPlaying[(int)eSoundName::Max];
	FMOD_CHANNEL* m_Channels[6];

	// 0 - BGM
	// 1 - ����(��1)
	// 2 - ����(��2)
	// 3 - ����(��3)
	// 4 - ����(��4)
	// 5 - ��� ����Ʈ


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

