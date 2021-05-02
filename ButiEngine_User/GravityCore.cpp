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
	RemoveGravity();
	AddGravity();
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

void ButiEngine::GravityCore::SetGravity(float arg_gravity)
{
	gravity = arg_gravity;
	if (gravity > 0)
	{
		gameObject.lock()->transform->RollLocalRotationX_Degrees(180.0f);
	}
}

void ButiEngine::GravityCore::RemoveGravity()
{
	shp_panelManager->RemoveGravityCores(coreNum, gravity);
}

void ButiEngine::GravityCore::AddGravity()
{
	auto closestPanel = shp_followPanel->GetClosestPanel();
	if (closestPanel.lock())
	{
		auto panelComponent = closestPanel.lock()->GetGameComponent<Panel>();
		panelComponent->AddGravityCore(coreNum, gravity);
	}
}
