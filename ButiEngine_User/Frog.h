#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;
	class SpliteAnimationComponent;

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
		//裏側のカエルを指定
		void SetBackFrog(std::weak_ptr<GameObject> arg_backFrog) { wkp_backFrog = arg_backFrog; }
		std::weak_ptr<GameObject> GetBackFrog() { return wkp_backFrog; }
		//裏側のカエルと比べてプレイヤーに近いか
		bool IsNearPlayer() { return nearPlayer; }
		void SetNearPlayer(bool arg_flag) { nearPlayer = arg_flag; }
		//生成時のカエルの向きで最初の重力を決める
		void SetDefaultGravity(bool arg_top);
		void SetGravity(float arg_gravity) { gravity = arg_gravity; }
		bool IsGrabbed() { return grabbed; }
		void SetGrabbed(bool arg_flag)
		{
			grabbed = arg_flag;
			PlayAnimation();
		}
		void PlayAnimation() 
		{
			isAnimation = true; 
			progress = 0;
		}
		void Exprosion();
		bool IsAnimation() { return isAnimation; }
		std::weak_ptr<GameObject> GetSitaSentan() { return wkp_sita_sentan; }
		std::weak_ptr<GameObject> GetSitatyuukan() { return wkp_sita_tyuukan; }
		float GetGravity() { return gravity; }
		bool IsExplosion() { return isExplosion; }
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_bottomAABB;
		std::shared_ptr<SpliteAnimationComponent> shp_spriteAnimation;

		std::weak_ptr<GameObject> wkp_backFrog;
		std::weak_ptr<GameObject> wkp_sita_tyuukan;
		std::weak_ptr<GameObject> wkp_sita_sentan;
		std::weak_ptr<GameObject> wkp_bottom;
		std::weak_ptr<GameObject> wkp_player;
		std::weak_ptr<GameObject> wkp_angel;
		std::vector<std::weak_ptr<GameObject>> vec_wkp_parts;

		SoundTag se_explosion;
		SoundTag se_extend;
		SoundTag se_sitaOut;
		SoundTag se_eat;

		enum Animation
		{
			EAT_APPLE,
			IDLE,
			SITA,
			EXPROSION,
		};

		Animation animation;

		Vector3 velocity;
		float gravity;
		bool grounded;
		bool nearPlayer;
		bool grabbed;
		bool isAnimation;
		bool isApple;
		bool once;
		bool isSpawnAngel;
		bool isOutSita;
		int progress;
		int animationFrame;
		int onceCount;
		int holdAppleCount;
		bool isExplosion;

		//舌の生成
		void CreateSita();
		//所属しているパネルの重力確認
		void CheckGravity();
		//裏側のカエルよりプレイヤーに近いか確認する
		void CheckNearPlayer();
		//移動関連
		void MoveY();
		void BackY();
		//裏側のカエルに合わせる
		void Interlock();
		//プレイヤー登録
		void StorePlayer();
		//プレイヤーとの衝突判定
		void CheckHitPlayer();
		//アニメーション関連
		void Animation();
		void SpriteAnimation();
	};

}

BUTI_REGIST_GAMECOMPONENT(Frog);