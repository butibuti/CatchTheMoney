#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class TalkWindow :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "TalkWindow";
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

	};

}

BUTI_REGIST_GAMECOMPONENT(TalkWindow);