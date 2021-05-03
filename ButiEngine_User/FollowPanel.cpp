#include "stdafx_u.h"
#include "FollowPanel.h"
#include"PauseManager.h"
#include"PanelManager.h"
#include"GravityCore.h"
#include"GameSettings.h"

void ButiEngine::FollowPanel::OnUpdate()
{
	if (shp_pauseManager->GetPause())
	{
		if (!gameObject.lock()->transform->GetBaseTransform())
		{
			//Vector3 panelPos = wkp_closestPanel.lock()->transform->GetWorldPosition();
			//panelPos.z = -5.0f;
			//wkp_closestPanel.lock()->transform->SetWorldPosition(panelPos);
			gameObject.lock()->transform->SetBaseTransform(wkp_closestPanel.lock()->transform);
			Correction();
		}
		return;
	}
	auto core = gameObject.lock()->GetGameComponent<GravityCore>();
	if (core)
	{
		if (!core->GetGrabbed())
		{
			gameObject.lock()->transform->SetBaseTransform(nullptr);
		}
	}
	else
	{
		gameObject.lock()->transform->SetBaseTransform(nullptr);
	}
	StoreClosestPanel();
}

void ButiEngine::FollowPanel::OnSet()
{
}

void ButiEngine::FollowPanel::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	shp_panelManager = GetManager().lock()->GetGameObject("PanelManager").lock()->GetGameComponent<PanelManager>();
}

void ButiEngine::FollowPanel::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::FollowPanel::Clone()
{
	return ObjectFactory::Create<FollowPanel>();
}

void ButiEngine::FollowPanel::StoreClosestPanel()
{
	float x = gameObject.lock()->transform->GetWorldPosition().x;
	wkp_closestPanel = shp_panelManager->GetClosestPanel(x);
}

void ButiEngine::FollowPanel::Correction()
{
	float localX = gameObject.lock()->transform->GetLocalPosition().x;
	float movableRange = GameSettings::panelWidth * 0.5f - GameSettings::blockSize;
	if (abs(localX) - movableRange > 0)
	{
		if (localX < 0)
		{
			movableRange *= -1;
		}
		float difference = movableRange - localX;

		auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(gameObject.lock()->transform->Clone());
		anim->GetTargetTransform()->TranslateX(difference);
		anim->SetSpeed(1.0f / 10.0f);
		anim->SetEaseType(Easing::EasingType::Liner);
	}
}
