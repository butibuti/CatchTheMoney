#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ClearBand :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ClearBand";
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

BUTI_REGIST_GAMECOMPONENT(ClearBand);