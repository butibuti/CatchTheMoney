#include "stdafx_u.h"
#include "MapChipCollider.h"
#include"GameSettings.h"
#include"EdgeCollider.h"

void ButiEngine::MapChipCollider::OnUpdate()
{

}

void ButiEngine::MapChipCollider::OnSet()
{
}

void ButiEngine::MapChipCollider::Start()
{
	float shortLength = 8.0f;
	float longLength = GameSettings::blockSize - shortLength * 2;
	float distance = GameSettings::blockSize / 2.0f - shortLength / 2.0f;

	SettingCollider(wkp_collider_right, Vector3(distance, 0.0f, 0.0f), Vector3(shortLength, longLength, 1.0f), "Right");
	SettingCollider(wkp_collider_left, Vector3(-distance, 0.0f, 0.0f), Vector3(shortLength, longLength, 1.0f), "Left");
	SettingCollider(wkp_collider_top, Vector3(0.0f, distance, 0.0f), Vector3(28.0f, 8.0f, 1.0f), "Top");
	SettingCollider(wkp_collider_bottom, Vector3(0.0f, -distance, 0.0f), Vector3(28.0f, 8.0f, 1.0f), "Bottom");
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::MapChipCollider::Clone()
{
	return ObjectFactory::Create<MapChipCollider>();
}

bool ButiEngine::MapChipCollider::IsHitRight(std::weak_ptr<Collision::CollisionPrimitive> arg_collisionPrimitive)
{
	if (!wkp_collider_right.lock()) { return false; }
	auto AABB = wkp_collider_right.lock()->GetGameComponent<EdgeCollider>()->GetAABB();

	return AABB->IsHit(arg_collisionPrimitive);
}

bool ButiEngine::MapChipCollider::IsHitLeft(std::weak_ptr<Collision::CollisionPrimitive> arg_collisionPrimitive)
{
	if (!wkp_collider_left.lock()) { return false; }
	auto AABB = wkp_collider_left.lock()->GetGameComponent<EdgeCollider>()->GetAABB();

	return AABB->IsHit(arg_collisionPrimitive);
}

bool ButiEngine::MapChipCollider::IsHitTop(std::weak_ptr<Collision::CollisionPrimitive> arg_collisionPrimitive)
{
	if (!wkp_collider_top.lock()) { return false; }
	auto AABB = wkp_collider_top.lock()->GetGameComponent<EdgeCollider>()->GetAABB();

	return AABB->IsHit(arg_collisionPrimitive);
}

bool ButiEngine::MapChipCollider::IsHitBottom(std::weak_ptr<Collision::CollisionPrimitive> arg_collisionPrimitive)
{
	if (!wkp_collider_bottom.lock()) { return false; }
	auto AABB = wkp_collider_bottom.lock()->GetGameComponent<EdgeCollider>()->GetAABB();

	return AABB->IsHit(arg_collisionPrimitive);
}

void ButiEngine::MapChipCollider::SettingCollider(std::weak_ptr<GameObject>& arg_collider, const Vector3& arg_localPos, const Vector3& arg_scale, const std::string& arg_name)
{
	std::string name = gameObject.lock()->GetGameObjectName();
	Vector3 parentScale = gameObject.lock()->transform->GetLocalScale();
	auto tag = GetTagManager()->GetObjectTag("MapChip");

	arg_collider = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Collider_" + arg_name);
	arg_collider.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	arg_collider.lock()->transform->SetLocalPosition(arg_localPos / parentScale);
	arg_collider.lock()->transform->SetLocalScale(arg_scale / parentScale);
	arg_collider.lock()->SetGameObjectTag(tag);
	arg_collider.lock()->AddGameComponent<EdgeCollider>();
}
