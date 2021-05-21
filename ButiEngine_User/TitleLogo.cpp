#include "stdafx_u.h"
#include "TitleLogo.h"

void ButiEngine::TitleLogo::OnUpdate()
{
	const float LERP_SCALE = 0.04f;
	previousPosY = previousPosY * (1.0f - LERP_SCALE) + currentPosY * LERP_SCALE;

	gameObject.lock()->transform->SetLocalPostionY(previousPosY);
}

void ButiEngine::TitleLogo::OnSet()
{
}

void ButiEngine::TitleLogo::Start()
{
	currentPosY = 100;
	previousPosY = 100;
}

void ButiEngine::TitleLogo::AnimationStart()
{
	currentPosY = 900;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::TitleLogo::Clone()
{
	return ObjectFactory::Create<TitleLogo>();
}
