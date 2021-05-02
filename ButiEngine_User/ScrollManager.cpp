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
		auto scroll = (position.x / GameSettings::windowWidth);
		float dist = (scroll - wkp_screenScroll.lock()->Get().lightDir.x);
		wkp_screenScroll.lock()->Get().lightDir.x = scroll;
		//wkp_screenScroll.lock()->Get().lightDir.x +=abs( dist) * dist;
	}
	else
	{
		MoveScroll();
		auto scroll = (scrollPosition.x / GameSettings::windowWidth);
		wkp_screenScroll.lock()->Get().lightDir.x = scroll;
	}
}

void ButiEngine::ScrollManager::OnSet()
{
}

void ButiEngine::ScrollManager::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	scrollSpeed = 1.0f;
	mode = false;
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
}

void ButiEngine::ScrollManager::ResetScroll()
{
	scrollPosition = wkp_player.lock()->transform->GetWorldPosition();
}
