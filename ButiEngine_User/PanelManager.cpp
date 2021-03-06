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
#include"ShakeComponent.h"
#include"ReverseText.h"
#include"StageManager.h"
#include"StageSelect.h"

void ButiEngine::PanelManager::OnUpdate()
{
	StorePlayer();
	if (StageManager::GetMode() != GameMode::Edit ||
		shp_pauseManager->IsPause() ||
		IsAnimation())
	{
		return;
	}
	Control();
	Reset();
	Reverse();
}

void ButiEngine::PanelManager::OnSet()
{
}

void ButiEngine::PanelManager::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	shp_shake = GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<ShakeComponent>();
	//shp_reverseText = GetManager().lock()->GetGameObject("ParentReverseText").lock()->GetGameComponent<ReverseText>();
	moveNum = 0;
	soundNum = 0;
	panelLock_moveNum = 0;
	reverse = false;
	reverse_moveCount = 0;

	se_panelLimit = SoundTag("Sound/PanelLimit.wav");
	se_slide0 = SoundTag("Sound/PanelSlide_0.wav");
	se_slide1 = SoundTag("Sound/PanelSlide_1.wav");
	se_slide2 = SoundTag("Sound/PanelSlide_2.wav");
}

void ButiEngine::PanelManager::OnShowUI()
{
	//auto end = vec_histories.end();
	//for (auto itr = vec_histories.begin(); itr != end; ++itr)
	//{
	//	int index = itr - vec_histories.begin();
	//	int num = 1;
	//	if (*itr == LEFT)
	//	{
	//		num = 0; 
	//	}
	//	GUI::Text(std::to_string(index) + ":%d", num);
	//	if (index == currentIndex)
	//	{
	//		GUI::Text("Current!!");
	//	}
	//}

	GUI::Text("%d", moveNum);
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
	arg_panel.lock()->GetGameComponent<Panel>()->SetPanelNum(vec_frontPanels.size(), false, 10);
	vec_frontPanels.push_back(arg_panel);
}

void ButiEngine::PanelManager::AddBackPanel(std::weak_ptr<GameObject> arg_panel)
{
	arg_panel.lock()->GetGameComponent<Panel>()->SetPanelNum(GameSettings::panelCount / 2 + vec_backPanels.size(), false, 10);
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

void ButiEngine::PanelManager::Control()
{
	if (reset || reverse) { return; }
	if (InputManager::OnTriggerRightKey())
	{
		/*if (moveNum >= MOVE_LIMIT)
		{
			shp_shake->ShakeStart(3.0);
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_panelLimit, GameSettings::masterVolume);
			return;
		}*/
		SwapRight();
		RemoveHistories();
		vec_histories.push_back(RIGHT);
		currentIndex = vec_histories.size() - 1;
	}
	else if (InputManager::OnTriggerLeftKey())
	{
		/*if (moveNum <= -MOVE_LIMIT)
		{
			shp_shake->ShakeStart(3.0);
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_panelLimit, GameSettings::masterVolume);
			return;
		}*/
		if (panelLock_moveNum <= 0)
		{
			shp_shake->ShakeStart(3.0);
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_panelLimit, GameSettings::masterVolume);
			return;
		}
		SwapLeft();
		RemoveHistories();
		vec_histories.push_back(LEFT);
		currentIndex = vec_histories.size() - 1;
	}
	else if (InputManager::OnTriggerResetPanelKey())
	{
		if (moveNum == 0 || vec_histories.size() == 0)
		{
			shp_shake->ShakeStart(3.0);
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_panelLimit, GameSettings::masterVolume);
			return;
		}
		reset = true;
	}
	else if (InputManager::OnTriggerReversePanelKey() && StageSelect::GetStageNum() >= 3)
	{
		soundNum = 0;
		reverse_moveCount = 0;
		reverse = true;
	}
	//if (vec_histories.size() == 0)
	//{
	//	return;
	//}
	//if (InputManager::OnTriggerUndoKey())
	//{
	//	Undo();
	//}
	//else if (InputManager::OnTriggerRedoKey())
	//{
	//	Redo();
	//}
}

void ButiEngine::PanelManager::StorePlayer()
{
	if (!wkp_target.lock())
	{
		wkp_target = GetManager().lock()->GetGameObject("Player");
	}
}

