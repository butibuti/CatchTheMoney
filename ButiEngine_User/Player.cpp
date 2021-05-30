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
#include "TalkText.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include"StageManager.h"
#include"Frog.h"
#include"SitaSentan.h"
#include"ScrollManager.h"
#include "ContorolByStick.h"
#include "StageSelect.h"

void ButiEngine::Player::OnUpdate()
{
	if (shp_pauseManager->IsPause() ||
		StageManager::GetMode() == GameMode::Edit ||
		isClear ||
		!TalkText::IsDelete() ||
		TalkText::IsNotMove()||
		wkp_holdSita.lock())
	{

		return;
	}

	if (!GameSettings::isTitle)
	{
		Control();
	}
	else
	{
		freeze = false;
		animation = ButiEngine::Player::WALK;
		velocity.x = 1.0f;
	}
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
	shp_contorolManager = GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<ContorolByStick>();
	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	shp_mobiusLoop = gameObject.lock()->GetGameComponent<MobiusLoop>();
	shp_AABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.499f, 0.499f, 10.0f), gameObject.lock()->transform);

	velocity = Vector3::Zero;
	sitaDifference = Vector3::Zero;
	grounded = true;
	gravity = -GameSettings::gravity;
	pushGrabKeyFrame = false;
	isClear = false;
	isTutorial = false;
	freezeProgressFrame = 0;
	jumpFrame = 0;
	animationFrame = 0;
	animation = ButiEngine::Player::IDLE;
	freeze = true;
	jump = false;
	jumpInputFrame = 0;
	hitCore = false;
	hitFrog = false;
	hitGoal = false;
	hitSita = false;

	if (!GameSettings::isTitle)
	{
		wkp_predictionLine = GetManager().lock()->AddObjectFromCereal("PredictionLine");
		wkp_predictionLine.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
		wkp_predictionLine.lock()->transform->SetWorldPostionZ(GameSettings::predictionLineZ);
	}

	wkp_bottom = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(), "Player_Bottom");
	wkp_bottom.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_bottom.lock()->transform->SetLocalPosition(Vector3(0.0f, -0.5f, 0.0f));
	wkp_bottom.lock()->transform->SetLocalScale(Vector3(1.0f, 0.5f, 10.0f));
	
	shp_bottomAABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.499f, 0.499f, 1.0f), wkp_bottom.lock()->transform);
	

	gameObject.lock()->RegistReactionComponent(GetThis<GameComponent>());

	se_dash = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Dash.wav");
	se_grab = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Grab.wav");
	se_jump = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Jump.wav");
	se_put = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Put.wav");
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
	GUI::Text("velY    : %f", velocity.y);
	GUI::Text("gravity : %f", gravity);
}

void ButiEngine::Player::ReverseGravity()
{
	gravity = -gravity;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::Control()
{
	animation = ButiEngine::Player::IDLE;
	if (freeze)
	{
		freezeProgressFrame++;
		if (freezeProgressFrame >= FREEZE_FRAME)
		{
			freeze = false;
		}
		return;
	}
	auto positionAnimation = gameObject.lock()->GetGameComponent<PositionAnimation>();
	if (positionAnimation) { return; }

	pushGrabKeyFrame = false;
	velocity.x = 0.0f;

	if (!isClear)
	{
		if (InputManager::OnTriggerRightKey() || InputManager::OnTriggerLeftKey())
		{
			//GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_dash, 0.1f);
		}
		if (InputManager::OnPushRightKey())
		{
			if (!wkp_holdFrog.lock())
			{
				animation = ButiEngine::Player::WALK;
			}
			velocity.x = 2.0f;
		}
		else if (InputManager::OnPushLeftKey())
		{
			if (!wkp_holdFrog.lock())
			{
				animation = ButiEngine::Player::WALK;
			}
			velocity.x = -2.0f;
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
		if ((InputManager::OnTriggerJumpKey() || jumpInputFrame > 0) && !isClear && !wkp_holdFrog.lock())
		{
			jumpInputFrame = 0;
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_jump, GameSettings::masterVolume);
			velocity.y = JUMP_FORCE;
			if (gravity > 0)
			{
				velocity.y *= -1;
			}
			grounded = false;
			jump = true;
		}
		if (InputManager::OnTriggerGrabKey())
		{
			ReleaseGravityCore();
			ReleaseFrog();
		}
	}
	else
	{
		if (InputManager::OnTriggerJumpKey())
		{
			jumpInputFrame = COYOTE_TIME;
		}
		if (jumpInputFrame > 0)
		{
			jumpInputFrame--;
		}
		shp_spriteAnimation->SetHorizontalAnim(0);
		shp_mobiusLoop->GetRight().lock()->GetGameComponent<SpliteAnimationComponent>()->SetHorizontalAnim(0);
		shp_mobiusLoop->GetLeft().lock()->GetGameComponent<SpliteAnimationComponent>()->SetHorizontalAnim(0);
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
				gravity = -GameSettings::gravity;
			}
			else if (previousGravity > 0)
			{
				gravity = GameSettings::gravity;
			}
		}
	}

	if ((gravity > 0) != (previousGravity > 0))
	{
		Vector3 scale = gameObject.lock()->transform->GetLocalScale();
		scale.y *= -1;
		gameObject.lock()->transform->SetLocalScale(scale);
		//GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_reverse, 0.1f);
	}
	else if (abs(gravity) > abs(previousGravity))
	{
		//GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_powerUp, 0.1f);
	}
}

