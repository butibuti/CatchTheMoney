#include "stdafx_u.h"
#include "BackDraw.h"
#include"GameSettings.h"
#include"GravityCore.h"
#include"FollowPanel.h"
#include"Panel.h"
#include"PauseManager.h"
#include"StageManager.h"
#include"MobiusLoop.h"
#include"Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"
#include"AnimationPlayer.h"

void ButiEngine::BackDraw::OnUpdate()
{
	StorePlayer();
	SwitchGravityCore(false);
	Correction();
}

void ButiEngine::BackDraw::OnSet()
{
}

void ButiEngine::BackDraw::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	std::string name = gameObject.lock()->GetGameObjectName();
	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	Vector3 clonePosition = Vector3::Zero;
	Vector3 scale = gameObject.lock()->transform->GetWorldScale();
	scale.y *= -1;
	Matrix4x4 rotation = gameObject.lock()->transform->GetWorldRotation();
	auto tag = GetTagManager()->GetObjectTag("MapChip");
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	if (!meshDraw)
	{
		meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent_Static>();
	}

	wkp_right = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Back_Right");
	clonePosition.x = position.x + GameSettings::windowWidth * 0.5f;
	clonePosition.y = -position.y;
	clonePosition.z = position.z;
	wkp_right.lock()->transform->SetWorldPosition(clonePosition);
	wkp_right.lock()->transform->SetLocalScale(scale);
	wkp_right.lock()->SetGameObjectTag(tag);
	if (meshDraw)
	{
		wkp_right.lock()->AddGameComponent(meshDraw->Clone());
	}
	wkp_left = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Back_Left");
	clonePosition.x = position.x - GameSettings::windowWidth * 0.5f;
	wkp_left.lock()->transform->SetWorldPosition(clonePosition);
	wkp_left.lock()->transform->SetLocalScale(scale);
	wkp_left.lock()->SetGameObjectTag(tag);
	if (meshDraw)
	{
		wkp_left.lock()->AddGameComponent(meshDraw->Clone());
	}
	auto effectUpdater = gameObject.lock()->GetGameComponent<PostEffectParamUpdater>();
	if (effectUpdater) {
		wkp_right.lock()->AddGameComponent(effectUpdater->Clone());
		wkp_left.lock()->AddGameComponent(effectUpdater->Clone());
	}

	if (name == "Apple")
	{
		auto spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
		auto animationPlayer = gameObject.lock()->GetGameComponent<AnimationPlayer>();
		wkp_right.lock()->AddGameComponent(spriteAnimation->Clone());
		wkp_left.lock()->AddGameComponent(spriteAnimation->Clone());
		wkp_right.lock()->AddGameComponent(animationPlayer->Clone());
		wkp_left.lock()->AddGameComponent(animationPlayer->Clone());
	}

	if (name == "PredictionLine")
	{
		auto rloop = wkp_right.lock()->AddGameComponent<MobiusLoop>();
		auto lLoop = wkp_left.lock()->AddGameComponent<MobiusLoop>();

		rloop->Start();
		lLoop->Start();
	}

}

void ButiEngine::BackDraw::OnShowUI()
{
	//GUI::Text("Right:%f", wkp_right.lock()->transform->GetWorldPosition().x);
	//GUI::Text("Left:%f", wkp_left.lock()->transform->GetWorldPosition().x);
}

void ButiEngine::BackDraw::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::BackDraw::Clone()
{
	return ObjectFactory::Create<BackDraw>();
}

void ButiEngine::BackDraw::StorePlayer()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
	}
}

void ButiEngine::BackDraw::Correction()
{
	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	Vector3 clonePosition = Vector3::Zero;
	Vector3 scale = gameObject.lock()->transform->GetWorldScale();
	Matrix4x4 rotation = gameObject.lock()->transform->GetWorldRotation();
	scale.y *= -1;
	clonePosition.x = position.x + GameSettings::windowWidth * 0.5f;
	clonePosition.y = -position.y;
	clonePosition.z = position.z;
	if (StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "PredictionLine"))
	{
		clonePosition.z += 0.001f;
	}
	wkp_right.lock()->transform->SetWorldPosition(clonePosition);
	wkp_right.lock()->transform->SetLocalScale(scale);
	clonePosition.x = position.x - GameSettings::windowWidth * 0.5f;
	wkp_left.lock()->transform->SetWorldPosition(clonePosition);
	wkp_left.lock()->transform->SetLocalScale(scale);
}

void ButiEngine::BackDraw::SwitchGravityCore(bool arg_scroll)
{
	if (shp_pauseManager->IsPause() || StageManager::GetMode() == GameMode::Edit)
	{
		return;
	}
	if (!StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "GravityCore"))
	{
		return;
	}
	if (!arg_scroll && gameObject.lock()->GetGameComponent<GravityCore>()->GetGrabbed())
	{
		return;
	}
	if (wkp_player.lock())
	{
		float playerX = wkp_player.lock()->transform->GetWorldPosition().x;
		float rightX = wkp_right.lock()->transform->GetWorldPosition().x;
		float leftX = wkp_left.lock()->transform->GetWorldPosition().x;
		Vector3 scale = gameObject.lock()->transform->GetLocalScale();

		if (rightX < GameSettings::windowWidth * 0.5f && playerX > 0 == rightX > 0)
		{
			scale.y *= -1;
			gameObject.lock()->transform->SetLocalScale(scale);
			gameObject.lock()->transform->SetWorldPosition(wkp_right.lock()->transform->GetWorldPosition());
			gameObject.lock()->GetGameComponent<GravityCore>()->RemoveGravity();
			gameObject.lock()->GetGameComponent<GravityCore>()->ReverseGravity();
			gameObject.lock()->GetGameComponent<FollowPanel>()->StoreClosestPanel();
			gameObject.lock()->GetGameComponent<GravityCore>()->RemoveGravity();
			gameObject.lock()->GetGameComponent<GravityCore>()->AddGravity();
		}
		else if (leftX > -GameSettings::windowWidth * 0.5f && playerX > 0 == leftX > 0)
		{
			scale.y *= -1;
			gameObject.lock()->transform->SetLocalScale(scale);
			gameObject.lock()->transform->SetWorldPosition(wkp_left.lock()->transform->GetWorldPosition());
			gameObject.lock()->GetGameComponent<GravityCore>()->RemoveGravity();
			gameObject.lock()->GetGameComponent<GravityCore>()->ReverseGravity();
			gameObject.lock()->GetGameComponent<FollowPanel>()->StoreClosestPanel();
			gameObject.lock()->GetGameComponent<GravityCore>()->RemoveGravity();
			gameObject.lock()->GetGameComponent<GravityCore>()->AddGravity();
		}
	}
}
