#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ClearZanzo :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ClearZanzo";
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
		int animationCount;

	};

}
BUTI_REGIST_GAMECOMPONENT(ClearZanzo);