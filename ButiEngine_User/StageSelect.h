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
		void OnDecision();
	};

}

BUTI_REGIST_GAMECOMPONENT(StageSelect);