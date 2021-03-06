#include "stdafx_u.h"
#include "Frog.h"
#include"SitaSentan.h"
#include"SitaTyuukan.h"
#include"GameSettings.h"
#include"FollowPanel.h"
#include"Panel.h"
#include"PauseManager.h"
#include"StageManager.h"
#include"TalkText.h"
#include"Player.h"
#include "StageSelect.h"
#include "AngelFrog.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"
#include"FrogParts.h"

void ButiEngine::Frog::OnUpdate()
{
	StorePlayer();
	if (once)
	{
		gameObject.lock()->GetGameComponent<MeshDrawComponent>()->ReRegist();
		once = false;
	}
	if (shp_pauseManager->IsPause() ||
		StageManager::GetMode() == GameMode::Edit)
	{
		return;
	}
	Interlock();
	CheckNearPlayer();
	Animation();
	if (onceCount < 100 && StageSelect::GetStageNum() == TalkStageNum::FROG_TALK)
	{
		onceCount++;
		wkp_sita_sentan.lock()->GetGameComponent<SitaSentan>()->SetAnim(1);
		return;
	}
	SpriteAnimation();
	if (wkp_player.lock() && wkp_player.lock()->GetGameComponent<Player>()->GetHoldSita().lock())
	{
		if (wkp_player.lock()->GetGameComponent<Player>()->GetHoldSita().lock() == wkp_sita_sentan.lock())
		{
			CheckHitPlayer();
		}
		return;
	}
	if (!nearPlayer) { return; }
	MoveY();
	CheckGravity();
}

void ButiEngine::Frog::OnSet()
{
}

void ButiEngine::Frog::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	CreateSita();
	shp_AABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.499f, 0.499f, 10.0f), gameObject.lock()->transform);

	wkp_bottom = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(), "Frog_Bottom");
	wkp_bottom.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_bottom.lock()->transform->SetLocalPosition(Vector3(0.0f, -0.125f, 0.0f));
	wkp_bottom.lock()->transform->SetLocalScale(Vector3(1.0f, 0.25f, 1.0f));

	shp_bottomAABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.499f, 0.499f, 10.0f), wkp_bottom.lock()->transform);

	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();

	auto position = gameObject.lock()->transform->GetLocalPosition();
	position.y -= 3000;
	position.z -= 0.02f;
	auto rotation = gameObject.lock()->transform->GetLocalRotation();
	auto scale = gameObject.lock()->transform->GetLocalScale();
	wkp_angel = GetManager().lock()->AddObjectFromCereal("AngelFrog", ObjectFactory::Create<Transform>(position, rotation, scale));
	auto angelFrogComponent = wkp_angel.lock()->GetGameComponent<AngelFrog>();
	angelFrogComponent->Start();
	angelFrogComponent->SetIsActive(false);

	se_explosion = SoundTag("Sound/Kaeru_Explosion1.wav");
	se_extend = SoundTag("Sound/Kaeru_Extend.wav");
	se_sitaOut = SoundTag("Sound/sita_Extend.wav");
	se_eat = SoundTag("Sound/Kaeru_Eat.wav");

	int partType = 0;
	for (int i = 0; i < 12; i++)
	{
		auto partScale = Vector3(16.0f, 16.0f, 1.0f);
		std::string name = "FrogArm";
		if (partType == 1)
		{
			name = "FrogPart";
			partScale.x = 8.0f;
			partScale.y = 8.0f;
		}
		else if (partType == 2)
		{
			name = "FrogHara";
			partScale.x = 8.0f;
			partScale.y = 8.0f;
		}
		partType++;
		const int MAX_PART = 3;
		if (partType == MAX_PART)
		{
			partType = 0;
		}
		auto parts = GetManager().lock()->AddObjectFromCereal(name, ObjectFactory::Create<Transform>(position, Vector3::Zero, partScale));
		parts.lock()->GetGameComponent<FrogParts>()->SetFrog(gameObject);
		vec_wkp_parts.push_back(parts);
	}

	if (StageSelect::GetStageNum() == TalkStageNum::FROG_TALK)
	{
		animation = Animation::EAT_APPLE;
		shp_spriteAnimation->SetVarticalAnim(Frog::EAT_APPLE);
	}
	else
	{
		animation = Animation::SITA;
	}


	velocity = Vector3::Zero;
	grounded = false;
	nearPlayer = false;
	once = true;
	isAnimation = false;
	isApple = false;
	isSpawnAngel = false;
	isExplosion = false;
	isOutSita = false;
	progress = 0;
	animationFrame = 0;
	onceCount = 0;
	holdAppleCount = 0;
}

