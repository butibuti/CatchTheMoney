#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class DebugController :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "DebugController";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		Vector2 velocity;
		float speed;
	};

}

BUTI_REGIST_GAMECOMPONENT(DebugController);