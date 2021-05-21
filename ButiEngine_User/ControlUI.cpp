#include "stdafx_u.h"
#include "ControlUI.h"
#include"InputManager.h"
#include"PauseManager.h"
#include"TalkText.h"

void ButiEngine::ControlUI::OnUpdate()
{
	if (stop || 
		shp_pauseManager->IsPause() ||
		!TalkText::IsDelete())
	{
		return; 
	}
	Animation();
	OnNoPush();
	OnPushAnyKey();
}

void ButiEngine::ControlUI::OnSet()
{
}

void ButiEngine::ControlUI::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	initScale = gameObject.lock()->transform->GetWorldScale();
	progress = 0.0f;
}

void ButiEngine::ControlUI::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ControlUI::Clone()
{
	return ObjectFactory::Create<ControlUI>();
}

void ButiEngine::ControlUI::Animation()
{
	if (!animation) 
	{
		if (abs(gameObject.lock()->transform->GetLocalScale().x - initScale.x) <= 5.0f)
		{
			progress = 0.0f;
			gameObject.lock()->transform->SetLocalScale(initScale);
			return;
		}
	}

	progress += 0.1f;
	float sin_ = sin(progress);
	Vector3 scale(initScale.x, initScale.y, 1);
	scale.x += sin_ * scale.x * 0.1f;
	scale.y += sin_ * scale.y * 0.1f;
	gameObject.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::ControlUI::OnNoPush()
{
	if (InputManager::IsNoPush() && !animation)
	{
		animation = true;
	}
}

void ButiEngine::ControlUI::OnPushAnyKey()
{
	if (!InputManager::IsNoPush() && animation)
	{
		animation = false;
	}
}