void ButiEngine::Player::OnJump()
{
	if (jumpFrame != 0)
	{
		jumpFrame--;
	}
	float prevVelY = velocity.y;
	if (!grounded && jumpFrame == 0)
	{
		velocity.y += gravity;
	}
	if (jump && jumpFrame == 0)
	{
		if ((gravity < 0 && ((prevVelY > 0) != (velocity.y > 0))) ||
			(gravity > 0 && ((prevVelY < 0) != (velocity.y < 0))))
		{
			velocity.y = 0;
			jumpFrame = FLOATING_FRAME;
		}
	}
}

void ButiEngine::Player::Move()
{
	if (freeze) { return; }
	if (wkp_holdFrog.lock()) { return; }
	
	OnJump();
	hitCore = false;
	hitFrog = false;
	hitGoal = false;
	hitSita = false;

	if (abs(velocity.x) > abs(velocity.y))
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
	else if (hitObjects.size() == 1)
	{
		if (StringHelper::Contains(hitObjects[0]->GetGameObjectName(), "Goal") ||
			StringHelper::Contains(hitObjects[0]->GetGameObjectName(), "Sita"))
		{
			grounded = false;
		}
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
	shp_mobiusLoop->BackYRight(velocity, gravity);
	shp_AABB->Update();
	shp_mobiusLoop->BackYLeft(velocity, gravity);
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
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Goal"))
			{
				OnCollisionGoal((*itr));
				continue; 
			}
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "GravityCore"))
			{
				OnCollisionCore((*itr));
				continue;
			}
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Sita_sentan"))
			{
				OnCollisionSita((*itr));
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
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Goal"))
			{
				OnCollisionGoal((*itr));
				continue;
			}
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "GravityCore"))
			{
				continue;
			}
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Sita_sentan"))
			{
				OnCollisionSita((*itr));
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
					//GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_land, 0.1f);
					grounded = true;
					jump = false;
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
					//GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_land, 0.1f);
					grounded = true;
					jump = false;
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
	shp_mobiusLoop->GetRight().lock()->GetGameComponent<SpliteAnimationComponent>()->SetVarticalAnim(animation);
	shp_mobiusLoop->GetLeft().lock()->GetGameComponent<SpliteAnimationComponent>()->SetVarticalAnim(animation);
	if (animationFrame < ANIMATION_RATE) return;

	animationFrame = 0;

	switch (animation)
	{
	case ButiEngine::Player::IDLE:
		shp_spriteAnimation->UpdateHorizontalAnim(1);
		shp_mobiusLoop->GetRight().lock()->GetGameComponent<SpliteAnimationComponent>()->UpdateHorizontalAnim(1);
		shp_mobiusLoop->GetLeft().lock()->GetGameComponent<SpliteAnimationComponent>()->UpdateHorizontalAnim(1);
		break;
	case ButiEngine::Player::WALK:
		shp_spriteAnimation->UpdateHorizontalAnim(1);
		shp_mobiusLoop->GetRight().lock()->GetGameComponent<SpliteAnimationComponent>()->UpdateHorizontalAnim(1);
		shp_mobiusLoop->GetLeft().lock()->GetGameComponent<SpliteAnimationComponent>()->UpdateHorizontalAnim(1);
		break;
	case ButiEngine::Player::JUMP:
		shp_spriteAnimation->UpdateHorizontalAnim(0);
		shp_mobiusLoop->GetRight().lock()->GetGameComponent<SpliteAnimationComponent>()->UpdateHorizontalAnim(0);
		shp_mobiusLoop->GetLeft().lock()->GetGameComponent<SpliteAnimationComponent>()->UpdateHorizontalAnim(0);
		break;
	default:
		break;
	}
}

