﻿#include "stdafx_u.h"
#include "Player.h"
#include"GameSettings.h"
#include"PauseManager.h"
#include"MobiusLoop.h"
#include"PanelManager.h"
#include"InputManager.h"

void ButiEngine::Player::OnUpdate()
{
	if (shp_pauseManager->GetPause())
	{
		if (!gameObject.lock()->transform->GetBaseTransform())
		{
			StoreClosestPanel();
			//Vector3 panelPos = wkp_closestPanel.lock()->transform->GetWorldPosition();
			//panelPos.z = -5.0f;
			//wkp_closestPanel.lock()->transform->SetWorldPosition(panelPos);
			//�Ȃ񂩔����ɂ�����Ȃ񂩔����ɂ�����Ȃ񂩔����ɂ�����Ȃ񂩔����ɂ�����Ȃ񂩔����ɂ�����Ȃ񂩔����ɂ����
			gameObject.lock()->transform->SetBaseTransform(wkp_closestPanel.lock()->transform);
		}
		return;
	}
	else
	{
		gameObject.lock()->transform->SetBaseTransform(nullptr);
	}
	Controll();
	Move();
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	shp_panelManager = GetManager().lock()->GetGameObject("PanelManager").lock()->GetGameComponent<PanelManager>();
	shp_mobiusLoop = gameObject.lock()->GetGameComponent<MobiusLoop>();
	shp_AABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.999f, 0.999f, 1.0f), gameObject.lock()->transform);
	//wkp_screenScroll = GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<LightVariable>("LightBuffer");

	velocity = Vector3::Zero;
	speed = 3.0f;
	grounded = false;
	gravity = -0.2f;
	jumpForce = 2.5f;
	isClear = false;

	wkp_predictionLine = GetManager().lock()->AddObjectFromCereal("PredictionLine");
	wkp_predictionLine.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);

	wkp_bottom = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(), "Player_Bottom");
	wkp_bottom.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_bottom.lock()->transform->SetLocalPosition(Vector3(0.0f, -0.75f, 0.0f));
	wkp_bottom.lock()->transform->SetLocalScale(Vector3(1.0f, 0.5f, 1.0f));
	
	shp_bottomAABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.999f, 1.0f, 1.0f), wkp_bottom.lock()->transform);
	

	gameObject.lock()->RegistReactionComponent(GetThis<GameComponent>());
}

void ButiEngine::Player::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Player::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Player::OnCollisionEnd(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Player::ShowGUI()
{
}

void ButiEngine::Player::OnShowUI()
{
	GUI::SliderFloat("gravity", &gravity, -1.0f, 1.0f);
	GUI::SliderFloat("jumpForce", &jumpForce, -10.0f, 10.0f);
	GUI::SliderFloat("speed", &speed, 0.0f, 50.0f);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::Controll()
{
	velocity.x = 0.0f;
	if (InputManager::OnPushRightKey())
	{
		velocity.x = 1.0f;
	}
	else if (InputManager::OnPushLeftKey())
	{
		velocity.x = -1.0f;
	}

	if (grounded)
	{
		if (InputManager::OnTriggerJumpKey())
		{
			velocity.y = jumpForce;
			grounded = false;
		}
	}
	else
	{
		velocity.y += gravity;
	}
}

void ButiEngine::Player::StoreClosestPanel()
{
	float x = gameObject.lock()->transform->GetWorldPosition().x;
	wkp_closestPanel = shp_panelManager->GetClosestPanel(x);
}

void ButiEngine::Player::Move()
{
	velocity.x *= speed;

	if (fabsf(velocity.x) > fabsf(velocity.y))
	{
		MoveX();
		MoveY();
	}
	else
	{
		MoveY();
		MoveX();
	}

	shp_AABB->Update();
	shp_bottomAABB->Update();

	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_bottomAABB, 0);
	if (hitObjects.size() == 0)
	{
		grounded = false;
	}

}

void ButiEngine::Player::MoveX()
{
	//if (velocity.x == 0) { return; }
	gameObject.lock()->transform->TranslateX(velocity.x);
	shp_AABB->Update();
	shp_bottomAABB->Update();
	shp_mobiusLoop->UpdateAABB();
	shp_mobiusLoop->BackXRight(velocity);
	shp_AABB->Update();
	shp_mobiusLoop->BackXLeft(velocity);
	shp_AABB->Update();
	BackX();
}

void ButiEngine::Player::MoveY()
{
	//if (velocity.y == 0) { return; }
	gameObject.lock()->transform->TranslateY(velocity.y);
	shp_AABB->Update();
	shp_bottomAABB->Update();
	shp_mobiusLoop->UpdateAABB();
	shp_mobiusLoop->BackYRight(velocity);
	shp_AABB->Update();
	shp_mobiusLoop->BackYLeft(velocity);
	shp_AABB->Update();
	BackY();
}

void ButiEngine::Player::BackX()
{
	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_AABB, 0);

	if (hitObjects.size() != 0)
	{
		auto end = hitObjects.end();
		for (auto itr = hitObjects.begin(); itr != end; ++itr)
		{
			if ((*itr) == gameObject.lock()) { continue; }
			if ((*itr)->GetGameObjectName() == "Goal") 
			{
				//ゴール時処理
				isClear = true;
				continue; 
			}

			float widthHalf = (*itr)->transform->GetWorldScale().x * 0.5f;
			if (velocity.x > 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x - widthHalf - GameSettings::blockSize * 0.5f - gameObject.lock()->transform->GetWorldPosition().x;
				gameObject.lock()->transform->TranslateX(backLength);
				shp_AABB->Update();
				shp_bottomAABB->Update();
			}
			else if (velocity.x < 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x + widthHalf + GameSettings::blockSize * 0.5f - gameObject.lock()->transform->GetWorldPosition().x;
				gameObject.lock()->transform->TranslateX(backLength);
				shp_AABB->Update();
				shp_bottomAABB->Update();
			}
			velocity.x = 0;
		}
	}
}

void ButiEngine::Player::BackY()
{
	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_AABB, 0);

	if (hitObjects.size() != 0)
	{
		auto end = hitObjects.end();
		for (auto itr = hitObjects.begin(); itr != end; ++itr)
		{
			if ((*itr) == gameObject.lock()) { continue; }
			if ((*itr)->GetGameObjectName() == "Goal") 
			{
				//ゴール時処理
				isClear = true;
				continue; 
			}

			if (velocity.y > 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().y - GameSettings::blockSize - gameObject.lock()->transform->GetWorldPosition().y;
				gameObject.lock()->transform->TranslateY(backLength);
				shp_AABB->Update();
				shp_bottomAABB->Update();
				grounded = true;
			}
			else if (velocity.y < 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().y + GameSettings::blockSize - gameObject.lock()->transform->GetWorldPosition().y;
				gameObject.lock()->transform->TranslateY(backLength);
				shp_AABB->Update();
				shp_bottomAABB->Update();
				grounded = true;
			}
			velocity.y = 0;
		}
	}
}
