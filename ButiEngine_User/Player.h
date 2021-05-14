#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class PanelManager;
	class MobiusLoop;
	class SpliteAnimationComponent;
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
		void ReverseGravity();
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		bool IsClear() { return isClear; }
		float GetGravity() { return gravity; }
		std::weak_ptr<GameObject> GetHoldCore() { return wkp_holdCore; }
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

		std::shared_ptr<SpliteAnimationComponent> shp_spriteAnimation;
		const float JUMP_FORCE = 3.8f;
		const int FREEZE_FRAME = 60;
		const int ANIMATION_RATE = 5;
		const int FLOATING_FRAME = 6;
		enum Animation
		{
			IDLE,
			WALK,
			JUMP,
		};


		SoundTag se_dash;
		SoundTag se_grab;
		SoundTag se_jump;
		SoundTag se_orosu;
		SoundTag se_land;
		SoundTag se_reverse;
		SoundTag se_powerUp;

		Animation animation;

		Vector3 velocity;
		float speed;

		int freezeProgressFrame;
		int animationFrame;
		int jumpFrame;
		bool isClear;
		bool grounded;
		float gravity;
		bool pushGrabKeyFrame;
		bool freeze;
		bool jump;

		void Controll();
		void CheckGravity();
		void OnJump();
		void Move();
		void MoveX();
		void MoveY();
		void BackX();
		void BackY();
		void GrabGravityCore(std::weak_ptr<GameObject> arg_core);
		void ReleaseGravityCore();
		void Animation();

		void OnCollisionGoal(std::weak_ptr<GameObject> arg_goal);
		void OnCollisionCore(std::weak_ptr<GameObject> arg_core);
	};

}

BUTI_REGIST_GAMECOMPONENT(Player);