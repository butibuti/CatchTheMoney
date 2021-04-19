#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	namespace Collision {
		class CollisionPrimitive_Box_AABB;
	}

	class EdgeCollider :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "EdgeCollider";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		void OnCollisionEnd(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> GetAABB() { return shp_AABB; }
	private:
		std::shared_ptr<Collision::CollisionPrimitive_Box_AABB> shp_AABB;
	};

}

BUTI_REGIST_GAMECOMPONENT(EdgeCollider);