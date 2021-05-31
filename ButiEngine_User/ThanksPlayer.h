#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpliteAnimationComponent;

	class ThanksPlayer :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ThanksPlayer";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void Decision();
		void Away();
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
		bool isAway;
		Vector3 position;
		SoundTag se_dash;

		void DecisionAnimation();
	};

}

BUTI_REGIST_GAMECOMPONENT(ThanksPlayer);

