#include "stdafx_u.h"
#include "MobiusLoop.h"
#include"GameSettings.h"
#include"PauseManager.h"

void ButiEngine::MobiusLoop::OnUpdate()
{
	SetPosition();
	SwitchPosition();
}

void ButiEngine::MobiusLoop::OnSet()
{
}

void ButiEngine::MobiusLoop::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();

	std::string name = gameObject.lock()->GetGameObjectName();
	Vector3 localPosition = Vector3::Zero;
	auto tag = GetTagManager()->GetObjectTag("MapChip");
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	if (!meshDraw)
	{
		meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent_Static>();
	}
	float scaleX = gameObject.lock()->transform->GetWorldScale().x;

	wkp_right = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Right");
	wkp_right.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	localPosition.x = GameSettings::windowWidth / scaleX;
	wkp_right.lock()->transform->SetLocalPosition(localPosition);
	wkp_right.lock()->SetGameObjectTag(tag);
	if (meshDraw)
	{
		wkp_right.lock()->AddGameComponent(meshDraw->Clone());
	}
	if (name == "Player")
	{
		shp_AABB_right = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.999f, 0.999f, 10.0f), wkp_right.lock()->transform);
	}

	wkp_left = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Left");
	wkp_left.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	localPosition.x = -GameSettings::windowWidth / scaleX;
	wkp_left.lock()->transform->SetLocalPosition(localPosition);
	wkp_left.lock()->SetGameObjectTag(tag);
	if (meshDraw)
	{
		wkp_left.lock()->AddGameComponent(meshDraw->Clone());
	}
	if (name == "Player")
	{
		shp_AABB_left = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.999f, 0.999f, 10.0f), wkp_left.lock()->transform);
	}

	if (StringHelper::Contains(name, "Panel"))
	{
		Vector3 scale = gameObject.lock()->transform->GetLocalScale();
		Vector3 cloneScale = scale;
		cloneScale.y = -cloneScale.y;
		wkp_right.lock()->transform->SetLocalScale(cloneScale / scale);
		wkp_left.lock()->transform->SetLocalScale(cloneScale / scale);
	}
}

void ButiEngine::MobiusLoop::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::MobiusLoop::OnShowUI()
{
}

void ButiEngine::MobiusLoop::ShowGUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::MobiusLoop::Clone()
{
	return ObjectFactory::Create<MobiusLoop>();
}

void ButiEngine::MobiusLoop::SwitchPosition()
{
	if (StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "PredictionLine") || 
		StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "Sita_tyuukan"))
	{
		return;
	}

	float x = gameObject.lock()->transform->GetWorldPosition().x;
	auto right = wkp_right;
	auto left = wkp_left;
	if (gameObject.lock()->transform->GetWorldScale().x < 0)
	{
		right = wkp_left;
		left = wkp_right;
	}

	if (x < -GameSettings::windowWidth * 0.5f)
	{
		gameObject.lock()->transform->SetWorldPosition(right.lock()->transform->GetWorldPosition());
	}
	else if (x > GameSettings::windowWidth * 0.5f)
	{
		gameObject.lock()->transform->SetWorldPosition(left.lock()->transform->GetWorldPosition());
	}
}

void ButiEngine::MobiusLoop::UpdateAABB()
{
	shp_AABB_right->Update();
	shp_AABB_left->Update();
}

void ButiEngine::MobiusLoop::BackXRight(Vector3& arg_velocity)
{
	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_AABB_right, 0);

	if (hitObjects.size() != 0)
	{
		auto end = hitObjects.end();
		for (auto itr = hitObjects.begin(); itr != end; ++itr)
		{
			if ((*itr) == wkp_right.lock()) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Goal")) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "GravityCore")) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Frog")) { continue; }

			float widthHalf = (*itr)->transform->GetWorldScale().x * 0.5f;
			if (arg_velocity.x > 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x - widthHalf - GameSettings::blockSize * 0.5f - wkp_right.lock()->transform->GetWorldPosition().x;
				gameObject.lock()->transform->TranslateX(backLength);
				shp_AABB_right->Update();
			}
			else if (arg_velocity.x < 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x + widthHalf + GameSettings::blockSize * 0.5f - wkp_right.lock()->transform->GetWorldPosition().x;
				gameObject.lock()->transform->TranslateX(backLength);
				shp_AABB_right->Update();
			}
			arg_velocity.x = 0;
		}
	}
}

