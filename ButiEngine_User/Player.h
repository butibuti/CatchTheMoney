#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class MobiusLoop;

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
		void MoveX();
		void MoveY();
		void BackX();
		void BackY();
		
		std::shared_ptr<PauseManager> shp_pauseManager;
		std::shared_ptr<MobiusLoop> shp_mobiusLoop;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB;

		std::weak_ptr<GameObject> wkp_bottom;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_bottomAABB;

		std::weak_ptr<CBuffer<LightVariable>> wkp_screenScroll;

		Vector3 velocity;
		float speed;

		bool grounded;
		float gravity;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player);