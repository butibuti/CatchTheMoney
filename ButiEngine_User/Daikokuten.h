#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Daikokuten :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "Daikokuten";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void TalkScale();
		void NormalScale();
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:

	};
}

BUTI_REGIST_GAMECOMPONENT(Daikokuten);