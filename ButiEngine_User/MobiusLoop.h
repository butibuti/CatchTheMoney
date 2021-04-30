#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

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
		void ShowGUI()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void UpdateAABB();
		void BackXRight(Vector3& arg_velocity);
		void BackYRight(Vector3& arg_velocity);
		void BackXLeft(Vector3& arg_velocity);
		void BackYLeft(Vector3& arg_velocity);
	private:
		std::weak_ptr<GameObject> wkp_right;
		std::weak_ptr<GameObject> wkp_left;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB_right;
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB_left;
	};

}

BUTI_REGIST_GAMECOMPONENT(MobiusLoop);