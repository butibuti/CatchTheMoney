#include "stdafx_u.h"
#include "AbuttonAnimation.h"
#include "InputManager.h"

void ButiEngine::AbuttonAnimation::OnUpdate()
{
	const float MIN_SCALE = 140;
	const float MAX_SCALE = 180;

	changeCount++;

	if (!isChange)
	{
		currentScale = MAX_SCALE;
	}
	else
	{
		currentScale = MIN_SCALE;
	}

	if (InputManager::OnTriggerDecisionKey())
	{
		previousScale = 60;
		isChange = false;
		currentScale = MAX_SCALE;
		changeCount = 0;
	}

	if (changeCount > 30)
	{
		isChange = !isChange;
		changeCount = 0;
	}

	//Lerp
	previousScale = previousScale * (1.0f - 0.08f) + currentScale * 0.08f;

	gameObject.lock()->transform->SetLocalScale(Vector3(previousScale, previousScale, 0));
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
