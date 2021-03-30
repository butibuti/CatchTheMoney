#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "Player";
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
		void Controll();
		void Move();
		void OnOutScreen();

		const Vector2 size = Vector2(100.0f, 100.0f);
		Vector2 velocity;
		float speed;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player);