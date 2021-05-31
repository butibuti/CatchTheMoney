#include "stdafx_u.h"
#include "ClearZanzo.h"

void ButiEngine::ClearZanzo::OnUpdate()
{
	const Vector3 AWAY_POS = Vector3(650, -5000, 0.02f);
	const int START_FRAME = 2;
	const int DASH_FRAME = 20;
	const int DEAD_FRAME = 22;
	if (animationCount == START_FRAME)
	{
		gameObject.lock()->transform->SetLocalPosition(AWAY_POS);
	}
	else if (animationCount == DASH_FRAME)
	{
	}
	else if (animationCount == DEAD_FRAME)
	{
		gameObject.lock()->transform->SetLocalPosition(AWAY_POS);
	}
	animationCount++;
}

void ButiEngine::ClearZanzo::OnSet()
{
}

void ButiEngine::ClearZanzo::Start()
{
	animationCount = 0;
	gameObject.lock()->transform->SetLocalPosition(Vector3(-30, 350, 0.02f));
	gameObject.lock()->transform->SetLocalScale(Vector3(1600, 320, 1));
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ClearZanzo::Clone()
{
    return ObjectFactory::Create<ClearZanzo>();
}
