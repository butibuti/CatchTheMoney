#include "stdafx_u.h"
#include "PanelManager.h"

void ButiEngine::PanelManager::OnUpdate()
{
}

void ButiEngine::PanelManager::OnSet()
{
}

void ButiEngine::PanelManager::Start()
{
}

void ButiEngine::PanelManager::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::PanelManager::Clone()
{
	return ObjectFactory::Create<PanelManager>();
}

std::weak_ptr<ButiEngine::GameObject> ButiEngine::PanelManager::GetClosestPanel(float arg_otherX)
{
	std::weak_ptr<GameObject> closestPanel;
	float closestPanelX = 0.0f;
	float closestPanelDistance = 10000.0f;

	auto end = vec_panels.end();
	for (auto itr = vec_panels.begin(); itr != end; ++itr)
	{
		float itrX = (*itr).lock()->transform->GetWorldPosition().x;
		float itrDistance = abs(itrX - arg_otherX);

		if (itrDistance < closestPanelDistance)
		{
			closestPanel = (*itr);
			closestPanelX = closestPanel.lock()->transform->GetWorldPosition().x;
			closestPanelDistance = abs(closestPanelX - arg_otherX);
		}
	}

	return closestPanel;
}

void ButiEngine::PanelManager::AddPanel(std::weak_ptr<GameObject> arg_panel)
{
	vec_panels.push_back(arg_panel);
}
