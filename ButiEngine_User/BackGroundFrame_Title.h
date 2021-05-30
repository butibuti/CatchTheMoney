#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class BackGroundFrame_Title :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "BackGroundFrame_Title";
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
	private:
		Vector3 velocity;
		float addAngle;
		std::shared_ptr<RelativeTimer> shp_sucideTimer;
		void SetVelocityRandom();
		void SetScaleRandom();
		void Move();
		void Rotation();
	};

}

BUTI_REGIST_GAMECOMPONENT(BackGroundFrame_Title);