#include "stdafx_u.h"
#include "InputManager.h"

ButiEngine::Vector2 ButiEngine::InputManager::previousLeftStick;
ButiEngine::Vector2 ButiEngine::InputManager::currentLeftSrick;
ButiEngine::Vector2 ButiEngine::InputManager::previousRightStick;
ButiEngine::Vector2 ButiEngine::InputManager::currentRightSrick;
const float ButiEngine::InputManager::DEADZONE = 0.5f;

void ButiEngine::InputManager::OnUpdate()
{
	previousLeftStick = currentLeftSrick;
	previousRightStick = currentRightSrick;
	currentLeftSrick = GameDevice::GetInput()->GetLeftStick();
	currentRightSrick = GameDevice::GetInput()->GetRightStick();
}

void ButiEngine::InputManager::OnSet()
{
}

void ButiEngine::InputManager::Start()
{
	previousLeftStick = Vector2();
	currentLeftSrick = Vector2();
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
		currentLeftSrick.x >= DEADZONE);
}

bool ButiEngine::InputManager::OnPushLeftKey()
{
	return (GameDevice::GetInput()->CheckKey(Keys::A) ||
		GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_LEFT) ||
		currentLeftSrick.x <= -DEADZONE);
}

bool ButiEngine::InputManager::OnTriggerRightKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::D) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_RIGHT) ||
		(currentLeftSrick.x >= DEADZONE && previousLeftStick.x < DEADZONE));
}

bool ButiEngine::InputManager::OnTriggerLeftKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::A) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_LEFT) ||
		(currentLeftSrick.x <= -DEADZONE && previousLeftStick.x > -DEADZONE));
}

bool ButiEngine::InputManager::OnTriggerMobiusRotateResetKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::R) ||
		GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_STICK_RIGHT));
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
