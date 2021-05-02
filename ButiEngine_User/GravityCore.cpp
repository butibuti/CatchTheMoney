#include "stdafx_u.h"
#include "GravityCore.h"
#include"PauseManager.h"
#include"PanelManager.h"
#include"FollowPanel.h"
#include"Panel.h"

void ButiEngine::GravityCore::OnUpdate()
{
	if (shp_pauseManager->GetPause())
	{
		return;
	}

	shp_panelManager->RemoveGravityCores(coreNum, gravity);
	auto closestPanel = shp_followPanel->GetClosestPanel();
	if (closestPanel.lock())
	{
		auto panelComponent = closestPanel.lock()->GetGameComponent<Panel>();
		panelComponent->AddGravityCore(coreNum, gravity);
	}
}

void ButiEngine::GravityCore::OnSet()
{
}

void ButiEngine::GravityCore::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	shp_panelManager = GetManager().lock()->GetGameObject("PanelManager").lock()->GetGameComponent<PanelManager>();
	shp_followPanel = gameObject.lock()->GetGameComponent<FollowPanel>();
}

void ButiEngine::GravityCore::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::GravityCore::Clone()
{
	return ObjectFactory::Create<GravityCore>();
}
