#include "stdafx_u.h"
#include "ScrollManager.h"
#include"GameSettings.h"
#include "Player.h"
#include"PauseManager.h"
#include"InputManager.h"
#include"BackDraw.h"
#include"FollowPanel.h"
#include"Panel.h"
#include"StageManager.h"

void ButiEngine::ScrollManager::OnUpdate()
{
	
	if (!wkp_target.lock())
	{
		wkp_target = GetManager().lock()->GetGameObject(targetName);
		return;
	}


	isPreviousEdit = isCurrentEdit;
	isCurrentEdit = (StageManager::GetMode() == GameMode::Edit);

	Vector3 position = wkp_target.lock()->transform->GetWorldPosition();
	currentScroll = (position.x / GameSettings::windowWidth);
	//scrollPosition.x = position.x;

	if (currentScroll > 1.0f)
	{
		currentScroll -= 1.0f;
	}
	else if (currentScroll < -1.0f)
	{
		currentScroll += 1.0f;
	}

	//BackScroll();

	float* distance = LoopDistance(previousScroll, currentScroll);
	if (distance[0] < distance[1])
	{
		previousScroll -= distance[0] * 0.2f;
		distance = LoopDistance(previousScroll, currentScroll);
		if (distance[1] < distance[0])
		{
			previousScroll = currentScroll;
		}
	}
	else if (distance[1] < distance[0])
	{
		previousScroll += distance[1] * 0.2f;
		distance = LoopDistance(previousScroll, currentScroll);
		if (distance[0] < distance[1])
		{
			previousScroll = currentScroll;
		}
	}

	//スクロールループ処理
	if (previousScroll > 0.5f)
	{
		previousScroll -= 1.0f;
	}
	else if (previousScroll < -0.5f)
	{
		previousScroll += 1.0f;
	}
	
	//previousScroll = previousScroll * (1.0f - 0.15f) + currentScroll * 0.15f;

	wkp_screenScroll.lock()->Get().lightDir.x = previousScroll;
	wkp_sideScroll.lock()->Get().lightDir.x = previousScroll;
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
	wkp_sideScroll=GetManager().lock()->GetGameObject("MobiusSide").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<LightVariable>("LightBuffer");


	isCurrentEdit = (StageManager::GetMode() == GameMode::Edit);
	isPreviousEdit = isCurrentEdit;

}

void ButiEngine::ScrollManager::OnShowUI()
{
	Vector3 position = wkp_target.lock()->transform->GetWorldPosition();
	auto playerUvPos = (position.x / GameSettings::windowWidth);
	GUI::Text("PlayerPositionX   : %f ", playerUvPos);
	GUI::Text("currentScroll     : %f ", currentScroll);
	GUI::Text("previousScroll    : %f ", previousScroll);
	GUI::Text("currentScrollRight: %f ", currentScrollRight);
	GUI::Text("currentScrollLeft : %f ", currentScrollLeft);
}

void ButiEngine::ScrollManager::SetTarget(const std::string& arg_targetName)
{
	targetName = arg_targetName;

	wkp_target = GetManager().lock()->GetGameObject(targetName);
}

void ButiEngine::ScrollManager::SetTarget(std::shared_ptr<GameObject> arg_target)
{
	wkp_target = arg_target;
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
	Vector3 playerPosition = wkp_target.lock()->transform->GetWorldPosition();
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
			float positionX = wkp_target.lock()->transform->GetWorldPosition().x;
			float positionY = wkp_target.lock()->transform->GetWorldPosition().y;
			float positionZ = wkp_target.lock()->transform->GetWorldPosition().z;
			auto scale = wkp_target.lock()->transform->GetLocalScale();
			scale.y = -scale.y;
			//プレイヤーをワープさせる
			Vector3 position = Vector3(positionX - 640.0f, -positionY, positionZ);
			if (isRight)
			{
				position = Vector3(positionX + 640.0f, -positionY, positionZ);
			}
			wkp_target.lock()->transform->SetWorldPosition(position);
			wkp_target.lock()->GetGameComponent<Player>()->ReverseGravity();
			wkp_target.lock()->transform->SetLocalScale(scale);
			auto holdCore = wkp_target.lock()->GetGameComponent<Player>()->GetHoldCore();
			if (holdCore.lock())
			{
				holdCore.lock()->GetGameComponent<BackDraw>()->SwitchGravityCore(true);
				holdCore.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel().lock()->GetGameComponent<Panel>()->OnChangeGravity(true);
			}
			
			Vector3 playerPosition_ = wkp_target.lock()->transform->GetWorldPosition();
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
	scrollPosition = wkp_target.lock()->transform->GetWorldPosition();
}
