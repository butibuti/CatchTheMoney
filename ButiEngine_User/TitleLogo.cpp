#include "stdafx_u.h"
#include "TitleLogo.h"

void ButiEngine::TitleLogo::OnUpdate()
{
	const float LERP_SCALE = 0.05f;
	previousPosY = previousPosY * (1.0f - LERP_SCALE) + currentPosY * LERP_SCALE;

	gameObject.lock()->transform->SetLocalPostionY(previousPosY);
}

void ButiEngine::TitleLogo::OnSet()
{
}

void ButiEngine::TitleLogo::Start()
{
	currentPosY = 0;
	previousPosY = 0;
}

void ButiEngine::TitleLogo::AnimationStart()
{
	currentPosY = 700;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::TitleLogo::Clone()
{
	return ObjectFactory::Create<TitleLogo>();
}
