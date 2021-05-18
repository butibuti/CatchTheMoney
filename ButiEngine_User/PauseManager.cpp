#include "stdafx_u.h"
#include "PauseManager.h"

void ButiEngine::PauseManager::OnUpdate()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::P))
	{
		SwitchPause();
	}
}

void ButiEngine::PauseManager::OnSet()
{
}

void ButiEngine::PauseManager::Start()
{
	pause = false;
}

void ButiEngine::PauseManager::OnShowUI()
{
}

void ButiEngine::PauseManager::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::PauseManager::Clone()
{
	return ObjectFactory::Create<PauseManager>();
}