void ButiEngine::Frog::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Frog::Clone()
{
	return ObjectFactory::Create<Frog>();
}

void ButiEngine::Frog::SetDefaultGravity(bool arg_top)
{
	if (arg_top)
	{
		gravity = -GameSettings::gravity;
	}
	else
	{
		gravity = GameSettings::gravity;
	}
}

void ButiEngine::Frog::Exprosion()
{
	animation = Animation::EXPROSION;
}

void ButiEngine::Frog::CreateSita()
{
	wkp_sita_tyuukan = GetManager().lock()->AddObjectFromCereal("Sita_tyuukan");
	wkp_sita_sentan = GetManager().lock()->AddObjectFromCereal("Sita_sentan");

	auto tyuukan = wkp_sita_tyuukan.lock()->GetGameComponent<SitaTyuukan>();
	tyuukan->SetFrog(gameObject);
	tyuukan->SetSentan(wkp_sita_sentan);

	wkp_sita_sentan.lock()->GetGameComponent<SitaSentan>()->SetFrog(gameObject);

	wkp_sita_sentan.lock()->transform->SetBaseTransform(nullptr);
	wkp_sita_sentan.lock()->transform->SetLocalScale(Vector3(32.0f, 32.0f, 1));
	float sitaLength = GameSettings::blockSize * 50 - 5.5f;
	Vector3 sentanPosition = gameObject.lock()->transform->GetWorldPosition();
	float scaleX = gameObject.lock()->transform->GetWorldScale().x;
	if (scaleX < 0)
	{
		sentanPosition.x -= 5.5f;
	}
	else if (scaleX > 0)
	{
		sentanPosition.x += 5.5f;
	}
	if (StageSelect::GetStageNum() != TalkStageNum::FROG_TALK)
	{
		if (scaleX < 0)
		{
			sentanPosition.x -= sitaLength;
		}
		else if (gameObject.lock()->transform->GetWorldScale().x > 0)
		{
			sentanPosition.x += sitaLength;
		}
	}
	sentanPosition.z -= 0.3f;
	wkp_sita_sentan.lock()->GetGameComponent<SitaSentan>()->SetAddZ(-0.3f);

	
	wkp_sita_sentan.lock()->transform->SetWorldPosition(sentanPosition);
}

void ButiEngine::Frog::CheckGravity()
{
	auto closestPanel = gameObject.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel();
	float previousGravity = gravity;
	if (closestPanel.lock())
	{
		auto panelComponent = closestPanel.lock()->GetGameComponent<Panel>();
		//?p?l?????d????????????
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
	//?d???????]???????J?G?????????????]??????
	if ((gravity > 0) != (previousGravity > 0))
	{
		Vector3 scale = gameObject.lock()->transform->GetLocalScale();
		scale.y *= -1;
		gameObject.lock()->transform->SetLocalScale(scale);
	}
}

void ButiEngine::Frog::CheckNearPlayer()
{
	if (!wkp_player.lock() || nearPlayer) { return; }
	if (wkp_player.lock()->GetGameComponent<Player>()->GetHoldSita().lock()) { return; }

	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	Vector3 playerPos = wkp_player.lock()->transform->GetWorldPosition();

	if (position.x < 0 == playerPos.x < 0)
	{
		nearPlayer = true;
		wkp_backFrog.lock()->GetGameComponent<Frog>()->SetNearPlayer(false);
	}
}

void ButiEngine::Frog::MoveY()
{
	if (grabbed) { return; }
	if (!grounded)
	{
		velocity.y += gravity;
	}
	gameObject.lock()->transform->TranslateY(velocity.y);
	shp_AABB->Update();
	shp_bottomAABB->Update();
	BackY();
	CheckHitPlayer();

	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_bottomAABB, 0);
	if (hitObjects.size() == 0)
	{
		grounded = false;
	}
}

void ButiEngine::Frog::BackY()
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
				continue;
			}
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "GravityCore"))
			{
				continue;
			}
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Player"))
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
					grounded = true;
				}
			}
			velocity.y = 0;
		}
	}
}

