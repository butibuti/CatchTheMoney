#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

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
		static bool OnPushAnyKey();
		static bool OnTriggerOpenGameExitMenuKey();

		//GameCommon
		static bool OnTriggerMobiusResetRotationKey();
		static bool OnTriggerModeChangeKey();
		static bool OnTriggerOpenMenuKey();
		static bool OnTextSkipKey();
		static bool IsNoPush() { return noPush; }

		//CharaMode
		static bool OnTriggerJumpKey();
		static bool OnTriggerGrabKey();

		//PanelMode
		static bool OnPushRightScrollKey();
		static bool OnPushLeftScrollKey();
		static bool OnTriggerUndoKey();
		static bool OnTriggerRedoKey();
		static bool OnTriggerResetPanelKey();

		//anotherScene
		static bool OnTriggerDecisionKey();
		static bool OnGameStartKey();
		static bool OnSkipKey();

	private:
		std::shared_ptr<PauseManager> shp_pauseManager;

		static const float DEADZONE;
		static const int NO_PUSH_FRAME = 120;
		static Vector2 previousLeftStick;
		static Vector2 currentLeftStick;
		static Vector2 previousRightStick;
		static Vector2 currentRightSrick;
		static bool noPush;
		static int noPushCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(InputManager);