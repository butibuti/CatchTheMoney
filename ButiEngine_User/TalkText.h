#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpliteAnimationComponent;
	class PauseManager;

	//会話が発生するステージ番号
	enum TalkStageNum
	{
		FIRST_TALK = 0,
		PANEL_TALK = 1,
		REVERSE_TALK = 3,
		REVERSE_RE_TALK = 5,
		GRAVITY_TALK = 8,
		FROG_TALK = 11,
		LAST_TALK = 19,
	};

	class TalkText :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "TalkText";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void ReTalk();
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		static bool IsNotMove() { return isNotMove; }
		static bool IsDelete() { return isDelete; }
		static void Delete();
		static void Revive();
	private:
		std::shared_ptr<SpliteAnimationComponent> shp_spriteAnimation;
		std::shared_ptr<PauseManager> shp_pauseManager;

		std::weak_ptr<GameObject> wkp_daikokuten;
		std::weak_ptr<GameObject> wkp_daikokutenAppear;
		std::weak_ptr<GameObject> wkp_daikokutenReaction;
		std::weak_ptr<GameObject> wkp_daikokutenRHand;
		std::weak_ptr<GameObject> wkp_daikokutenLHand;
		std::weak_ptr<GameObject> wkp_camera;
		std::weak_ptr<GameObject> wkp_cameraUI;

		SoundTag se_bigText;
		SoundTag se_normalText;

		int textCount;
		int waitTime;
		int onceFrame;
		int intervalTime;
		bool isInterval;
		Vector3 initPos;

		const float SHAKE_POWER = 8.0f;

		static bool isNotMove;
		static bool isDelete;
		bool isGeneratedControl=false;
		void TextEffect();
		void NormalTalk();
		void FrogTalk();
		void NextTalk();
		void DaikokutenDisappear();
		void FrogInterval();
		bool SetCamera();
		bool Once();
		void NextText();
		void Skip();
	};

}

BUTI_REGIST_GAMECOMPONENT(TalkText);