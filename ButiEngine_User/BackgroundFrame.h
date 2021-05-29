#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class BackgroundFrame :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "BackgroundFrame";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		float velocityX;
		float addAngle;
		
		void SetVelocityRandom();
		void Move();
		void Loop();
		void Rotation();
	};

}

BUTI_REGIST_GAMECOMPONENT(BackgroundFrame);