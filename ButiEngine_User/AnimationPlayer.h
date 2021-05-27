#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpliteAnimationComponent;

	class AnimationPlayer :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "AnimationPlayer";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::shared_ptr<GameComponent> Clone()override;
		void OnShowUI()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(pase);
			archive(animMax);
		}
	private:
		std::shared_ptr<SpliteAnimationComponent> shp_anim;
		std::shared_ptr<RelativeTimer> shp_timer;
		int animNum = 0,animMax;
		int pase;
	};

}

BUTI_REGIST_GAMECOMPONENT(AnimationPlayer);