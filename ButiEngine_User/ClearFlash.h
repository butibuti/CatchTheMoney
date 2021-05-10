#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ClearFlash :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ClearFlash";
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
		bool isOnce;
	};

}

BUTI_REGIST_GAMECOMPONENT(ClearFlash);