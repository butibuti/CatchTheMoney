#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class TitleLogo :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "TitleLogo";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void AnimationStart();
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		float currentPosY;
		float previousPosY;
	};

}

BUTI_REGIST_GAMECOMPONENT(TitleLogo);