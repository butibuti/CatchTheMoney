#include "stdafx_u.h"
#include "SelectZanzo.h"

void ButiEngine::SelectZanzo::OnUpdate()
{
	const Vector3 AWAY_POS = Vector3(650, -5000, 0.02f);
	const int START_FRAME = 5;
	const int DASH_FRAME = 20;
	const int DEAD_FRAME = 25;
	if (animationCount == START_FRAME)
	{
		gameObject.lock()->transform->SetLocalPosition(AWAY_POS);
	}
	else if (animationCount == DASH_FRAME)
	{
		gameObject.lock()->transform->SetLocalPosition(Vector3(450, 400, 0.02f));
		gameObject.lock()->transform->SetLocalScale(Vector3(760, 80, 1));
		gameObject.lock()->transform->SetLocalRotation(Vector3(-180, 0, -180));
	}
	else if (animationCount == DEAD_FRAME)
	{
		gameObject.lock()->transform->SetLocalPosition(AWAY_POS);
	}
	animationCount++;
}

void ButiEngine::SelectZanzo::OnSet()
{
}

void ButiEngine::SelectZanzo::Start()
{
	animationCount = 0;
	gameObject.lock()->transform->SetLocalPosition(Vector3(320,-400,0.02f));
	gameObject.lock()->transform->SetLocalScale(Vector3(1600, 160, 1));
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SelectZanzo::Clone()
{
	return ObjectFactory::Create<SelectZanzo>();
}
