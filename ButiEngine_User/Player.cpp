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
	//ポーズ中かパネル操作モードだったら
	if (PauseManager::IsPause() ||
		StageManager::GetMode() == GameMode::Edit)
	{
		return;
	}

	OnSwallowedFrog();
	
	if (IsOnEvent())
	{
		//矢印のスケールをゼロにして見えなくする
		if (wkp_predictionLine.lock())
		{
			wkp_predictionLine.lock()->transform->SetLocalScale(Vector3::Zero);
		}
		return; 
	}
	if (wkp_predictionLine.lock())
	{
		wkp_predictionLine.lock()->transform->SetLocalScale(Vector3(25.0f, 0.5f, 1.0f));
	}

	//タイトルプレイヤーが出来たらここ直す
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
	//
	Move();
	CheckGravity();
	Animation();
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::Start()
{
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

	wkp_apple = GetManager().lock()->AddObjectFromCereal("Apple");
	wkp_apple.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_apple.lock()->transform->SetLocalPosition(Vector3(0, 0.5f, 0.001f));
	wkp_apple.lock()->transform->SetLocalScale(Vector3(0, 0, 0));
	

	gameObject.lock()->RegistReactionComponent(GetThis<GameComponent>());

	se_not = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/PanelLimit.wav");
	se_dash = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Dash.wav");
	se_grab = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Grab.wav");
	se_jump = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Jump.wav");
	se_put = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Put.wav");
	se_land = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Land.wav");
	se_reverse = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/GravityReverse.wav");
	se_powerUp = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/GravityPowerUp.wav");
	se_frogIn = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/sita_Shrink.wav");
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

	if (isTutorial) return;

	if (!isClear)
	{
		if (InputManager::OnPushRightKey())
		{
			animation = ButiEngine::Player::WALK;
			velocity.x = 2.0f;
		}
		else if (InputManager::OnPushLeftKey())
		{
			animation = ButiEngine::Player::WALK;
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
		if ((InputManager::OnTriggerJumpKey() || jumpInputFrame > 0) && !isClear)
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
		}
	}
	else
	{
		if (InputManager::OnTriggerGrabKey())
		{
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_not, GameSettings::masterVolume);
		}
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
	
	OnJump();

	hitCore = false;
	hitGoal = false;
	hitSita = false;

	//X方向とY方向の移動量を比べて大きいほうから当たっているか計算する
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
			StringHelper::Contains(hitObjects[0]->GetGameObjectName(), "Sita") ||
			StringHelper::Contains(hitObjects[0]->GetGameObjectName(), "Frog") ||
			StringHelper::Contains(hitObjects[0]->GetGameObjectName(), "Gravity"))
		{
			grounded = false;
		}
	}

}

void ButiEngine::Player::MoveX()
{
	//移動
	gameObject.lock()->transform->TranslateX(velocity.x);
	//当たり判定更新
	shp_AABB->Update();
	shp_bottomAABB->Update();
	//メビウスの輪でループする用のクローンの当たり判定更新
	shp_mobiusLoop->UpdateAABB();
	//右側のクローンの押し戻し処理
	shp_mobiusLoop->BackXRight(velocity);
	shp_AABB->Update();
	//左側のクローンの押し戻し処理
	shp_mobiusLoop->BackXLeft(velocity);
	shp_AABB->Update();
	//自分の押し戻し処理
	BackX();
}

void ButiEngine::Player::MoveY()
{
	//移動
	gameObject.lock()->transform->TranslateY(velocity.y);
	//当たり判定更新
	shp_AABB->Update();
	shp_bottomAABB->Update();
	//メビウスの輪でループする用のクローンの当たり判定更新
	shp_mobiusLoop->UpdateAABB();
	//右側のクローンの押し戻し処理
	shp_mobiusLoop->BackYRight(velocity, gravity);
	shp_AABB->Update();
	//左側のクローンの押し戻し処理
	shp_mobiusLoop->BackYLeft(velocity, gravity);
	shp_AABB->Update();
	//自分の押し戻し処理
	BackY();
}

