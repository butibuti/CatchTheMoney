#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class PanelManager;
	class MobiusLoop;
	class SpliteAnimationComponent;
	class ContorolByStick;

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
		bool IsTutorial() { return isTutorial; }
		float GetGravity() { return gravity; }
		std::weak_ptr<GameObject> GetHoldCore() { return wkp_holdCore; }
		std::weak_ptr<GameObject> GetHoldFrog() { return wkp_holdFrog; }
		std::weak_ptr<GameObject> GetHoldGoal() { return wkp_holdGoal; }
		std::weak_ptr<GameObject> GetHoldSita() { return wkp_holdSita; }
		std::weak_ptr<GameObject> GetPredictionLine() { return wkp_predictionLine; }
		bool IsHitCore() { return hitCore; }
		bool IsHitFrog() { return hitFrog; }
		bool IsHitGoal() { return hitGoal; }
		bool IsHitSita() { return hitSita; }
		void ResetHitFrog() { hitFrog = false; }
		void Clear() { isClear = true; }
		void ExitTutorial() { isTutorial = false; }
		bool IsFreeze() { return freeze; }

		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> GetAABB() { return shp_AABB; }

		void OnCollisionFrog(std::weak_ptr<GameObject> arg_frog);
		void FollowSita();
		void SetGrounded(bool arg_flag) { grounded = arg_flag; }
		void SetJump(bool arg_flag) { jump = arg_flag; }
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;
		std::shared_ptr<PanelManager> shp_panelManager;
		std::shared_ptr<ContorolByStick> shp_contorolManager;
		std::shared_ptr<MobiusLoop> shp_mobiusLoop;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB;

		std::weak_ptr<GameObject> wkp_bottom;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_bottomAABB;

		std::weak_ptr<GameObject> wkp_predictionLine;
		std::weak_ptr<GameObject> wkp_holdCore;
		std::weak_ptr<GameObject> wkp_holdFrog;
		std::weak_ptr<GameObject> wkp_holdGoal;
		std::weak_ptr<GameObject> wkp_holdSita;
		std::weak_ptr<GameObject> wkp_swallowFrog;
		std::weak_ptr<GameObject> wkp_apple;

		std::weak_ptr<CBuffer<ObjectInformation>> wkp_screenScroll;

		std::shared_ptr<SpliteAnimationComponent> shp_spriteAnimation;
		const float JUMP_FORCE = 3.8f;
		const int COYOTE_TIME = 10;
		const int FREEZE_FRAME = 10;
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
		SoundTag se_put;
		SoundTag se_land;
		SoundTag se_not;
		SoundTag se_reverse;
		SoundTag se_powerUp;
		SoundTag se_frogIn;

		Animation animation;

		Vector3 velocity;
		Vector3 sitaDifference;
		Vector3 defaultScale;

		int freezeProgressFrame;
		int animationFrame;
		int jumpFrame;
		bool isClear;
		bool isTutorial;
		bool grounded;
		float gravity;
		bool pushGrabKeyFrame;
		bool freeze;
		bool jump;
		bool hitCore;
		bool hitFrog;
		bool hitGoal;
		bool hitSita;
		int jumpInputFrame;

		void Control();
		void CheckGravity();
		void OnJump();
		void Move();
		void MoveX();
		void MoveY();
		void BackX();
		void BackY();
		void GrabGravityCore(std::weak_ptr<GameObject> arg_core);
		void ReleaseGravityCore();
		void GrabFrog(std::weak_ptr<GameObject> arg_frog);
		void ReleaseFrog();
		void GrabGoal(std::weak_ptr<GameObject> arg_goal);
		void GrabSita(std::weak_ptr<GameObject> arg_sita);
		void Animation();
		void CorrectionFrog(std::weak_ptr<GameObject> arg_frog);
		void OnSwallowedFrog();

		void OnCollisionGoal(std::weak_ptr<GameObject> arg_goal);
		void OnCollisionCore(std::weak_ptr<GameObject> arg_core);
		void OnCollisionSita(std::weak_ptr<GameObject> arg_sita);

	};
}

BUTI_REGIST_GAMECOMPONENT(Player);