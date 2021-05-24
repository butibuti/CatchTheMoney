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

void ButiEngine::Frog::OnUpdate()
{
	if (shp_pauseManager->IsPause() ||
		StageManager::GetMode() == GameMode::Edit ||
		!TalkText::IsDelete())
	{

		return;
	}

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
	wkp_bottom.lock()->transform->SetLocalPosition(Vector3(0.0f, -0.75f, 0.0f));
	wkp_bottom.lock()->transform->SetLocalScale(Vector3(1.0f, 0.25f, 1.0f));

	shp_bottomAABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.499f, 0.499f, 1.0f), wkp_bottom.lock()->transform);
}

void ButiEngine::Frog::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Frog::Clone()
{
	return ObjectFactory::Create<Frog>();
}

void ButiEngine::Frog::CreateSita()
{
	wkp_sita_tyuukan = GetManager().lock()->AddObjectFromCereal("Sita_tyuukan");
	wkp_sita_sentan = GetManager().lock()->AddObjectFromCereal("Sita_sentan");

	auto tyuukan = wkp_sita_tyuukan.lock()->GetGameComponent<SitaTyuukan>();
	tyuukan->SetFrog(gameObject);
	tyuukan->SetSentan(wkp_sita_sentan);

	wkp_sita_sentan.lock()->GetGameComponent<SitaSentan>()->StoreFrog(gameObject);

	wkp_sita_sentan.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_sita_sentan.lock()->transform->SetLocalScale(Vector3(1, 1, 1));
	float sitaLength = GameSettings::blockSize * 50;
	Vector3 sentanPosition = Vector3::Zero;
	sentanPosition.x = sitaLength / gameObject.lock()->transform->GetWorldScale().x;
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
	}
	else if (abs(gravity) > abs(previousGravity))
	{
	}
}

void ButiEngine::Frog::MoveY()
{
	if (!grounded)
	{
		velocity.y += gravity;
	}
	gameObject.lock()->transform->TranslateY(velocity.y);
	shp_AABB->Update();
	shp_bottomAABB->Update();
	BackY();

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
