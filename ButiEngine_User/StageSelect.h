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
	private:
		static int stageNum;
		static int maxStageNum;

		void OnPushRight();
		void OnPushLeft();
		void OnPushSkip();
		void OnDecision();
		void SelectRotation();

		std::weak_ptr<GameObject> wkp_parentSelectPanel;

		Vector3 preParentRotation;
		float childAngle;
		int intervalFrame;
	};

}

BUTI_REGIST_GAMECOMPONENT(StageSelect);