void ButiEngine::Player::CorrectionFrog(std::weak_ptr<GameObject> arg_frog)
{
	auto anim_ = gameObject.lock()->GetGameComponent<PositionAnimation>();
	if (anim_)
	{
		gameObject.lock()->RemoveGameComponent("PositionAnimation");
	}
	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();

	anim->SetInitPosition(gameObject.lock()->transform->GetWorldPosition());

	Vector3 targetPosition = gameObject.lock()->transform->GetWorldPosition();
	targetPosition.x = wkp_holdFrog.lock()->transform->GetWorldPosition().x;
	anim->SetTargetPosition(targetPosition);
	anim->SetSpeed(1.0f / 30.0f);
	anim->SetEaseType(Easing::EasingType::Liner);
}

void ButiEngine::Player::GrabGoal(std::weak_ptr<GameObject> arg_goal)
{
	auto core = wkp_holdCore.lock();
	auto frog = wkp_holdFrog.lock();
	auto sita = wkp_holdSita.lock();
	auto goal = wkp_holdGoal.lock();
	bool noGrab = !core && !frog && !sita && !goal;
	if (noGrab && !pushGrabKeyFrame)
	{
		Vector3 playerPos = gameObject.lock()->transform->GetWorldPosition();
		Vector3 targetPos = arg_goal.lock()->transform->GetWorldPosition();
		float difference = 16.0f;

		if (gravity > 0)
		{
			difference *= -1;
		}

		targetPos.x = playerPos.x;
		targetPos.y = playerPos.y + difference;
		arg_goal.lock()->transform->SetWorldPostionX(targetPos.x);
		arg_goal.lock()->transform->SetWorldPostionY(targetPos.y);

		wkp_holdGoal = arg_goal;
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_grab, GameSettings::masterVolume);
		shp_contorolManager->Clear();
		if (StageSelect::GetStageNum() == TalkStageNum::REVERSE_TALK)
		{
			isTutorial = true;

		}
		else
		{
			isClear = true;
		}
	}
}

void ButiEngine::Player::GrabGravityCore(std::weak_ptr<GameObject> arg_core)
{
	auto core = wkp_holdCore.lock();
	auto frog = wkp_holdFrog.lock();
	auto sita = wkp_holdSita.lock();
	auto goal = wkp_holdGoal.lock();
	bool noGrab = !core && !frog && !sita && !goal;
	if (noGrab && !pushGrabKeyFrame)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_grab, GameSettings::masterVolume);

		wkp_holdCore = arg_core;
		wkp_holdCore.lock()->GetGameComponent<GravityCore>()->SetGrabbed(true);
	}
}

void ButiEngine::Player::ReleaseGravityCore()
{
	if (wkp_holdCore.lock())
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_put, GameSettings::masterVolume / 2.0f);
		wkp_holdCore.lock()->GetGameComponent<GravityCore>()->SetGrabbed(false);
		Vector3 corePos = gameObject.lock()->transform->GetWorldPosition();
		int coreNum = wkp_holdCore.lock()->GetGameComponent<GravityCore>()->GetCoreNum();
		corePos.z = GameSettings::coreZ - 0.001f * coreNum;
		wkp_holdCore.lock()->transform->SetWorldPosition(corePos);
		wkp_holdCore = std::weak_ptr<GameObject>();
		pushGrabKeyFrame = true;
	}
}

void ButiEngine::Player::GrabFrog(std::weak_ptr<GameObject> arg_frog)
{
	auto core = wkp_holdCore.lock();
	auto frog = wkp_holdFrog.lock();
	auto sita = wkp_holdSita.lock();
	auto goal = wkp_holdGoal.lock();
	bool noGrab = !core && !frog && !sita && !goal;
	if (noGrab && !pushGrabKeyFrame)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_grab, GameSettings::masterVolume);
		wkp_holdFrog = arg_frog;
		auto frog = wkp_holdFrog.lock()->GetGameComponent<Frog>();
		frog->SetGrabbed(true);
		frog->GetBackFrog().lock()->GetGameComponent<Frog>()->SetGrabbed(true);

		if (wkp_holdFrog.lock()->transform->GetWorldScale().x < 0 != gameObject.lock()->transform->GetWorldScale().x < 0)
		{
			Vector3 scale = gameObject.lock()->transform->GetLocalScale();
			scale.x *= -1;
			gameObject.lock()->transform->SetLocalScale(scale);
		}

		CorrectionFrog(wkp_holdFrog);
	}
}

void ButiEngine::Player::ReleaseFrog()
{
	if (wkp_holdFrog.lock())
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_put, GameSettings::masterVolume / 2.0f);
		auto frog = wkp_holdFrog.lock()->GetGameComponent<Frog>();
		frog->SetGrabbed(false);
		frog->GetBackFrog().lock()->GetGameComponent<Frog>()->SetGrabbed(false);
		Vector3 frogPos = wkp_holdFrog.lock()->transform->GetWorldPosition();
		frogPos.y = gameObject.lock()->transform->GetWorldPosition().y;
		wkp_holdFrog.lock()->transform->SetWorldPosition(frogPos);
		wkp_holdFrog = std::weak_ptr<GameObject>();
		pushGrabKeyFrame = true;
	}
}

