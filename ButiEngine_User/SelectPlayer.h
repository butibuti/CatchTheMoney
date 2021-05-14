#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpliteAnimationComponent;

	class SelectPlayer :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "SelectPlayer";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void IsDecision();
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::shared_ptr<SpliteAnimationComponent> shp_spriteAnimation;

		float velocityX;
		int animationFrame;
		int animationTime;
		bool isDecision;
		Vector3 position;

		void DecisionAnimation();
	};

}

BUTI_REGIST_GAMECOMPONENT(SelectPlayer);