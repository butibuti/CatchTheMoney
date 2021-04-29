#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Panel :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "Panel";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::weak_ptr<GameObject> wkp_drawObject;
	};

}

BUTI_REGIST_GAMECOMPONENT(Panel);