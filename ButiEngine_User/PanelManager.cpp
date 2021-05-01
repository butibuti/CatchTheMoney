#include "stdafx_u.h"
#include "PanelManager.h"
#include"ParentPanel.h"
#include"Player.h"
#include"Panel.h"

void ButiEngine::PanelManager::OnUpdate()
{
	StorePlayer();
	if (GameDevice::GetInput()->TriggerKey(Keys::U))
	{
		auto currentPanel = wkp_player.lock()->GetGameComponent<Player>()->GetClosestPanel();
		int currentIndex = currentPanel.lock()->GetGameComponent<Panel>()->GetPanelNum();
		SwapPanelNum(currentIndex, currentIndex + 1);
	}
	else if (GameDevice::GetInput()->TriggerKey(Keys::Y))
	{
		auto currentPanel = wkp_player.lock()->GetGameComponent<Player>()->GetClosestPanel();
		int currentIndex = currentPanel.lock()->GetGameComponent<Panel>()->GetPanelNum();
		SwapPanelNum(currentIndex, currentIndex - 1);
	}
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

void ButiEngine::PanelManager::AddParentPanel(std::weak_ptr<GameObject> arg_panel)
{
	arg_panel.lock()->GetGameComponent<ParentPanel>()->SetPanelNum(vec_parentPanels.size());
	vec_parentPanels.push_back(arg_panel);
}

void ButiEngine::PanelManager::StorePlayer()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
	}
}

void ButiEngine::PanelManager::SwapPanelNum(int arg_num1, int arg_num2)
{
	int index1 = arg_num1;
	int index2 = arg_num2;

	if (arg_num1 == 0 && arg_num2 < 0)
	{
		arg_num1 = vec_parentPanels.size();
		index2 = vec_parentPanels.size() - 1;
	}
	else if (arg_num1 == vec_parentPanels.size() - 1 && arg_num2 >= vec_parentPanels.size())
	{
		arg_num1 = -1;
		index2 = 0;
	}

	vec_parentPanels[index1].lock()->GetGameComponent<ParentPanel>()->SetPanelNum(arg_num2);
	vec_parentPanels[index2].lock()->GetGameComponent<ParentPanel>()->SetPanelNum(arg_num1);

	std::iter_swap(vec_parentPanels.begin() + index1, vec_parentPanels.begin() + index2);
}
