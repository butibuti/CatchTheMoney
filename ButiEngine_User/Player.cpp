#include "stdafx_u.h"
#include "Player.h"
#include"GameSettings.h"
#include"PauseManager.h"
#include"MobiusLoop.h"
#include"PanelManager.h"
#include"InputManager.h"
#include"Panel.h"
#include"FollowPanel.h"
#include"GravityCore.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"

void ButiEngine::Player::OnUpdate()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::Colon))
	{
		isClear = true;
	}
	if (shp_pauseManager->GetPause())
	{
		return;
	}

	Controll();
	Move();
	CheckGravity();
	Animation();
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	shp_panelManager = GetManager().lock()->GetGameObject("PanelManager").lock()->GetGameComponent<PanelManager>();
	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	shp_mobiusLoop = gameObject.lock()->GetGameComponent<MobiusLoop>();
	shp_AABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.999f, 0.999f, 10.0f), gameObject.lock()->transform);
	//wkp_screenScroll = GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<LightVariable>("LightBuffer");

	velocity = Vector3::Zero;
	speed = 3.0f;
	grounded = true;
	gravity = -0.2f;
	pushGrabKeyFrame = false;
	isClear = false;
	progressFrame = 0;
	animationFrame = 0;
	animation = ButiEngine::Player::IDLE;

	wkp_predictionLine = GetManager().lock()->AddObjectFromCereal("PredictionLine");
	wkp_predictionLine.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);

	wkp_bottom = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(), "Player_Bottom");
	wkp_bottom.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_bottom.lock()->transform->SetLocalPosition(Vector3(0.0f, -0.75f, 0.0f));
	wkp_bottom.lock()->transform->SetLocalScale(Vector3(1.0f, 0.5f, 1.0f));
	
	shp_bottomAABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.999f, 0.999f, 1.0f), wkp_bottom.lock()->transform);
	

	gameObject.lock()->RegistReactionComponent(GetThis<GameComponent>());

	se_dash = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Dash.wav");
	se_grab = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Grab.wav");
	se_jump = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Jump.wav");
	se_orosu = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Orosu.wav");
	se_land = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Land.wav");
	se_reverse = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/GravityReverse.wav");
	se_powerUp = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/GravityPowerUp.wav");
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
	GUI::SliderFloat("speed", &speed, 0.0f, 50.0f);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::Controll()
{
	animation = ButiEngine::Player::IDLE;
	progressFrame++;
	if (progressFrame < FREEZE_FRAME)
	{
		return;
	}

	velocity.x = 0.0f;

	if (!isClear)
	{
		if (InputManager::OnTriggerRightKey() || InputManager::OnTriggerLeftKey())
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_dash, 1.0f);
		}
		if (InputManager::OnPushRightKey())
		{
			animation = ButiEngine::Player::WALK;
			velocity.x = 1.0f;
		}
		else if (InputManager::OnPushLeftKey())
		{
			animation = ButiEngine::Player::WALK;
			velocity.x = -1.0f;
		}
		Vector3 scale = gameObject.lock()->transform->GetLocalScale();
		if (velocity.x != 0 && scale.x > 0 != velocity.x > 0)
		{
			scale.x *= -1;
		}
		gameObject.lock()->transform->SetLocalScale(scale);
	}

	if (grounded)
	{
		if (InputManager::OnTriggerJumpKey() && !isClear)
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_jump, 1.0f);
			velocity.y = JUMP_FORCE;
			if (gravity > 0)
			{
				velocity.y *= -1;
			}
			grounded = false;
		}
		if (InputManager::OnTriggerGrabKey())
		{
			ReleaseGravityCore();
		}
	}
	else
	{
		shp_spriteAnimation->SetHorizontalAnim(0);
		animation = ButiEngine::Player::JUMP;
	}
}

void ButiEngine::Player::CheckGravity()
{
	auto closestPanel = gameObject.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel();
	float previousGravity = gravity;
	if (closestPanel.lock())
	{
		auto panelComponent = closestPanel.lock()->GetGameComponent<Panel>();
		if (wkp_holdCore.lock())
		{
			auto core = wkp_holdCore.lock()->GetGameComponent<GravityCore>();
			if (!panelComponent->ContainsGravityCore(core->GetCoreNum()))
			{
				return;
			}
		}
		int coreCount = panelComponent->GetGravityCoreCount();
		gravity = panelComponent->GetGravity();
		if (gravity == 0)
		{
			if (previousGravity < 0)
			{
				gravity = -0.2f;
			}
			else if (previousGravity > 0)
			{
				gravity = 0.2f;
			}
		}
	}

	if ((gravity > 0) != (previousGravity > 0))
	{
		Vector3 scale = gameObject.lock()->transform->GetLocalScale();
		scale.y *= -1;
		gameObject.lock()->transform->SetLocalScale(scale);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_reverse, 1.0f);
	}
	else if (abs(gravity) > abs(previousGravity))
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_powerUp, 1.0f);
	}
}

