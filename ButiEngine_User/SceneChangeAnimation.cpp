#include "stdafx_u.h"
#include "SceneChangeAnimation.h"

void ButiEngine::SceneChangeAnimation::OnUpdate()
{
	//if (lateCount > 0)
	//{
	//	lateCount--;
	//	return;
	//}

	if (life > 0)
	{
		life--;
	}
	else
	{
		position.y = -4000;
		gameObject.lock()->transform->SetLocalPosition(position);
		return;
	}

	const float LERP_SCALE = 0.18f;
	position.y = position.y * (1.0f - LERP_SCALE) + pointPosY * LERP_SCALE;

	gameObject.lock()->transform->SetLocalPosition(position);
}

void ButiEngine::SceneChangeAnimation::OnSet()
{
}

void ButiEngine::SceneChangeAnimation::Start()
{
	lateCount = 3;
	life = 44;

	position = gameObject.lock()->transform->GetLocalPosition();

	pointPosY = position.y - 1134.0f;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SceneChangeAnimation::Clone()
{
	return ObjectFactory::Create<SceneChangeAnimation>();
}
