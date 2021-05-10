#include "stdafx_u.h"
#include "ClearFlash.h"

void ButiEngine::ClearFlash::OnUpdate()
{


	if (isOnce) return;
	isOnce = true;

	auto goalPosition = GetManager().lock()->GetGameObject("Goal").lock()->transform->GetWorldPosition();
	gameObject.lock()->transform->SetLocalPosition(goalPosition);

	auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (!anim)
	{
		anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(gameObject.lock()->transform->Clone());
		anim->GetTargetTransform()->SetLocalScale(Vector3(40.0f, 40.0f, 0.0f));
		anim->SetSpeed(0.02f);
		anim->SetEaseType(Easing::EasingType::EaseOutExpo);
	}
}

void ButiEngine::ClearFlash::OnSet()
{
}

void ButiEngine::ClearFlash::Start()
{
	isOnce = false;
	gameObject.lock()->transform->SetLocalScale(Vector3(0, 0, 0));
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ClearFlash::Clone()
{
	return ObjectFactory::Create<ClearFlash>();
}
