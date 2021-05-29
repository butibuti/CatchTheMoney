#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class AngelFrog :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "AngelFrog";
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
		int life;

		void Move();
	};

}

BUTI_REGIST_GAMECOMPONENT(AngelFrog);