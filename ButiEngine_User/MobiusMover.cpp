#include "stdafx_u.h"
#include "MobiusMover.h"

void ButiEngine::MobiusMover::OnUpdate()
{
	float distortion = GetManager().lock()->GetGameObject("Distortion").lock()->transform->GetWorldPosition().x;
	gameObject.lock()->transform->GetBaseTransform()->SetLocalRotation(Vector3(0.0f, 0.0f, progress));
	gameObject.lock()->transform->SetLocalRotation(Vector3(progress * 0.5f * distortion, 0.0f, 0.0f));
}

void ButiEngine::MobiusMover::OnSet()
{
	progress = 0.0f;
}

void ButiEngine::MobiusMover::Start()
{
	Vector3 mobiusPos = GetManager().lock()->GetGameObject("Mobius").lock()->transform->GetWorldPosition();
	auto center = GetManager().lock()->AddObjectFromCereal("Center", ObjectFactory::Create<Transform>(mobiusPos, Vector3::Zero, 1.0f));
	gameObject.lock()->transform->SetBaseTransform(center.lock()->transform);
}

void ButiEngine::MobiusMover::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::MobiusMover::Clone()
{
	return ObjectFactory::Create<MobiusMover>();
}