void ButiEngine::Player::BackX()
{
	//当たっているオブジェクトを取得
	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_AABB, 0);
	//何かと当たっていたら
	if (hitObjects.size() != 0)
	{
		auto end = hitObjects.end();
		for (auto itr = hitObjects.begin(); itr != end; ++itr)
		{
			//当たっているオブジェクトが自分だったら
			if ((*itr) == gameObject.lock()) { continue; }
			//当たっているオブジェクトがゴールだったら
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Goal"))
			{
				OnCollisionGoal((*itr));
				continue; 
			}
			//当たっているオブジェクトが重力コアだったら
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "GravityCore"))
			{
				OnCollisionCore((*itr));
				continue;
			}
			//当たっているオブジェクトがカエルの舌の先端部分だったら
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Sita_sentan"))
			{
				OnCollisionSita((*itr));
				continue;
			}

			float widthHalf = (*itr)->transform->GetWorldScale().x * 0.5f;
			float backLength = 0.0f;
			//右に移動していたら
			if (velocity.x > 0)
			{
				//押し戻す距離を計算
				backLength = (*itr)->transform->GetWorldPosition().x - widthHalf - GameSettings::blockSize * 0.5f - gameObject.lock()->transform->GetWorldPosition().x;
			}
			//左に移動していたら
			else if (velocity.x < 0)
			{
				//押し戻す距離を計算
				backLength = (*itr)->transform->GetWorldPosition().x + widthHalf + GameSettings::blockSize * 0.5f - gameObject.lock()->transform->GetWorldPosition().x;
			}
			//計算した距離分押し戻す
			gameObject.lock()->transform->TranslateX(backLength);
			//当たり判定更新
			shp_AABB->Update();
			shp_bottomAABB->Update();
			//X方向の移動量をゼロにする
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

void ButiEngine::Player::OnSwallowedFrog()
{
	if (!wkp_swallowFrog.lock()) { return; }
	if (wkp_swallowFrog.lock()->GetGameComponent<Frog>()->IsExplosion())
	{
		gameObject.lock()->transform->SetLocalScale(defaultScale);

		wkp_apple.lock()->transform->SetLocalScale(Vector3(1, 1, 1));
	}
}

void ButiEngine::Player::GrabGoal(std::weak_ptr<GameObject> arg_goal)
{
	auto core = wkp_holdCore.lock();
	auto sita = wkp_holdSita.lock();
	auto goal = wkp_holdGoal.lock();
	bool noGrab = !core  && !sita && !goal;
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
	auto sita = wkp_holdSita.lock();
	auto goal = wkp_holdGoal.lock();
	bool noGrab = !core && !sita && !goal;
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
	if (wkp_swallowFrog.lock()) { return; }
	auto sita = arg_frog.lock()->GetGameComponent<Frog>()->GetSitaSentan();
	if (wkp_holdSita.lock())
	{
		if (wkp_holdSita.lock() != sita.lock()) { return; }
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
		wkp_swallowFrog = arg_frog;
		defaultScale = gameObject.lock()->transform->GetLocalScale();
		gameObject.lock()->transform->SetLocalScale(Vector3(0, 0, 0));
		shp_contorolManager->Clear();
		return;
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

bool ButiEngine::Player::IsOnEvent()
{
	//テキストウィンドウが消えていなかったら
	if (!TalkText::IsDelete()) { return true; }
	if (TalkText::IsNotMove()) { return true; }
	//カエルに飲み込まれていたら
	if (wkp_swallowFrog.lock()) { return true; }
	//カエルの舌をつかんでいたら
	if (wkp_holdSita.lock()) { return true; }
	//ステージをクリアしていたら
	if (isClear) { return true; }
	return false;
}

void ButiEngine::Player::GrabSita(std::weak_ptr<GameObject> arg_sita)
{
	auto core = wkp_holdCore.lock();
	auto sita = wkp_holdSita.lock();
	auto goal = wkp_holdGoal.lock();
	bool noGrab = !core && !sita && !goal;
	if (noGrab && !pushGrabKeyFrame)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_frogIn, GameSettings::masterVolume);
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_grab, GameSettings::masterVolume);
		wkp_holdSita = arg_sita;
		sitaDifference = wkp_holdSita.lock()->transform->GetWorldPosition() - gameObject.lock()->transform->GetWorldPosition();
		gameObject.lock()->transform->SetWorldPostionZ(GameSettings::frogZ + 0.05f);
		auto frog_ = wkp_holdSita.lock()->GetGameComponent<SitaSentan>()->GetFrog();
		auto frogComponent = frog_.lock()->GetGameComponent<Frog>();
		frogComponent->PlayAnimation();
		frogComponent->GetBackFrog().lock()->GetGameComponent<Frog>()->PlayAnimation();
		GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<ScrollManager>()->SetIsActive(false);
		hitSita = false;
	}
}

void ButiEngine::Player::FollowSita()
{
	if (isClear) { return; }
	Vector3 targetPos = wkp_holdSita.lock()->transform->GetWorldPosition() - sitaDifference;
	gameObject.lock()->transform->SetWorldPostionX(targetPos.x);
	gameObject.lock()->transform->SetWorldPostionY(targetPos.y);
	shp_AABB->Update();
	shp_bottomAABB->Update();
}
