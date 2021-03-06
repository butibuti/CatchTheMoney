#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class MobiusLoop :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "MobiusLoop";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		void OnShowUI()override;
		void ShowGUI()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		std::weak_ptr<GameObject> GetRight() { return wkp_right; }
		std::weak_ptr<GameObject> GetLeft() { return wkp_left; }

		void SwitchPosition();
		void UpdateAABB();
		void BackXRight(Vector3& arg_velocity);
		void BackYRight(Vector3& arg_velocity, float arg_gravity);
		void BackXLeft(Vector3& arg_velocity);
		void BackYLeft(Vector3& arg_velocity, float arg_gravity);
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;

		std::weak_ptr<GameObject> wkp_target;

		std::weak_ptr<GameObject> wkp_right;
		std::weak_ptr<GameObject> wkp_left;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB_right;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB_left;

		float initScaleX;

		void SetSitaPosition();
	};

}

BUTI_REGIST_GAMECOMPONENT(MobiusLoop);