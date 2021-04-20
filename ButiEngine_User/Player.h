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
		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		void OnCollisionEnd(std::weak_ptr<GameObject> arg_other)override;
		void OnShowUI()override;
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

		std::shared_ptr<PauseManager> shp_pauseManager;

		std::weak_ptr<CBuffer<LightVariable>> wkp_screenScroll;

		Vector2 velocity;
		float speed;

		bool jump;
		float gravity;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player);