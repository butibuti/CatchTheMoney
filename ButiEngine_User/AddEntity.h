#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class AddEntity :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "AddEntity";
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
		int count;
	};

}

BUTI_REGIST_GAMECOMPONENT(AddEntity);