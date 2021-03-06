#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

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
		void Clear() { isClear = true; }

	private:
		std::shared_ptr<PauseManager> shp_pauseManager;

		Vector3 initAxis;
		Vector3 initRotate;
		Vector3 rotation;
		Vector3 curRotation;
		Vector3 preRotation;
		Vector2 preMousePos;
		Vector2 currMousePos;
		const float DEAD_ZONE = 0.3f;
		const float ROTATION_LIMIT = 60.0f;
		float rotationSpeed;
		float returnSpeed;
		int cameraResetFrame;
		bool isClear;

		void Control();
		void Reset();
	};

}

BUTI_REGIST_GAMECOMPONENT(ContorolByStick);