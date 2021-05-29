#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class StageSelect :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "StageSelect";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		void ShowGUI() override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		static int GetStageNum() { return stageNum; }
		static int GetMaxStage() { return maxStageNum; }
		static std::string GetRemoveStageName() { return removeStageName; }
		static void SetStageNum(int arg_stageNum);
		static void SetRemoveStageName(std::string arg_removeStageName);
		static bool IsAnimation() { return isAnimation; }
		static int maxStageNum;
	private:
		static int stageNum;

		void OnPushRight();
		void OnPushLeft();
		void OnPushSkip();
		void OnDecision();
		void DecisionAnimation();
		void SelectRotation();
		void Onece();

		std::shared_ptr<PauseManager> shp_pauseManager;

		std::weak_ptr<GameObject> wkp_parentSelectPanel;
		std::weak_ptr<GameObject> wkp_animationPlayer;
		std::weak_ptr<GameObject> wkp_fadeObject;

		SoundTag bgm;
		SoundTag se_enter;
		SoundTag se_select;
		SoundTag se_dash;
		SoundTag se_hit;
		SoundTag se_start;

		float preParentRotation;
		float childAngle;
		const int screenRotateFrame = 40;
		int intervalFrame;
		int animationFrame;
		int fadeCount;
		static bool isAnimation;
		bool isOnece;
		bool isNext;

		static std::string removeStageName;
	};

}

BUTI_REGIST_GAMECOMPONENT(StageSelect);