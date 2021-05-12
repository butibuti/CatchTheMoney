#include "stdafx_u.h"
#include "SelectZanzo.h"

void ButiEngine::SelectZanzo::OnUpdate()
{
	if (animationCount == 5)
	{
		gameObject.lock()->transform->SetLocalPosition(Vector3(650, -5000, 0.02f));
	}
	else if (animationCount == 20)
	{
		gameObject.lock()->transform->SetLocalPosition(Vector3(650, -130, 0.02f));
		gameObject.lock()->transform->SetLocalScale(Vector3(640, 80, 1));
		gameObject.lock()->transform->SetLocalRotation(Vector3(-180, 0, -180));
	}
	else if (animationCount == 25)
	{
		gameObject.lock()->transform->SetLocalPosition(Vector3(650, -5000, 0.02f));
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
