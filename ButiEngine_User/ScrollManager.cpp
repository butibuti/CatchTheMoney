#include "stdafx_u.h"
#include "ScrollManager.h"
#include"GameSettings.h"
#include "Player.h"
#include"PauseManager.h"
#include"InputManager.h"

void ButiEngine::ScrollManager::OnUpdate()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
		return;
	}

	if (!shp_pauseManager->GetPause())
	{
		Vector3 position = wkp_player.lock()->transform->GetWorldPosition();
		currentScroll = (position.x / GameSettings::windowWidth);
		scrollPosition.x = position.x;
		//float dist = (currentScroll - wkp_screenScroll.lock()->Get().lightDir.x);
		// 
		//wkp_screenScroll.lock()->Get().lightDir.x +=abs( dist) * dist;
	}
	else
	{
		//Vector3 position = wkp_player.lock()->transform->GetWorldPosition();
		//currentScroll = (position.x / GameSettings::windowWidth);
		//scrollPosition.x = position.x;
		MoveScroll();
		currentScroll = (scrollPosition.x / GameSettings::windowWidth);
	}

	float* distance = LoopDistance(previousScroll, currentScroll);
	float* distanceRight = LoopDistance(previousScroll + 0.5f, currentScroll);
	float* distanceLeft = LoopDistance(previousScroll - 0.5f, currentScroll);

	if (distance[0] < distance[1])
	{
		previousScroll -= distance[0] * 0.075f;
		distance = LoopDistance(previousScroll, currentScroll);
		if (distance[1] < distance[0])
		{
			previousScroll = currentScroll;
		}
	}
	else if (distance[1] < distance[0])
	{
		previousScroll += distance[1] * 0.075f;
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
}

void ButiEngine::ScrollManager::OnCollision(std::weak_ptr<GameObject> arg_other)
{
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

float* ButiEngine::ScrollManager::LoopDistance(float arg_prev, float arg_curr)
{
	float distance[2];

	if (arg_prev < arg_curr)
	{
		distance[0] = arg_prev + 1.0f - arg_curr;
		distance[1] = arg_curr - arg_prev;
	}
	else if(arg_prev > arg_curr)
	{
		distance[0] = arg_prev - arg_curr;
		distance[1] = arg_curr + 1.0f - arg_prev;
	}

	return distance;
}

void ButiEngine::ScrollManager::ResetScroll()
{
	scrollPosition = wkp_player.lock()->transform->GetWorldPosition();
}
