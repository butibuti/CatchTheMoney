#include "stdafx_u.h"
#include "EdgeCollider.h"

void ButiEngine::EdgeCollider::OnUpdate()
{
	shp_AABB->Update();
}

void ButiEngine::EdgeCollider::OnSet()
{
	shp_AABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.999f, 0.999f, 0.999f), gameObject.lock()->transform);
}

void ButiEngine::EdgeCollider::Start()
{
}

void ButiEngine::EdgeCollider::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::EdgeCollider::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::EdgeCollider::OnCollisionEnd(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::EdgeCollider::Clone()
{
	return ObjectFactory::Create<EdgeCollider>();
}
