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
		void CreateBlock();

		std::weak_ptr<GameObject> wkp_block;

		const Vector2 size = Vector2(60.0f, 60.0f);
		Vector2 velocity;
		float speed;

		bool startCreateBlock;
		Vector2 startPoint;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player);