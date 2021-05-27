#include "stdafx_u.h"
#include "ClearFlash.h"
#include"Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"
#include"Player.h"

void ButiEngine::ClearFlash::OnUpdate()
{
	StorePlayer();
	auto transform = gameObject.lock()->transform;
	if (blinkingCount > 40)
	{
		transform->SetLocalPosition(awayPos);
		return;
	}

	blinkingCount++;
	if (!(blinkingCount % 4))
	{
		transform->SetLocalPosition(awayPos);
	}
	else
	{
		transform->SetLocalPosition(initPos);
	}

	ClearScaleAnimation();
}

void ButiEngine::ClearFlash::OnSet()
{
}

void ButiEngine::ClearFlash::Start()
{
	isOnce = false;
	gameObject.lock()->transform->SetLocalScale(Vector3(0, 0, 0));
	blinkingCount = 0;
	initPos = Vector3::Zero;
	awayPos = Vector3(0, 0, 1000);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ClearFlash::Clone()
{
	return ObjectFactory::Create<ClearFlash>();
}

void ButiEngine::ClearFlash::ClearScaleAnimation()
{
	if (isOnce) return;
	isOnce = true;

	auto goal = wkp_player.lock()->GetGameComponent<Player>()->GetHoldGoal();
	if (!goal.lock())
	{
		goal = GetManager().lock()->GetGameObject("Frog");
	}
	auto goalPosition = goal.lock()->transform->GetWorldPosition();
	goalPosition.z -= 0.01f;
	gameObject.lock()->transform->SetLocalPosition(goalPosition);
	initPos = gameObject.lock()->transform->GetLocalPosition();

	auto anim = gameObject.lock()->GetGameComponent<ScaleAnimation>();
	if (!anim)
	{
		anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
		anim->SetInitScale(gameObject.lock()->transform->GetWorldScale());
		anim->SetTargetScale(Vector3(50.0f, 50.0f, 0.0f));
		anim->SetSpeed(0.06f);
		anim->SetEaseType(Easing::EasingType::EaseOutExpo);
	}
}

void ButiEngine::ClearFlash::StorePlayer()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
	}
}
