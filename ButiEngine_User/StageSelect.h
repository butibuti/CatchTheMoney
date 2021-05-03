#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

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
	private:
		static int stageNum;
		static int maxStageNum;

		void OnPushRight();
		void OnPushLeft();
		void OnPushSkip();
		void OnDecision();
		void DecisionAnimation();
		void SelectRotation();
		void Onece();

		std::weak_ptr<GameObject> wkp_parentSelectPanel;

		Vector3 preParentRotation;
		float childAngle;
		const int screenRotateFrame = 40;
		int intervalFrame;
		int animationFrame;
		bool isAnimation;
		bool isOnece;
	};

}

BUTI_REGIST_GAMECOMPONENT(StageSelect);