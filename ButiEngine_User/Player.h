#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

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

		std::shared_ptr<PauseManager> shp_pauseManager;

		std::weak_ptr<GameObject> wkp_block;
		std::weak_ptr<CBuffer<LightVariable>> wkp_screenScroll;

		const Vector2 size = Vector2(60.0f, 60.0f);
		Vector2 velocity;
		float speed;

		bool startCreateBlock;
		Vector2 startPoint;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player);