void ButiEngine::MobiusLoop::BackYRight(Vector3& arg_velocity)
{
	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_AABB_right, 0);

	if (hitObjects.size() != 0)
	{
		auto end = hitObjects.end();
		for (auto itr = hitObjects.begin(); itr != end; ++itr)
		{
			if ((*itr) == wkp_right.lock()) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Goal")) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "GravityCore")) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Frog")) { continue; }

			if (arg_velocity.y > 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().y - GameSettings::blockSize - wkp_right.lock()->transform->GetWorldPosition().y;
				gameObject.lock()->transform->TranslateY(backLength);
				shp_AABB_right->Update();
			}
			else if (arg_velocity.y < 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().y + GameSettings::blockSize - wkp_right.lock()->transform->GetWorldPosition().y;
				gameObject.lock()->transform->TranslateY(backLength);
				shp_AABB_right->Update();
			}
			arg_velocity.y = 0;
		}
	}
}

void ButiEngine::MobiusLoop::BackXLeft(Vector3& arg_velocity)
{
	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_AABB_left, 0);

	if (hitObjects.size() != 0)
	{
		auto end = hitObjects.end();
		for (auto itr = hitObjects.begin(); itr != end; ++itr)
		{
			if ((*itr) == wkp_left.lock()) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Goal")) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "GravityCore")) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Frog")) { continue; }

			float widthHalf = (*itr)->transform->GetWorldScale().x * 0.5f;
			if (arg_velocity.x > 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x - widthHalf - GameSettings::blockSize * 0.5f - wkp_left.lock()->transform->GetWorldPosition().x;
				gameObject.lock()->transform->TranslateX(backLength);
				shp_AABB_left->Update();
			}
			else if (arg_velocity.x < 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x + widthHalf + GameSettings::blockSize * 0.5f - wkp_left.lock()->transform->GetWorldPosition().x;
				gameObject.lock()->transform->TranslateX(backLength);
				shp_AABB_left->Update();
			}
		}
		arg_velocity.x = 0;
	}
}

void ButiEngine::MobiusLoop::BackYLeft(Vector3& arg_velocity)
{
	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_AABB_left, 0);

	if (hitObjects.size() != 0)
	{
		auto end = hitObjects.end();
		for (auto itr = hitObjects.begin(); itr != end; ++itr)
		{
			if ((*itr) == wkp_left.lock()) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Goal")) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "GravityCore")) { continue; }
			if (StringHelper::Contains((*itr)->GetGameObjectName(), "Frog")) { continue; }

			if (arg_velocity.y > 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().y - GameSettings::blockSize - wkp_left.lock()->transform->GetWorldPosition().y;
				gameObject.lock()->transform->TranslateY(backLength);
				shp_AABB_left->Update();
			}
			else if (arg_velocity.y < 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().y + GameSettings::blockSize - wkp_left.lock()->transform->GetWorldPosition().y;
				gameObject.lock()->transform->TranslateY(backLength);
				shp_AABB_left->Update();
			}
		}
		arg_velocity.y = 0;
	}
}

void ButiEngine::MobiusLoop::SetPosition()
{
	if (!StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "Sita_tyuukan")) { return; }

	Vector3 localPosition = Vector3::Zero;

	float scaleX = gameObject.lock()->transform->GetWorldScale().x;

	localPosition.x = GameSettings::windowWidth / scaleX;
	wkp_right.lock()->transform->SetLocalPosition(localPosition);

	localPosition.x = -GameSettings::windowWidth / scaleX;
	wkp_left.lock()->transform->SetLocalPosition(localPosition);
}
