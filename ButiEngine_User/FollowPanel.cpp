#include "stdafx_u.h"
#include "FollowPanel.h"
#include"PauseManager.h"
#include"PanelManager.h"

void ButiEngine::FollowPanel::OnUpdate()
{
	StoreClosestPanel();
	if (shp_pauseManager->GetPause())
	{
		if (!gameObject.lock()->transform->GetBaseTransform())
		{
			//Vector3 panelPos = wkp_closestPanel.lock()->transform->GetWorldPosition();
			//panelPos.z = -5.0f;
			//wkp_closestPanel.lock()->transform->SetWorldPosition(panelPos);
			gameObject.lock()->transform->SetBaseTransform(wkp_closestPanel.lock()->transform);
		}
		return;
	}
	else
	{
		gameObject.lock()->transform->SetBaseTransform(nullptr);
	}
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
