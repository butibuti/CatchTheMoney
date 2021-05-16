#include "stdafx_u.h"
#include "AbuttonAnimation.h"
#include "InputManager.h"

void ButiEngine::AbuttonAnimation::OnUpdate()
{
	const float MIN_SCALE = 160;
	const float MAX_SCALE = 180;

	changeCount++;

	if (!isChange)
	{
		currentScale = MIN_SCALE;
	}
	else
	{
		currentScale = MAX_SCALE;
	}

	if (InputManager::OnTriggerDecisionKey())
	{
		gameObject.lock()->transform->SetLocalScale(Vector3(80, 80, 0));
	}

	if (changeCount > 30)
	{
		isChange = !isChange;
		changeCount = 0;

		auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
		if (!anim)
		{
			anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
			anim->SetTargetTransform(gameObject.lock()->transform->Clone());
			anim->GetTargetTransform()->SetLocalScale(Vector3(currentScale, currentScale, 0));
			anim->SetSpeed(0.05f);
			anim->SetEaseType(Easing::EasingType::EaseOut);
		}
	}
}

void ButiEngine::AbuttonAnimation::OnSet()
{
}

void ButiEngine::AbuttonAnimation::Start()
{
	isChange = false;
	currentScale = 180;
	previousScale = 180;
	changeCount = 0;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::AbuttonAnimation::Clone()
{
	return ObjectFactory::Create<AbuttonAnimation>();
}
