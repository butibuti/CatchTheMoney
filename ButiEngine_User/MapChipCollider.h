#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class MapChipCollider :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "MapChipCollider";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override; 
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		bool IsHitRight(std::weak_ptr<Collision::CollisionPrimitive> arg_collisionPrimitive);
		bool IsHitLeft(std::weak_ptr<Collision::CollisionPrimitive> arg_collisionPrimitive);
		bool IsHitTop(std::weak_ptr<Collision::CollisionPrimitive> arg_collisionPrimitive);
		bool IsHitBottom(std::weak_ptr<Collision::CollisionPrimitive> arg_collisionPrimitive);
	private:
		std::weak_ptr<GameObject> wkp_collider_right;
		std::weak_ptr<GameObject> wkp_collider_left;
		std::weak_ptr<GameObject> wkp_collider_top;
		std::weak_ptr<GameObject> wkp_collider_bottom;

		void SettingCollider(std::weak_ptr<GameObject>& arg_collider, 
			const Vector3& arg_localPos, const Vector3& arg_scale, const std::string& arg_name);
	};

}

BUTI_REGIST_GAMECOMPONENT(MapChipCollider);