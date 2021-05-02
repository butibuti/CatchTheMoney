#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "InputManager";
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

		//Common
		static bool OnPushRightKey();
		static bool OnPushLeftKey();
		static bool OnTriggerRightKey();
		static bool OnTriggerLeftKey();

		//GameCommon
		static bool OnTriggerMobiusRotateResetKey();
		static bool OnTriggerModeChangeKey();

		//NormalMode
		static bool OnTriggerJumpKey();
		static bool OnTriggerGrabKey();

		//EditMode
		static bool OnPushRightScrollKey();
		static bool OnPushLeftScrollKey();
	private:
		static const float DEADZONE;
		static Vector2 previousLeftStick;
		static Vector2 currentLeftSrick;
		static Vector2 previousRightStick;
		static Vector2 currentRightSrick;
	};

}

BUTI_REGIST_GAMECOMPONENT(InputManager);