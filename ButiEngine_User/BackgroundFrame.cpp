#include "stdafx_u.h"
#include "BackgroundFrame.h"
#include "GameSettings.h"
#include"PauseManager.h"

void ButiEngine::BackgroundFrame::OnUpdate()
{
	if (PauseManager::IsPause()) { return; }
	Move();
	Rotation();
}

void ButiEngine::BackgroundFrame::OnSet()
{
}

void ButiEngine::BackgroundFrame::Start()
{
	SetVelocityRandom();
}

void ButiEngine::BackgroundFrame::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::BackgroundFrame::Clone()
{
	return ObjectFactory::Create<BackgroundFrame>();
}

void ButiEngine::BackgroundFrame::SetVelocityRandom()
{
	bool right = ButiRandom::GetRandom(0, 1, 1);
	velocityX = ButiRandom::GetRandom(3, 6, 100);

	if (right)
	{
		velocityX *= -1;
	}

	float scaleX = gameObject.lock()->transform->GetWorldScale().x;
	const float MAX_SCALE = 960.0f;
	addAngle = velocityX * MAX_SCALE / scaleX * -0.125f;
}

void ButiEngine::BackgroundFrame::Move()
{
	gameObject.lock()->transform->TranslateX(velocityX);
	Loop();
}

void ButiEngine::BackgroundFrame::Loop()
{
	float x = gameObject.lock()->transform->GetWorldPosition().x;
	float scaleX = gameObject.lock()->transform->GetWorldScale().x;
	float loopRange = 1980 * 0.5f + scaleX * 0.7f;
	if (x < -loopRange)
	{
		gameObject.lock()->transform->SetWorldPostionX(loopRange);
	}
	else if (x > loopRange)
	{
		gameObject.lock()->transform->SetWorldPostionX(-loopRange);
	}
}

void ButiEngine::BackgroundFrame::Rotation()
{
	gameObject.lock()->transform->RollWorldRotationZ_Degrees(addAngle);
}
