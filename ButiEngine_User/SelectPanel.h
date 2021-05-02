#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SelectPanel :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "SelectPanel";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::weak_ptr<GameObject> wkp_parentSelectPanel;
	};

}

BUTI_REGIST_GAMECOMPONENT(SelectPanel);