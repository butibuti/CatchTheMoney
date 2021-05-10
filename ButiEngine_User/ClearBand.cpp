#include "stdafx_u.h"
#include "ClearBand.h"

void ButiEngine::ClearBand::OnUpdate()
{ 	
}

void ButiEngine::ClearBand::OnSet()
{
}

void ButiEngine::ClearBand::Start()
{
	gameObject.lock()->transform->SetLocalScale(Vector3(2000.0f, 0.0f, 1.0f));
	auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (!anim)
	{
		anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(gameObject.lock()->transform->Clone());
		anim->GetTargetTransform()->SetLocalScale(Vector3(2000.0f, 200.0f, 1.0f));
		anim->SetSpeed(0.06f);
		anim->SetEaseType(Easing::EasingType::EaseInOutExpo);
	}
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ClearBand::Clone()
{
	return ObjectFactory::Create<ClearBand>();
}