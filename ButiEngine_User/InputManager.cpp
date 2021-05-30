#include "stdafx_u.h"
#include "InputManager.h"
#include"TalkText.h"
#include"PauseManager.h"

ButiEngine::Vector2 ButiEngine::InputManager::previousLeftStick;
ButiEngine::Vector2 ButiEngine::InputManager::currentLeftStick;
ButiEngine::Vector2 ButiEngine::InputManager::previousRightStick;
ButiEngine::Vector2 ButiEngine::InputManager::currentRightSrick;
const float ButiEngine::InputManager::DEADZONE = 0.5f;
bool ButiEngine::InputManager::noPush;
int ButiEngine::InputManager::noPushCount;

void ButiEngine::InputManager::OnUpdate()
{
	previousLeftStick = currentLeftStick;
	previousRightStick = currentRightSrick;
	currentLeftStick = GameDevice::GetInput()->GetLeftStick();
	currentRightSrick = GameDevice::GetInput()->GetRightStick();

	if ((shp_pauseManager && shp_pauseManager->IsPause()) || !TalkText::IsDelete()) { return; }
	if (OnPushAnyKey())
	{
		noPushCount = 0;
		noPush = false;
	}
	else
	{
		noPushCount++;
	}
	if (noPushCount >= NO_PUSH_FRAME)
	{
		noPush = true;
	}
}

void ButiEngine::InputManager::OnSet()
{
}

void ButiEngine::InputManager::Start()
{
	auto pauseManager = GetManager().lock()->GetGameObject("PauseManager");
	if (pauseManager.lock())
	{
		shp_pauseManager = pauseManager.lock()->GetGameComponent<PauseManager>();
	}

	previousLeftStick = Vector2();
	currentLeftStick = Vector2();
	previousRightStick = Vector2();
	currentRightSrick = Vector2();
	noPushCount = 0;
	noPush = false;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::InputManager::Clone()
{
	return ObjectFactory::Create<InputManager>();
}

bool ButiEngine::InputManager::OnPushRightKey()
{
	return (GameDevice::GetInput()->CheckKey(Keys::D) ||
		GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_RIGHT) ||
		currentLeftStick.x >= DEADZONE);
}

bool ButiEngine::InputManager::OnPushLeftKey()
{
	return (GameDevice::GetInput()->CheckKey(Keys::A) ||
		GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_LEFT) ||
		currentLeftStick.x <= -DEADZONE);
}

bool ButiEngine::InputManager::OnTriggerRightKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::D) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_RIGHT) ||
		(currentLeftStick.x >= DEADZONE && previousLeftStick.x < DEADZONE));
}

bool ButiEngine::InputManager::OnTriggerLeftKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::A) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_LEFT) ||
		(currentLeftStick.x <= -DEADZONE && previousLeftStick.x > -DEADZONE));
}

bool ButiEngine::InputManager::OnTriggerUpKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::W) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_UP) ||
		(currentLeftStick.y >= DEADZONE && previousLeftStick.y < DEADZONE));
}

bool ButiEngine::InputManager::OnTriggerDownKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::S) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_DOWN) ||
		(currentLeftStick.y <= -DEADZONE && previousLeftStick.y > -DEADZONE));
}

bool ButiEngine::InputManager::OnPushAnyKey()
{
	return (OnPushRightKey() ||
		OnPushLeftKey() ||
		OnTriggerJumpKey() ||
		OnTriggerModeChangeKey() ||
		OnTriggerMobiusResetRotationKey() ||
		abs(currentRightSrick.x) > DEADZONE ||
		abs(currentRightSrick.y) > DEADZONE ||
		OnTriggerGrabKey());
}

bool ButiEngine::InputManager::OnTriggerOpenGameExitMenuKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::F4) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_BACK));
}

bool ButiEngine::InputManager::OnTriggerMobiusResetRotationKey()
{
	return (GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_STICK_RIGHT) ||
		GameDevice::GetInput()->GetMouseTrigger(MouseButtons::RightClick));
}

bool ButiEngine::InputManager::OnTriggerModeChangeKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::Space) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_XB) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_Y));
}

bool ButiEngine::InputManager::OnTriggerOpenMenuKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::Esc) ||
		//GameDevice::GetInput()->TriggerKey(Keys::Esc) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_START));
}

bool ButiEngine::InputManager::OnTextSkipKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::X) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_Y));
}

bool ButiEngine::InputManager::OnTriggerJumpKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::W) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_A));
}

bool ButiEngine::InputManager::OnTriggerGrabKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::E) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_B));
}

bool ButiEngine::InputManager::OnPushRightScrollKey()
{
	return (GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_BUTTON_RIGHT));
}

bool ButiEngine::InputManager::OnPushLeftScrollKey()
{
	return (GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_BUTTON_LEFT));
}

bool ButiEngine::InputManager::OnTriggerUndoKey()
{
	return ((GameDevice::GetInput()->TriggerKey(Keys::LeftCtrl) && GameDevice::GetInput()->CheckKey(Keys::Z)) ||
		(GameDevice::GetInput()->CheckKey(Keys::LeftCtrl) && GameDevice::GetInput()->TriggerKey(Keys::Z)));
		//GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_LEFT);
}

bool ButiEngine::InputManager::OnTriggerRedoKey()
{
	return ((GameDevice::GetInput()->TriggerKey(Keys::LeftCtrl) && GameDevice::GetInput()->CheckKey(Keys::Y)) ||
		(GameDevice::GetInput()->CheckKey(Keys::LeftCtrl) && GameDevice::GetInput()->TriggerKey(Keys::Y)));
		//GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_BACK);
}

bool ButiEngine::InputManager::OnTriggerResetPanelKey()
{
	return ((GameDevice::GetInput()->TriggerKey(Keys::LeftCtrl) && GameDevice::GetInput()->CheckKey(Keys::R)) ||
		(GameDevice::GetInput()->CheckKey(Keys::LeftCtrl) && GameDevice::GetInput()->TriggerKey(Keys::R)) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_B));
}

bool ButiEngine::InputManager::OnTriggerDecisionKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::Z) ||
		GameDevice::GetInput()->TriggerKey(Keys::Enter) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_A));
}

bool ButiEngine::InputManager::OnGameStartKey()
{
	return GameDevice::GetInput()->GetAnyButtonTrigger() ||
		GameDevice::GetInput()->TriggerKey(Keys::Z);
}

bool ButiEngine::InputManager::OnSkipKey()
{
	const float DEADZONE_SUPER = 0.8f;
	return (GameDevice::GetInput()->TriggerKey(Keys::W) ||
		GameDevice::GetInput()->TriggerKey(Keys::S) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_UP) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_DOWN) ||
		(currentLeftStick.y >= DEADZONE_SUPER && previousLeftStick.y < DEADZONE_SUPER) ||
		(currentLeftStick.y <= -DEADZONE_SUPER && previousLeftStick.y > -DEADZONE_SUPER));
}
