#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {



	class BackGroundMobius :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "BackGroundMobius";
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
			archive(scale);
			archive(euler);
			archive(translate);
			archive(timePase);
			archive(rotateEaseType);
			archive(translateEaseType);
			archive(scaleEaseType);
		}
	private:

		Quat rotate,initRotate,targetRotate;
		Vector3 translate, scale,euler;
		

		float time,timePase,zAxisRotate;
		Easing::EasingType rotateEaseType, translateEaseType, scaleEaseType;
		std::shared_ptr<Transform> shp_parent = nullptr;
	};

}

BUTI_REGIST_GAMECOMPONENT(BackGroundMobius);