void ButiEngine::Player::OnCollisionGoal(std::weak_ptr<GameObject> arg_goal)
{
	if (!grounded || velocity.y != 0) { return; }
	hitGoal = true;
	if (InputManager::OnTriggerGrabKey())
	{
		GrabGoal(arg_goal);
	}
}

void ButiEngine::Player::OnCollisionCore(std::weak_ptr<GameObject> arg_core)
{
	if (!grounded) { return; }

	int closestPanelNum = gameObject.lock()->GetGameComponent<FollowPanel>()
		->GetClosestPanel().lock()->GetGameComponent<Panel>()->GetPanelNum();
	int coreClosestPanelNum = arg_core.lock()->GetGameComponent<FollowPanel>()
		->GetClosestPanel().lock()->GetGameComponent<Panel>()->GetPanelNum();

	if (closestPanelNum == coreClosestPanelNum)
	{
		hitCore = true;
	}

	if (InputManager::OnTriggerGrabKey() && hitCore)
	{
		GrabGravityCore(arg_core);
	}
}

void ButiEngine::Player::OnCollisionFrog(std::weak_ptr<GameObject> arg_frog)
{
	if (!grounded) { return; }
	if (wkp_holdSita.lock())
	{
		Vector3 scale = gameObject.lock()->transform->GetLocalScale();
		float frogScaleY = arg_frog.lock()->transform->GetWorldScale().y;
		if (frogScaleY < 0 != scale.y < 0)
		{
			scale.y *= -1;
		}
		gameObject.lock()->transform->SetLocalScale(scale);
		Vector3 frogPos = arg_frog.lock()->transform->GetWorldPosition();
		frogPos.z = gameObject.lock()->transform->GetWorldPosition().z;
		gameObject.lock()->transform->SetWorldPosition(frogPos);
		isClear = true;
		shp_contorolManager->Clear();
		return;
	}

	int closestPanelNum = gameObject.lock()->GetGameComponent<FollowPanel>()
		->GetClosestPanel().lock()->GetGameComponent<Panel>()->GetPanelNum();
	int frogClosestPanelNum = arg_frog.lock()->GetGameComponent<FollowPanel>()
		->GetClosestPanel().lock()->GetGameComponent<Panel>()->GetPanelNum();

	if (closestPanelNum == frogClosestPanelNum)
	{
		hitFrog = true;
	}
	if (InputManager::OnTriggerGrabKey() && hitFrog)
	{
		GrabFrog(arg_frog);
	}
}

void ButiEngine::Player::OnCollisionSita(std::weak_ptr<GameObject> arg_sita)
{
	if (!grounded || velocity.y != 0 || wkp_holdSita.lock()) { return; }
	hitSita = true;
	if (InputManager::OnTriggerGrabKey())
	{
		GrabSita(arg_sita);
	}
}

void ButiEngine::Player::GrabSita(std::weak_ptr<GameObject> arg_sita)
{
	auto core = wkp_holdCore.lock();
	auto frog = wkp_holdFrog.lock();
	auto sita = wkp_holdSita.lock();
	auto goal = wkp_holdGoal.lock();
	bool noGrab = !core && !frog && !sita && !goal;
	if (noGrab && !pushGrabKeyFrame)
	{
		wkp_holdSita = arg_sita;
		sitaDifference = wkp_holdSita.lock()->transform->GetWorldPosition() - gameObject.lock()->transform->GetWorldPosition();
		gameObject.lock()->transform->SetWorldPostionZ(GameSettings::frogZ + 0.05f);
		auto frog = wkp_holdSita.lock()->GetGameComponent<SitaSentan>()->GetFrog();
		auto frogComponent = GetManager().lock()->GetGameObject("Frog").lock()->GetGameComponent<Frog>();
		frogComponent->PlayAnimation();
		frogComponent->GetBackFrog().lock()->GetGameComponent<Frog>()->PlayAnimation();
		GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<ScrollManager>()->SetIsActive(false);
		hitSita = false;
	}
}

void ButiEngine::Player::FollowSita()
{
	Vector3 targetPos = wkp_holdSita.lock()->transform->GetWorldPosition() - sitaDifference;
	gameObject.lock()->transform->SetWorldPostionX(targetPos.x);
	gameObject.lock()->transform->SetWorldPostionY(targetPos.y);
	shp_AABB->Update();
	shp_bottomAABB->Update();
}
