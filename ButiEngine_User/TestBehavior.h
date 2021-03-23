#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	namespace Collision {
		class CollisionPrimitive_Box_AABB;
	}

	class TestBehavior :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "TestBehavior";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override; 
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::shared_ptr<ICamera> shp_camera;
		float value;
		std::shared_ptr<RelativeTimer>shp_timer;
		std::shared_ptr<IResource_Mesh> shp_stageMesh;
		Vector3 rayPos;
		Vector3 rayNormal;
		Vector3 gravityVeloc=Vector3(0,1,0);
		std::shared_ptr<Transform> shp_floorTransform;
	};

}

BUTI_REGIST_GAMECOMPONENT(TestBehavior);