void ButiEngine::Frog::Interlock()
{
	if (!nearPlayer || wkp_backFrog.lock()->GetGameComponent<Frog>()->IsNearPlayer()) { return; }
	if (wkp_player.lock() && wkp_player.lock()->GetGameComponent<Player>()->IsClear()) { return; }

	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	Vector3 backPosition = wkp_backFrog.lock()->transform->GetWorldPosition();

	position.x = backPosition.x;
	position.y *= -1;
	position.z += 0.001f;

	wkp_backFrog.lock()->transform->SetWorldPosition(position);

	Vector3 scale = gameObject.lock()->transform->GetWorldScale();
	scale.y *= -1;

	wkp_backFrog.lock()->transform->SetLocalScale(scale);

	wkp_backFrog.lock()->GetGameComponent<Frog>()->SetGravity(-gravity);
}

void ButiEngine::Frog::StorePlayer()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
	}
}

void ButiEngine::Frog::CheckHitPlayer()
{
	if (!wkp_player.lock()) { return; }
	auto player = wkp_player.lock()->GetGameComponent<Player>();
	auto playerAABB = player->GetAABB();

	if (Geometry::BoxHit::IsHitBox_AABB(*shp_AABB, *playerAABB))
	{
		player->OnCollisionFrog(gameObject);
	}
}

void ButiEngine::Frog::Animation()
{
	if (!isAnimation) { return; }

	int animationFrame = 30;

	auto playerHoldSita = wkp_player.lock()->GetGameComponent<Player>()->GetHoldSita();
	if (playerHoldSita.lock())
	{
		animationFrame = 40;
	}
	float targetX = gameObject.lock()->transform->GetWorldPosition().x;
	float scaleX = gameObject.lock()->transform->GetWorldScale().x;
	if (scaleX > 0)
	{
		targetX += 5.5f;
	}
	else if (scaleX < 0)
	{
		targetX -= 5.5f;
	}
	float sitaLength = GameSettings::blockSize * 50 - 5.5f;
	float speed = sitaLength / animationFrame;
	progress++;
	float moveX = 0.0f;
	if (gameObject.lock()->transform->GetWorldScale().x < 0)
	{
		moveX = speed;
		if (!grabbed && !playerHoldSita.lock())
		{
			moveX = -speed;
			targetX -= sitaLength;
			if (targetX < -GameSettings::windowWidth * 0.5f)
			{
				targetX += GameSettings::windowWidth;
			}
		}
	}
	else if(gameObject.lock()->transform->GetWorldScale().x > 0)
	{
		moveX = -speed;
		if (!grabbed && !playerHoldSita.lock())
		{
			moveX = speed;
			targetX += sitaLength;
			if (targetX > GameSettings::windowWidth * 0.5f)
			{
				targetX -= GameSettings::windowWidth;
			}
		}
	}

	float x = wkp_sita_sentan.lock()->transform->GetWorldPosition().x + moveX;
	wkp_sita_sentan.lock()->transform->SetWorldPostionX(x);

	Vector3 pos = wkp_sita_sentan.lock()->transform->GetWorldPosition();
	if (abs(pos.x - targetX) < speed)
	{
		if (grabbed)
		{
			animation = Animation::IDLE;
			shp_spriteAnimation->SetHorizontalAnim(0);
			isOutSita = false;
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_eat, GameSettings::masterVolume);
		}
		else
		{
			animation = Animation::SITA;
			shp_spriteAnimation->SetHorizontalAnim(0);
		}
		wkp_sita_sentan.lock()->transform->SetWorldPostionX(targetX);
		isAnimation = false;

		//wkp_player.lock()->GetGameComponent<MeshDrawComponent>()->ReRegist();
		//gameObject.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel().lock()->GetGameComponent<Panel>()->Resist();
		//gameObject.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel().lock()->GetGameComponent<Panel>()->ReResist();

		if (playerHoldSita.lock())
		{
			Exprosion();
			wkp_backFrog.lock()->GetGameComponent<Frog>()->Exprosion();
			wkp_player.lock()->GetGameComponent<Player>()->OnCollisionFrog(gameObject);
		}
	}
	else
	{
		if (abs(pos.x - targetX) < GameSettings::panelWidth)
		{
			auto sita = wkp_sita_sentan.lock()->GetGameComponent<SitaSentan>();
			if (grabbed)
			{
				sita->SetAddZ(0.1f);
			}
			else
			{
				sita->SetAddZ(-0.3f);
			}
		}
		if (!isOutSita)
		{
			isOutSita = true;
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_sitaOut, GameSettings::masterVolume * 0.5f);
		}
		animation = Animation::SITA;
		shp_spriteAnimation->SetHorizontalAnim(0);
	}

	wkp_sita_tyuukan.lock()->GetGameComponent<SitaTyuukan>()->Move();
	if (playerHoldSita.lock())
	{
		wkp_player.lock()->GetGameComponent<Player>()->FollowSita();
	}
}