void ButiEngine::PanelManager::SwapPanelNum(int arg_num1, int arg_num2, int arg_frame)
{
	auto end = vec_panels.end();
	for (auto itr = vec_panels.begin(); itr != end; ++itr)
	{
		(*itr).lock()->GetGameComponent<MobiusLoop>()->SwitchPosition();
	}
	wkp_target.lock()->GetGameComponent<MobiusLoop>()->SwitchPosition();


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

	vec_panels[index1].lock()->GetGameComponent<Panel>()->SetPanelNum(arg_num2, true, arg_frame);
	vec_panels[index2].lock()->GetGameComponent<Panel>()->SetPanelNum(arg_num1, true, arg_frame);
	std::iter_swap(vec_panels.begin() + index1, vec_panels.begin() + index2);

	vec_panels[index3].lock()->GetGameComponent<Panel>()->SetPanelNum(num4, true, arg_frame);
	vec_panels[index4].lock()->GetGameComponent<Panel>()->SetPanelNum(num3, true, arg_frame);
	std::iter_swap(vec_panels.begin() + index3, vec_panels.begin() + index4);
}

void ButiEngine::PanelManager::SwapRight(int arg_frame)
{
	auto currentPanel = wkp_target.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel();
	int currentIndex = currentPanel.lock()->GetGameComponent<Panel>()->GetPanelNum();
	int rightPanelIndex = currentIndex + 1;
	if (rightPanelIndex >= GameSettings::panelCount)
	{
		rightPanelIndex -= GameSettings::panelCount;
	}
	if (vec_panels[rightPanelIndex].lock()->GetGameComponent<Panel>()->IsLock())
	{
		shp_shake->ShakeStart(3.0);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_panelLimit, GameSettings::masterVolume);
		return;
	}

	SwapPanelNum(currentIndex, currentIndex + 1, arg_frame);
	PlaySlideSound();
	moveNum++;
	if (moveNum == MOVE_LIMIT)
	{
		moveNum = 0;
	}
	soundNum++;
	panelLock_moveNum++;
}

void ButiEngine::PanelManager::SwapLeft(int arg_frame)
{
	auto currentPanel = wkp_target.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel();
	int currentIndex = currentPanel.lock()->GetGameComponent<Panel>()->GetPanelNum();
	int leftPanelIndex = currentIndex - 1;
	if (leftPanelIndex < 0)
	{
		leftPanelIndex += GameSettings::panelCount;
	}
	if (vec_panels[leftPanelIndex].lock()->GetGameComponent<Panel>()->IsLock())
	{
		shp_shake->ShakeStart(3.0);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_panelLimit, GameSettings::masterVolume);
		return;
	}
	SwapPanelNum(currentIndex, currentIndex - 1, arg_frame);
	moveNum--;
	if (moveNum == -MOVE_LIMIT)
	{
		moveNum = 0;
	}
	soundNum--;
	panelLock_moveNum--;
	PlaySlideSound();
}

void ButiEngine::PanelManager::Undo(int arg_frame)
{
	if (currentIndex < 0) { return; }

	if (vec_histories[currentIndex] == RIGHT)
	{
		SwapLeft(arg_frame);
	}
	else
	{
		SwapRight(arg_frame);
	}
	currentIndex--;
}

void ButiEngine::PanelManager::Redo(int arg_frame)
{
	if (currentIndex + 1 >= vec_histories.size()) { return; }

	if (vec_histories[currentIndex + 1] == RIGHT)
	{
		SwapRight(arg_frame);
	}
	else
	{
		SwapLeft(arg_frame);
	}
	currentIndex++;
}

void ButiEngine::PanelManager::Reset()
{
	if (!reset) { return; }
	if (moveNum > 0)
	{
		SwapLeft(5);
	}
	else if (moveNum < 0)
	{
		SwapRight(5);
	}

	if (moveNum == 0)
	{
		ResetMoveHistories();
		soundNum = 0;
		panelLock_moveNum = 0;
		reset = false;
	}
}

void ButiEngine::PanelManager::Reverse()
{
	if (!reverse) { return; }
	SwapRight(5);
	RemoveHistories();
	vec_histories.push_back(RIGHT);
	currentIndex = vec_histories.size() - 1;
	reverse_moveCount++;
	if (reverse_moveCount == 3) { reverse = false; }
}

void ButiEngine::PanelManager::RemoveHistories()
{
	int lastIndex = int(vec_histories.size() - 1);
	int nextIndex = currentIndex + 1;
	if (nextIndex <= lastIndex)
	{
		auto itr = vec_histories.begin() + nextIndex;
		auto end = vec_histories.end();
		vec_histories.erase(itr, end);
	}
}

void ButiEngine::PanelManager::PlaySlideSound()
{
	if (abs(soundNum) % SOUND_LIMIT == 0)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_slide0, GameSettings::masterVolume * 3.0f);
	}
	else if (abs(soundNum) % SOUND_LIMIT == 1)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_slide1, GameSettings::masterVolume * 3.0f);
	}
	else if (abs(soundNum) % SOUND_LIMIT == 2)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_slide2, GameSettings::masterVolume * 3.0f);
	}
}
