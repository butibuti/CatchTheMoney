#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SelectScreen :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "SelectScreen";
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

		void StartAnimation() { isAnimation = true; }
	private:
		float subRotationY;
		float rotationY;
		bool isAnimation;
	};

}

BUTI_REGIST_GAMECOMPONENT(SelectScreen);