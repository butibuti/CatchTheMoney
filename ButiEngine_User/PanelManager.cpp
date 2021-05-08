#include "stdafx_u.h"
#include "PanelManager.h"
#include"ParentPanel.h"
#include"Player.h"
#include"Panel.h"
#include"GameSettings.h"
#include"MobiusLoop.h"
#include"FollowPanel.h"
#include"PauseManager.h"
#include"InputManager.h"

void ButiEngine::PanelManager::OnUpdate()
{
	StorePlayer();
	if (!shp_pauseManager->GetPause() || IsAnimation())
	{
		return;
	}
	if (InputManager::OnTriggerRightKey() && moveNum < 4)
	{
		auto currentParentPanel = wkp_player.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel();
		int currentParentIndex = currentParentPanel.lock()->GetGameComponent<Panel>()->GetPanelNum();
		SwapPanelNum(currentParentIndex, currentParentIndex + 1);
		moveNum++;
	}
	else if (InputManager::OnTriggerLeftKey() && moveNum > -4)
	{
		auto currentPanel = wkp_player.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel();
		int currentIndex = currentPanel.lock()->GetGameComponent<Panel>()->GetPanelNum();
		SwapPanelNum(currentIndex, currentIndex - 1);
		moveNum--;
	}
}

void ButiEngine::PanelManager::OnSet()
{
}

void ButiEngine::PanelManager::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	moveNum = 0;
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

void ButiEngine::PanelManager::AddFrontPanel(std::weak_ptr<GameObject> arg_panel)
{
	arg_panel.lock()->GetGameComponent<Panel>()->SetPanelNum(vec_frontPanels.size(), false);
	vec_frontPanels.push_back(arg_panel);
}

void ButiEngine::PanelManager::AddBackPanel(std::weak_ptr<GameObject> arg_panel)
{
	arg_panel.lock()->GetGameComponent<Panel>()->SetPanelNum(GameSettings::panelCount / 2 + vec_backPanels.size(), false);
	vec_backPanels.push_back(arg_panel);

	if (vec_backPanels.size() == GameSettings::panelCount / 2)
	{
		vec_panels = vec_frontPanels;
		vec_panels.insert(vec_panels.end(), vec_backPanels.begin(), vec_backPanels.end());
	}
}

void ButiEngine::PanelManager::RemoveGravityCores(int arg_num, float arg_gravity)
{
	auto end = vec_panels.end();
	for (auto itr = vec_panels.begin(); itr != end; ++itr)
	{
		(*itr).lock()->GetGameComponent<Panel>()->RemoveGravityCore(arg_num, arg_gravity);
	}
}

bool ButiEngine::PanelManager::IsAnimation()
{
	bool result = false;
	auto end = vec_panels.end();
	for (auto itr = vec_panels.begin(); itr != end; ++itr)
	{
		if ((*itr).lock()->GetGameComponent<Panel>()->IsAnimation())
		{
			result = true;
		}
	}
	return result;
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
	auto end = vec_panels.end();
	for (auto itr = vec_panels.begin(); itr != end; ++itr)
	{
		(*itr).lock()->GetGameComponent<MobiusLoop>()->SwitchPosition();
	}
	wkp_player.lock()->GetGameComponent<MobiusLoop>()->SwitchPosition();


	int index1 = arg_num1;
	int index2 = arg_num2;
	int index3 = index1 + GameSettings::panelCount / 2;
	int index4 = index2 + GameSettings::panelCount / 2;

	if (index1 >= GameSettings::panelCount / 2)
	{
		index3 = index1 - GameSettings::panelCount / 2;
	}
	if (index2 >= GameSettings::panelCount / 2)
	{
		index4 = index2 - GameSettings::panelCount / 2;
	}

	int num3 = index3;
	int num4 = index4;

	if (arg_num1 == GameSettings::panelCount / 2 - 1 && arg_num2 == GameSettings::panelCount / 2)
	{
		num3 = -1;
		num4 = GameSettings::panelCount;
	}
	else if (arg_num1 == GameSettings::panelCount - 1 && arg_num2 == GameSettings::panelCount)
	{
		arg_num1 = -1;
		index2 = 0;
	}
	else if (arg_num1 == GameSettings::panelCount / 2 && arg_num2 == GameSettings::panelCount / 2 - 1)
	{
		num3 = GameSettings::panelCount;
		num4 = -1;
	}
	else if (arg_num1 == 0 && arg_num2 == -1)
	{
		arg_num1 = GameSettings::panelCount;
		index2 = GameSettings::panelCount - 1;
	}

	vec_panels[index1].lock()->GetGameComponent<Panel>()->SetPanelNum(arg_num2, true);
	vec_panels[index2].lock()->GetGameComponent<Panel>()->SetPanelNum(arg_num1, true);
	std::iter_swap(vec_panels.begin() + index1, vec_panels.begin() + index2);

	vec_panels[index3].lock()->GetGameComponent<Panel>()->SetPanelNum(num4, true);
	vec_panels[index4].lock()->GetGameComponent<Panel>()->SetPanelNum(num3, true);
	std::iter_swap(vec_panels.begin() + index3, vec_panels.begin() + index4);
}
