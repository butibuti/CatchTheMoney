#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RotateButtonUI :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "RotateButtonUI";
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
		void Appear();
		void Disappear();
	private:
	};

}

BUTI_REGIST_GAMECOMPONENT(RotateButtonUI);