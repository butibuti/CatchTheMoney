#include "stdafx_u.h"
#include "MobiusLoop.h"
#include"GameSettings.h"
#include "PredictionLine.h"

void ButiEngine::MobiusLoop::OnUpdate()
{
	float x = gameObject.lock()->transform->GetWorldPosition().x;

	if (x < -GameSettings::windowWidth * 0.5f)
	{
		gameObject.lock()->transform->SetWorldPosition(wkp_right.lock()->transform->GetWorldPosition());
	}
	else if (x > GameSettings::windowWidth * 0.5f)
	{
		gameObject.lock()->transform->SetWorldPosition(wkp_left.lock()->transform->GetWorldPosition());
	}

	GetManager().lock()->GetApplication().lock()->GetGUIController()->SetGUIObject(GetThis<MobiusLoop>());
}

void ButiEngine::MobiusLoop::OnSet()
{
}

void ButiEngine::MobiusLoop::Start()
{
	std::string name = gameObject.lock()->GetGameObjectName();
	Vector3 localPosition = Vector3::Zero;
	auto tag = GetTagManager()->GetObjectTag("MapChip");
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();

	auto scaleX = gameObject.lock()->transform->GetWorldScale().x;
	wkp_right = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Right");
	wkp_right.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	localPosition.x = GameSettings::windowWidth / scaleX;
	wkp_right.lock()->transform->SetLocalPosition(localPosition);
	wkp_right.lock()->SetGameObjectTag(tag);
	wkp_right.lock()->AddGameComponent(meshDraw->Clone());
	shp_AABB_right = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.999f, 0.999f, 1.0f), wkp_right.lock()->transform);

	wkp_left = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Left");
	wkp_left.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	localPosition.x = -GameSettings::windowWidth / scaleX;
	wkp_left.lock()->transform->SetLocalPosition(localPosition);
	wkp_left.lock()->SetGameObjectTag(tag);
	wkp_left.lock()->AddGameComponent(meshDraw->Clone());
	shp_AABB_left = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.999f, 0.999f, 1.0f), wkp_left.lock()->transform);
}

void ButiEngine::MobiusLoop::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::MobiusLoop::ShowGUI()
{
	GUI::Begin("MobiusLoop");
	GUI::Text("Right:%f", wkp_right.lock()->transform->GetWorldPosition().x);
	GUI::Text("Left:%f", wkp_left.lock()->transform->GetWorldPosition().x);
	GUI::End();
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::MobiusLoop::Clone()
{
	return ObjectFactory::Create<MobiusLoop>();
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

			if (arg_velocity.x > 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x - GameSettings::blockSize - wkp_right.lock()->transform->GetWorldPosition().x;
				gameObject.lock()->transform->TranslateX(backLength);
				shp_AABB_right->Update();
			}
			else if (arg_velocity.x < 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x + GameSettings::blockSize - wkp_right.lock()->transform->GetWorldPosition().x;
				gameObject.lock()->transform->TranslateX(backLength);
				shp_AABB_right->Update();
			}
		}
		arg_velocity.x = 0;
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
		}
		arg_velocity.y = 0;
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

			if (arg_velocity.x > 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x - GameSettings::blockSize - wkp_left.lock()->transform->GetWorldPosition().x;
				gameObject.lock()->transform->TranslateX(backLength);
				shp_AABB_left->Update();
			}
			else if (arg_velocity.x < 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x + GameSettings::blockSize - wkp_left.lock()->transform->GetWorldPosition().x;
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
