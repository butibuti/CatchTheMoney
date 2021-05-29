#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class TextWindow :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "TextWindow";
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
		float currentPosY;
		float previousPosY;

		Vector3 initPos;
	};

}

BUTI_REGIST_GAMECOMPONENT(TextWindow);