void ButiEngine::Player::Move()
{
	if(!grounded)
	{
		velocity.y += gravity;
	}
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
			if ((*itr)->GetGameObjectName().find("Goal") != std::string::npos)
			{
				OnCollisionGoal((*itr));
				continue; 
			}
			if ((*itr)->GetGameObjectName().find("GravityCore") != std::string::npos)
			{
				OnCollisionCore((*itr));
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
			if ((*itr)->GetGameObjectName().find("Goal") != std::string::npos)
			{
				OnCollisionGoal((*itr));
				continue;
			}
			if ((*itr)->GetGameObjectName().find("GravityCore") != std::string::npos)
			{
				continue;
			}

			if (velocity.y > 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().y - GameSettings::blockSize - gameObject.lock()->transform->GetWorldPosition().y;
				gameObject.lock()->transform->TranslateY(backLength);
				shp_AABB->Update();
				shp_bottomAABB->Update();
				if (velocity.y >= 0 == gravity >= 0)
				{
					GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_land, 1.0f);
					grounded = true;
				}
			}
			else if (velocity.y < 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().y + GameSettings::blockSize - gameObject.lock()->transform->GetWorldPosition().y;
				gameObject.lock()->transform->TranslateY(backLength);
				shp_AABB->Update();
				shp_bottomAABB->Update();
				if (velocity.y >= 0 == gravity >= 0)
				{
					GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_land, 1.0f);
					grounded = true;
				}
			}
			velocity.y = 0;
		}
	}
}

void ButiEngine::Player::Animation()
{
	animationFrame++;
	shp_spriteAnimation->SetVarticalAnim(animation); //jump
	if (animationFrame < ANIMATION_RATE) return;

	animationFrame = 0;

	switch (animation)
	{
	case ButiEngine::Player::IDLE:
		shp_spriteAnimation->UpdateHorizontalAnim(1);
		break;
	case ButiEngine::Player::WALK:
		shp_spriteAnimation->UpdateHorizontalAnim(1);
		break;
	case ButiEngine::Player::JUMP:
		shp_spriteAnimation->UpdateHorizontalAnim(0);
		break;
	default:
		break;
	}
}

void ButiEngine::Player::GrabGravityCore(std::weak_ptr<GameObject> arg_core)
{
	if (!wkp_holdCore.lock() && !pushGrabKeyFrame)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_grab, 1.0f);
		int closestPanelNum = gameObject.lock()->GetGameComponent<FollowPanel>()
			->GetClosestPanel().lock()->GetGameComponent<Panel>()->GetPanelNum();
		int coreClosestPanelNum = arg_core.lock()->GetGameComponent<FollowPanel>()
			->GetClosestPanel().lock()->GetGameComponent<Panel>()->GetPanelNum();

		if (closestPanelNum != coreClosestPanelNum) { return; }
		wkp_holdCore = arg_core;
		wkp_holdCore.lock()->GetGameComponent<GravityCore>()->SetGrabbed(true);
	}
}

void ButiEngine::Player::ReleaseGravityCore()
{
	if (wkp_holdCore.lock())
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_orosu, 1.0f);
		wkp_holdCore.lock()->GetGameComponent<GravityCore>()->SetGrabbed(false);
		Vector3 corePos = gameObject.lock()->transform->GetWorldPosition();
		int coreNum = wkp_holdCore.lock()->GetGameComponent<GravityCore>()->GetCoreNum();
		corePos.z = GameSettings::coreZ - 0.001f * coreNum;
		wkp_holdCore.lock()->transform->SetWorldPosition(corePos);
		wkp_holdCore = std::weak_ptr<GameObject>();
		pushGrabKeyFrame = true;
	}
}

void ButiEngine::Player::OnCollisionGoal(std::weak_ptr<GameObject> arg_goal)
{
	isClear = true;
}

void ButiEngine::Player::OnCollisionCore(std::weak_ptr<GameObject> arg_core)
{
	if (InputManager::OnTriggerGrabKey() && grounded)
	{
		GrabGravityCore(arg_core);
	}
}
