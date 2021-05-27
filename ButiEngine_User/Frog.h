#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class Frog :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "Frog";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void SetBackFrog(std::weak_ptr<GameObject> arg_backFrog) { wkp_backFrog = arg_backFrog; }
		std::weak_ptr<GameObject> GetBackFrog() { return wkp_backFrog; }
		bool IsNearPlayer() { return nearPlayer; }
		void SetNearPlayer(bool arg_flag) { nearPlayer = arg_flag; }
		void SetDefaultGravity(bool arg_top);
		void SetGravity(float arg_gravity) { gravity = arg_gravity; }
		bool IsGrabbed() { return grabbed; }
		void SetGrabbed(bool arg_flag)
		{
			grabbed = arg_flag;
			animation = true;
		}
		bool IsAnimation() { return animation; }
		std::weak_ptr<GameObject> GetSitaSentan() { return wkp_sita_sentan; }
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_bottomAABB;

		std::weak_ptr<GameObject> wkp_backFrog;
		std::weak_ptr<GameObject> wkp_sita_tyuukan;
		std::weak_ptr<GameObject> wkp_sita_sentan;
		std::weak_ptr<GameObject> wkp_bottom;
		std::weak_ptr<GameObject> wkp_target;

		Vector3 velocity;
		float gravity;
		bool grounded;
		bool nearPlayer;
		bool grabbed;
		bool animation;
		float progress;
		bool once;

		void CreateSita();
		void CheckGravity();
		void CheckNearPlayer();
		void MoveY();
		void BackY();
		void Interlock();
		void StorePlayer();
		void CheckHitPlayer();
		void FollowPlayer();
		void Animation();
	};

}

BUTI_REGIST_GAMECOMPONENT(Frog);