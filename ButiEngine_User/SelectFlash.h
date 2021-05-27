#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpliteAnimationComponent;

	class SelectFlash :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "SelectFlash";
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
		int life;
		int animationFrame;
		int awayCount;
		bool awayFlag;
		Vector3 initPosition;
	};

}

BUTI_REGIST_GAMECOMPONENT(SelectFlash);