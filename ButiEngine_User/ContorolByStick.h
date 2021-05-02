#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ContorolByStick :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ContorolByStick";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		void OnShowUI() override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(rotationSpeed);
			archive(returnSpeed);
		}
	private:
		Vector3 initAxis;
		Vector3 rotation;
		float rotationLimit;
		float rotationSpeed;
		float returnSpeed;
		int cameraResetFrame;
	};

}

BUTI_REGIST_GAMECOMPONENT(ContorolByStick);