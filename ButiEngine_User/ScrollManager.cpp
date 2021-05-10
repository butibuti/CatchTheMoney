#include "stdafx_u.h"
#include "ScrollManager.h"
#include"GameSettings.h"
#include "Player.h"
#include"PauseManager.h"
#include"InputManager.h"
#include"BackDraw.h"
#include"FollowPanel.h"
#include"Panel.h"

void ButiEngine::ScrollManager::OnUpdate()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
		return;
	}

	isPreviousEdit = isCurrentEdit;
	isCurrentEdit = shp_pauseManager->GetPause();
	if (!shp_pauseManager->GetPause())
	{
	}
	else
	{
		//MoveScroll();
		//currentScroll = (scrollPosition.x / GameSettings::windowWidth);
	}
	Vector3 position = wkp_player.lock()->transform->GetWorldPosition();
	currentScroll = (position.x / GameSettings::windowWidth);
	scrollPosition.x = position.x;

	//BackScroll();

	float* distance = LoopDistance(previousScroll, currentScroll);
	if (distance[0] < distance[1])
	{
		previousScroll -= distance[0] * 0.1f;
		distance = LoopDistance(previousScroll, currentScroll);
		if (distance[1] < distance[0])
		{
			previousScroll = currentScroll;
		}
	}
	else if (distance[1] < distance[0])
	{
		previousScroll += distance[1] * 0.1f;
		distance = LoopDistance(previousScroll, currentScroll);
		if (distance[0] < distance[1])
		{
			previousScroll = currentScroll;
		}
	}

	//スクロールループ処理
	if (previousScroll > 1.0f)
	{
		previousScroll -= 1.0f;
	}
	else if (previousScroll < 0.0f)
	{
		previousScroll += 1.0f;
	}
	
	//previousScroll = previousScroll * (1.0f - 0.15f) + currentScroll * 0.15f;

	wkp_screenScroll.lock()->Get().lightDir.x = previousScroll;
}

void ButiEngine::ScrollManager::OnSet()
{
}

void ButiEngine::ScrollManager::Start()
{
	previousScroll = 0;
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	scrollSpeed = 4.0f;
	wkp_screenScroll = gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<LightVariable>("LightBuffer");

	isCurrentEdit = shp_pauseManager->GetPause();
	isPreviousEdit = isCurrentEdit;
}

void ButiEngine::ScrollManager::OnShowUI()
{
	Vector3 position = wkp_player.lock()->transform->GetWorldPosition();
	auto playerUvPos = (position.x / GameSettings::windowWidth);
	GUI::Text("PlayerPositionX   : %f ", playerUvPos);
	GUI::Text("currentScroll     : %f ", currentScroll);
	GUI::Text("currentScrollRight: %f ", currentScrollRight);
	GUI::Text("currentScrollLeft : %f ", currentScrollLeft);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ScrollManager::Clone()
{
	return ObjectFactory::Create<ScrollManager>();
}

void ButiEngine::ScrollManager::MoveScroll()
{
	if (InputManager::OnPushLeftScrollKey())
	{
		scrollPosition.x -= scrollSpeed;
	}
	if (InputManager::OnPushRightScrollKey())
	{
		scrollPosition.x += scrollSpeed;
	}

	float stageSizeX = GameSettings::windowWidth / 2.0f;
	if (scrollPosition.x > stageSizeX)
	{
		scrollPosition.x = -stageSizeX;
	}
	if (scrollPosition.x < -stageSizeX)
	{
		scrollPosition.x = stageSizeX;
	}
}

void ButiEngine::ScrollManager::BackScroll()
{
	Vector3 playerPosition = wkp_player.lock()->transform->GetWorldPosition();
	currentScrollRight = ((playerPosition.x + 640.0f) / GameSettings::windowWidth);
	currentScrollLeft = ((playerPosition.x - 640.0f) / GameSettings::windowWidth);
	if (isPreviousEdit && !isCurrentEdit)
	{
		float minDistance = 0;
		float backDistance = 0;
		bool isRight = false;
		float* distanceBack = nullptr;
		float* distance = LoopDistance(previousScroll, currentScroll);
		if (distance[0] < distance[1])
		{
			minDistance = distance[0];
		}
		else if (distance[1] < distance[0])
		{
			minDistance = distance[1];
		}

		if (currentScrollRight < 0.5f && currentScrollRight > -0.5f)
		{
			isRight = true;
			distanceBack = LoopDistance(previousScroll, currentScrollRight);
		}
		else if (currentScrollLeft < 0.5f && currentScrollLeft > -0.5f)
		{
			distanceBack = LoopDistance(previousScroll, currentScrollLeft);
		}
		else
		{
			return;
		}

		if (distanceBack[1] > distanceBack[0])
		{
			backDistance = distanceBack[0];
		}
		else if (distanceBack[1] < distanceBack[0])
		{
			backDistance = distanceBack[1];
		}

		if (abs(minDistance) > abs(backDistance))
		{
			float positionX = wkp_player.lock()->transform->GetWorldPosition().x;
			float positionY = wkp_player.lock()->transform->GetWorldPosition().y;
			float positionZ = wkp_player.lock()->transform->GetWorldPosition().z;
			auto scale = wkp_player.lock()->transform->GetLocalScale();
			scale.y = -scale.y;
			//プレイヤーをワープさせる
			Vector3 position = Vector3(positionX - 640.0f, -positionY, positionZ);
			if (isRight)
			{
				position = Vector3(positionX + 640.0f, -positionY, positionZ);
			}
			wkp_player.lock()->transform->SetWorldPosition(position);
			wkp_player.lock()->GetGameComponent<Player>()->ReverseGravity();
			wkp_player.lock()->transform->SetLocalScale(scale);
			auto holdCore = wkp_player.lock()->GetGameComponent<Player>()->GetHoldCore();
			if (holdCore.lock())
			{
				holdCore.lock()->GetGameComponent<BackDraw>()->SwitchGravityCore(true);
				holdCore.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel().lock()->GetGameComponent<Panel>()->OnChangeGravity(true);
			}
			
			Vector3 playerPosition_ = wkp_player.lock()->transform->GetWorldPosition();
			currentScroll = (playerPosition_.x / GameSettings::windowWidth);
		}
	}
}

float* ButiEngine::ScrollManager::LoopDistance(const float arg_prev, const float arg_curr)
{
	float distance[2];
	float prev = arg_prev;
	float curr = arg_curr;

	if (prev < curr)
	{
		distance[0] = prev + 1.0f - curr;
		distance[1] = curr - prev;
	}
	else if(prev > curr)
	{
		distance[0] = prev - curr;
		distance[1] = curr + 1.0f - prev;
	}
	else
	{
		distance[0] = 0;
		distance[1] = 0;
	}

	return distance;
}

void ButiEngine::ScrollManager::ResetScroll()
{
	scrollPosition = wkp_player.lock()->transform->GetWorldPosition();
}
