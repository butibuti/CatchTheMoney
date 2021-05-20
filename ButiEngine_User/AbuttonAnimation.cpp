#include "stdafx_u.h"
#include "AbuttonAnimation.h"
#include "InputManager.h"

void ButiEngine::AbuttonAnimation::OnUpdate()
{
	const float MIN_SCALE = 140;
	const float MAX_SCALE = 180;

	if (!isChange)
	{
		currentScale = MAX_SCALE;
	}
	else
	{
		currentScale = MIN_SCALE;
	}

	const float PUSH_SCALE = 60;
	if (InputManager::OnTriggerDecisionKey())
	{
		isChange = false;
		previousScale = PUSH_SCALE;
		currentScale = MAX_SCALE;
		changeCount = 0;
	}

	if (changeCount > 30)
	{
		isChange = !isChange;
		changeCount = 0;
	}
	else
	{
		changeCount++;
	}

	const float LERP_SCALE = 0.08f;
	//Lerp
	previousScale = previousScale * (1.0f - LERP_SCALE) + currentScale * LERP_SCALE;

	gameObject.lock()->transform->SetLocalScale(Vector3(previousScale, previousScale, 0));
}

void ButiEngine::AbuttonAnimation::OnSet()
{
}

void ButiEngine::AbuttonAnimation::Start()
{
	isChange = false;
	const float DEFAULT_SCALE = 180;
	currentScale = DEFAULT_SCALE;
	previousScale = DEFAULT_SCALE;
	changeCount = 0;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::AbuttonAnimation::Clone()
{
	return ObjectFactory::Create<AbuttonAnimation>();
}
