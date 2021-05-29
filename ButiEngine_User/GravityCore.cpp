#include "stdafx_u.h"
#include "GravityCore.h"
#include"PauseManager.h"
#include"PanelManager.h"
#include"FollowPanel.h"
#include"Panel.h"
#include"Player.h"
#include"StageManager.h"
#include "MobiusLoop.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"

void ButiEngine::GravityCore::OnUpdate()
{
	StorePlayer();
	FollowPlayer();
	if (shp_pauseManager->IsPause() || StageManager::GetMode() == GameMode::Edit)
	{
		return;
	}
	RemoveGravity();
	AddGravity();
	Animation();
}

void ButiEngine::GravityCore::OnSet()
{
}

void ButiEngine::GravityCore::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	shp_panelManager = GetManager().lock()->GetGameObject("PanelManager").lock()->GetGameComponent<PanelManager>();
	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	shp_followPanel = gameObject.lock()->GetGameComponent<FollowPanel>();
	shp_mobiusLoop = gameObject.lock()->GetGameComponent<MobiusLoop>();
}

void ButiEngine::GravityCore::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::GravityCore::Clone()
{
	return ObjectFactory::Create<GravityCore>();
}

void ButiEngine::GravityCore::SetGravity(float arg_gravity)
{
	gravity = arg_gravity;
	if (gravity > 0)
	{
		Vector3 scale = gameObject.lock()->transform->GetLocalScale();
		scale.y *= -1;
		gameObject.lock()->transform->SetLocalScale(scale);
	}
}

void ButiEngine::GravityCore::SetGrabbed(bool arg_flag)
{
	grabbed = arg_flag;
}

void ButiEngine::GravityCore::RemoveGravity()
{
	shp_panelManager->RemoveGravityCores(coreNum, gravity);
}

void ButiEngine::GravityCore::AddGravity()
{
	auto closestPanel = shp_followPanel->GetClosestPanel();
	if (closestPanel.lock())
	{
		auto panelComponent = closestPanel.lock()->GetGameComponent<Panel>();
		panelComponent->AddGravityCore(coreNum, gravity);
	}
}

void ButiEngine::GravityCore::StorePlayer()
{
	if (!wkp_target.lock())
	{
		wkp_target = GetManager().lock()->GetGameObject("Player");
	}
}

void ButiEngine::GravityCore::FollowPlayer()
{
	if (!grabbed) { return; }

	Vector3 playerPos = wkp_target.lock()->transform->GetWorldPosition();
	Vector3 targetPos = playerPos;
	float playerGravity = wkp_target.lock()->GetGameComponent<Player>()->GetGravity();
	float difference = 16.0f;

	if (playerGravity > 0)
	{
		difference *= -1;
	}

	targetPos.y += difference;
	targetPos.z = gameObject.lock()->transform->GetWorldPosition().z;
	gameObject.lock()->transform->SetWorldPosition(targetPos);
}

void ButiEngine::GravityCore::Animation()
{
	animationFrame++;
	const int ANIMATION_RATE = 10;
	if (animationFrame < ANIMATION_RATE) return;

	animationFrame = 0;
	shp_spriteAnimation->UpdateHorizontalAnim(1);
	shp_mobiusLoop->GetRight().lock()->GetGameComponent<SpliteAnimationComponent>()->UpdateHorizontalAnim(1);
	shp_mobiusLoop->GetLeft().lock()->GetGameComponent<SpliteAnimationComponent>()->UpdateHorizontalAnim(1);
}
