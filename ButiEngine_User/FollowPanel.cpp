#include "stdafx_u.h"
#include "FollowPanel.h"
#include"PauseManager.h"
#include"PanelManager.h"
#include"GravityCore.h"
#include"GameSettings.h"
#include"Panel.h"
#include"ParentPanel.h"

void ButiEngine::FollowPanel::OnUpdate()
{
	if (shp_pauseManager->GetPause())
	{
		pause = true;
		if (!gameObject.lock()->transform->GetBaseTransform())
		{
			gameObject.lock()->transform->SetBaseTransform(wkp_closestPanel.lock()->transform);
			panelInitZ = wkp_closestPanel.lock()->transform->GetWorldPosition().z;
			wkp_closestPanel.lock()->transform->TranslateZ(-3.0f);
			Correction();
		}
		return;
	}
	else if (pause)
	{
		pause = false;
		Vector3 pos = wkp_closestPanel.lock()->transform->GetWorldPosition();
		pos.z = panelInitZ;
		wkp_closestPanel.lock()->transform->SetWorldPosition(pos);
	}

	StoreClosestPanel();

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
	float movableRange = GameSettings::panelWidth * 0.5f - GameSettings::blockSize * 0.5f;
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
