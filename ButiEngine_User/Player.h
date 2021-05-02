#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class PanelManager;
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
		void ShowGUI()override;
		void OnShowUI()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;
		std::shared_ptr<PanelManager> shp_panelManager;
		std::shared_ptr<MobiusLoop> shp_mobiusLoop;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB;

		std::weak_ptr<GameObject> wkp_bottom;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_bottomAABB;

		std::weak_ptr<GameObject> wkp_predictionLine;
		std::weak_ptr<GameObject> wkp_holdCore;

		std::weak_ptr<CBuffer<LightVariable>> wkp_screenScroll;

		const float JUMP_FORCE = 2.5f;

		Vector3 velocity;
		float speed;

		bool grounded;
		float gravity;
		bool pushGrabKeyFrame;
		int delay;
		float rotationX;

		void Controll();
		void CheckGravity();
		void Move();
		void MoveX();
		void MoveY();
		void BackX();
		void BackY();

		void OnCollisionGoal(std::weak_ptr<GameObject> arg_goal);
		void OnCollisionCore(std::weak_ptr<GameObject> arg_core);
	};

}

BUTI_REGIST_GAMECOMPONENT(Player);