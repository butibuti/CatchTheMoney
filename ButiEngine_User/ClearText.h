#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpliteAnimationComponent;

	class ClearText :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ClearText";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::shared_ptr<SpliteAnimationComponent> shp_spriteAnimation;

		bool isOnce;
		int horizontalNum;
		int animationCount;

		void Animation();
	};

}

BUTI_REGIST_GAMECOMPONENT(ClearText);