void ButiEngine::Frog::SpriteAnimation()
{
	animationFrame++;

	if (isApple)
	{
		holdAppleCount++;
	}
	if (holdAppleCount > 150)
	{
		holdAppleCount = 0;
		isApple = false;
		//animation = Animation::SITA;
		//shp_spriteAnimation->SetVarticalAnim(Frog::SITA);
		//shp_spriteAnimation->SetHorizontalAnim(0);
	}

	const int ANIMATION_RATE = 5;
	if (animationFrame < ANIMATION_RATE) return;

	animationFrame = 0;

	const int EAT_APPLE_COUNT = 7;
	const int IDLE_COUNT = 1;
	const int EXPROSION_COUNT = 18;

	switch (animation)
	{
	case ButiEngine::Frog::EAT_APPLE:
		wkp_sita_sentan.lock()->GetGameComponent<SitaSentan>()->SetAnim(1);
		shp_spriteAnimation->SetVarticalAnim(Frog::EAT_APPLE);
		if (shp_spriteAnimation->GetHorizontalAnim() < EAT_APPLE_COUNT)
		{
			const int EAT_SE_FRAME = 3;
			if (shp_spriteAnimation->GetHorizontalAnim() == EAT_SE_FRAME)
			{
				GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_eat, GameSettings::masterVolume);
			}
			shp_spriteAnimation->UpdateHorizontalAnim(1);
		}
		else
		{
			isApple = true;
		}

		break;
	case ButiEngine::Frog::IDLE:
		wkp_sita_sentan.lock()->GetGameComponent<SitaSentan>()->SetAnim(1);
		shp_spriteAnimation->SetVarticalAnim(Frog::IDLE);
		if (shp_spriteAnimation->GetHorizontalAnim() < IDLE_COUNT)
		{
			shp_spriteAnimation->UpdateHorizontalAnim(1);
		}
		else
		{
			shp_spriteAnimation->SetHorizontalAnim(0);
		}

		break;
	case ButiEngine::Frog::SITA:
		wkp_sita_sentan.lock()->GetGameComponent<SitaSentan>()->SetAnim(0);
		shp_spriteAnimation->SetVarticalAnim(Frog::SITA);
		if (shp_spriteAnimation->GetHorizontalAnim() < IDLE_COUNT)
		{
			shp_spriteAnimation->UpdateHorizontalAnim(1);
		}
		else
		{
			shp_spriteAnimation->SetHorizontalAnim(0);
		}
		break;
	case ButiEngine::Frog::EXPROSION:
		wkp_sita_sentan.lock()->GetGameComponent<SitaSentan>()->SetAnim(1);
		shp_spriteAnimation->SetVarticalAnim(Frog::EXPROSION);
		if (shp_spriteAnimation->GetHorizontalAnim() < EXPROSION_COUNT)
		{
			const int EAT_SE_FRAME = 0;
			if (shp_spriteAnimation->GetHorizontalAnim() == EAT_SE_FRAME)
			{
				GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_eat, GameSettings::masterVolume);
			}
			else if (shp_spriteAnimation->GetHorizontalAnim() == EXPROSION_COUNT - 8)
			{
				GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_extend, GameSettings::masterVolume * 0.5f);
			}
			else if (shp_spriteAnimation->GetHorizontalAnim() == EXPROSION_COUNT - 1)
			{
				GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_explosion, GameSettings::masterVolume * 0.5f);
			}

			shp_spriteAnimation->UpdateHorizontalAnim(1);
		}
		else
		{
			if (!isSpawnAngel)
			{
				isSpawnAngel = true;
				auto position = gameObject.lock()->transform->GetLocalPosition();
				position.z -= 0.02f;
				auto scale = gameObject.lock()->transform->GetLocalScale();
				wkp_angel.lock()->transform->SetLocalPosition(position);
				wkp_angel.lock()->transform->SetLocalScale(scale);
				wkp_angel.lock()->GetGameComponent<AngelFrog>()->SetIsActive(true);

				position.z -= 0.1f;
				auto end = vec_wkp_parts.end();
				for (auto itr = vec_wkp_parts.begin(); itr != end; ++itr)
				{
					position.z -= 0.01f;
					(*itr).lock()->GetGameComponent<FrogParts>()->Explosion(position);
				}
				isExplosion = true;
			}

			Vector3 position = gameObject.lock()->transform->GetWorldPosition();
			position.y = -3000;
			gameObject.lock()->transform->SetWorldPosition(position);
		}
		break;
	default:
		break;
	}
}
