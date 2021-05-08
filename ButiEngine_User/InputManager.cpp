#include "stdafx_u.h"
#include "InputManager.h"

ButiEngine::Vector2 ButiEngine::InputManager::previousLeftStick;
ButiEngine::Vector2 ButiEngine::InputManager::currentLeftStick;
ButiEngine::Vector2 ButiEngine::InputManager::previousRightStick;
ButiEngine::Vector2 ButiEngine::InputManager::currentRightSrick;
const float ButiEngine::InputManager::DEADZONE = 0.5f;

void ButiEngine::InputManager::OnUpdate()
{
	previousLeftStick = currentLeftStick;
	previousRightStick = currentRightSrick;
	currentLeftStick = GameDevice::GetInput()->GetLeftStick();
	currentRightSrick = GameDevice::GetInput()->GetRightStick();
}

void ButiEngine::InputManager::OnSet()
{
}

void ButiEngine::InputManager::Start()
{
	previousLeftStick = Vector2();
	currentLeftStick = Vector2();
	previousRightStick = Vector2();
	currentRightSrick = Vector2();
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

bool ButiEngine::InputManager::OnTriggerMobiusRotateResetKey()
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
	return (GameDevice::GetInput()->TriggerKey(Keys::Tab) ||
		//GameDevice::GetInput()->TriggerKey(Keys::Esc) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_START));
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
	return (GameDevice::GetInput()->CheckKey(Keys::LeftCtrl) &&
		GameDevice::GetInput()->CheckKey(Keys::Z)) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_BACK);
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
	return (GameDevice::GetInput()->TriggerKey(Keys::W) ||
		GameDevice::GetInput()->TriggerKey(Keys::S) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_UP) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_DOWN) ||
		(currentLeftStick.y >= DEADZONE && previousLeftStick.y < DEADZONE) ||
		(currentLeftStick.y <= -DEADZONE && previousLeftStick.y > -DEADZONE));
}
