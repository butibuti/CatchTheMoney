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

float ButiEngine::Frog::sitaLength;

void ButiEngine::Frog::OnUpdate()
{
	StorePlayer();
	Interlock();
	SetZ();
	if (shp_pauseManager->IsPause() ||
		StageManager::GetMode() == GameMode::Edit ||
		!TalkText::IsDelete())
	{
		return;
	}

	CheckNearPlayer();
	if (!nearPlayer) { return; }
	MoveY();
	CheckGravity();
	FollowPlayer();
	Animation();
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
	wkp_bottom.lock()->transform->SetLocalPosition(Vector3(0.0f, -0.75f, 0.0f));
	wkp_bottom.lock()->transform->SetLocalScale(Vector3(1.0f, 0.25f, 1.0f));

	shp_bottomAABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.499f, 0.499f, 10.0f), wkp_bottom.lock()->transform);

	velocity = Vector3::Zero;
	grounded = false;
	nearPlayer = false;
	progress = 1.0f;
	once = true;
}

void ButiEngine::Frog::OnShowUI()
{
	GUI::Text("%f", gameObject.lock()->transform->GetWorldPosition().z);
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

void ButiEngine::Frog::CreateSita()
{
	wkp_sita_tyuukan = GetManager().lock()->AddObjectFromCereal("Sita_tyuukan");
	wkp_sita_sentan = GetManager().lock()->AddObjectFromCereal("Sita_sentan");

	auto tyuukan = wkp_sita_tyuukan.lock()->GetGameComponent<SitaTyuukan>();
	tyuukan->SetFrog(gameObject);
	tyuukan->SetSentan(wkp_sita_sentan);

	wkp_sita_sentan.lock()->GetGameComponent<SitaSentan>()->SetFrog(gameObject);

	wkp_sita_sentan.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_sita_sentan.lock()->transform->SetLocalScale(Vector3(0.75f, 0.75f, 1));
	sitaLength = GameSettings::blockSize * 50;
	Vector3 sentanPosition = Vector3::Zero;
	sentanPosition.x = sitaLength / 32;
	sentanPosition.z = 0.01f;
	wkp_sita_sentan.lock()->transform->SetLocalPosition(sentanPosition);
}

void ButiEngine::Frog::CheckGravity()
{
	auto closestPanel = gameObject.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel();
	float previousGravity = gravity;
	if (closestPanel.lock())
	{
		auto panelComponent = closestPanel.lock()->GetGameComponent<Panel>();
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
	}
	else if (abs(gravity) > abs(previousGravity))
	{
	}
}

void ButiEngine::Frog::CheckNearPlayer()
{
	if (!wkp_player.lock() || nearPlayer) { return; }

	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	Vector3 playerPos = wkp_player.lock()->transform->GetWorldPosition();

	if (position.x < 0 == playerPos.x < 0)
	{
		nearPlayer = true;
		wkp_backFrog.lock()->GetGameComponent<Frog>()->SetNearPlayer(false);
	}
}

void ButiEngine::Frog::SetZ()
{

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

	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	Vector3 backPosition = wkp_backFrog.lock()->transform->GetWorldPosition();

	position.x = backPosition.x;
	position.y *= -1;

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

	player->ResetHitFrog();
	if (Geometry::BoxHit::IsHitBox_AABB(*shp_AABB, *playerAABB))
	{
		player->OnCollisionFrog(gameObject);
	}
}

void ButiEngine::Frog::FollowPlayer()
{
	if (!grabbed) { return; }
	if (once)
	{
		gameObject.lock()->GetGameComponent<MeshDrawComponent>()->ReRegist();
		once = false;
	}

	Vector3 playerPos = wkp_player.lock()->transform->GetWorldPosition();
	Vector3 targetPos = gameObject.lock()->transform->GetWorldPosition();
	float playerGravity = wkp_player.lock()->GetGameComponent<Player>()->GetGravity();
	float difference = 16.0f;

	if (playerGravity > 0)
	{
		difference *= -1;
	}

	targetPos.y = playerPos.y + difference;
	gameObject.lock()->transform->SetWorldPostionY(targetPos.y);

	Vector3 scale = gameObject.lock()->transform->GetLocalScale();
	Vector3 playerScale = wkp_player.lock()->transform->GetLocalScale();

	if (playerScale.x < 0 != scale.x < 0)
	{
		scale.x *= -1;
		gameObject.lock()->transform->SetLocalScale(scale);
	}

	if (GameDevice::GetInput()->CheckKey(Keys::U))
	{
		sitaLength += 1.0f;
	}
	else if (GameDevice::GetInput()->CheckKey(Keys::J))
	{
		sitaLength -= 1.0f;
	}
}

void ButiEngine::Frog::Animation()
{
	if (!animation) { return; }


	if (grabbed)
	{
		progress -= 0.05f;
	}
	else
	{
		progress += 0.05f;
	}

	if (progress < 0.0f)
	{
		progress = 0.0f;
		animation = false;
	}
	else if (progress > 1.0f)
	{
		progress = 1.0f;
		animation = false;
	}

	const float MAX_SITA_LENGTH = GameSettings::blockSize * 50;

	sitaLength = progress * MAX_SITA_LENGTH;
}
