#include "stdafx_u.h"
#include "SceneChangeAnimation.h"

void ButiEngine::SceneChangeAnimation::OnUpdate()
{
	position.y = position.y * (1.0f - 0.15f) + pointPosY * 0.15f;

	gameObject.lock()->transform->SetLocalPosition(position);
}

void ButiEngine::SceneChangeAnimation::OnSet()
{
}

void ButiEngine::SceneChangeAnimation::Start()
{
	position = gameObject.lock()->transform->GetLocalPosition();

	pointPosY = position.y - 1080.0f;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SceneChangeAnimation::Clone()
{
	return ObjectFactory::Create<SceneChangeAnimation>();
}
