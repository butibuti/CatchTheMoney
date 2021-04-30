#include "stdafx_u.h"
#include "ScrollManager.h"
#include"GameSettings.h"
#include "Player.h"

void ButiEngine::ScrollManager::OnUpdate()
{
	if (!weak_player.lock())
	{
		weak_player = GetManager().lock()->GetGameObject("Player");
	}
	else
	{
		if (GameDevice::GetInput()->GetPadButtonTriger(PadButtons::XBOX_START))
		{
			mode = !mode;
			scrollPosition = weak_player.lock()->transform->GetWorldPosition();
		}
		if (!mode)
		{
			Vector3 position = weak_player.lock()->transform->GetWorldPosition();
			auto scroll = (position.x / GameSettings::windowWidth);
			float dist = (scroll - wkp_screenScroll.lock()->Get().lightDir.x);
			wkp_screenScroll.lock()->Get().lightDir.x = scroll;
			//wkp_screenScroll.lock()->Get().lightDir.x +=abs( dist) * dist;
		}
		else
		{
			MoveScroll();
			auto scroll = (scrollPosition.x / GameSettings::windowWidth);
			float dist = (scroll - wkp_screenScroll.lock()->Get().lightDir.x);
			wkp_screenScroll.lock()->Get().lightDir.x = scroll;
			//wkp_screenScroll.lock()->Get().lightDir.x +=abs( dist) * dist;
		}
	}
}

void ButiEngine::ScrollManager::OnSet()
{
}

void ButiEngine::ScrollManager::Start()
{
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
	if (GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_BUTTON_LEFT))
	{
		scrollPosition.x -= scrollSpeed;
	}
	if (GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_BUTTON_RIGHT))
	{
		scrollPosition.x += scrollSpeed;
	}
}
