#include "stdafx_u.h"
#include "ClearText.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"

void ButiEngine::ClearText::OnUpdate()
{
	bool isAnimation = horizontalNum < 2;
	if (!isAnimation) return;

	Animation();
}

void ButiEngine::ClearText::OnSet()
{
}

void ButiEngine::ClearText::Start()
{
	isOnce = false;
	horizontalNum = 0;
	animationCount = 0;

	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	shp_spriteAnimation->SetHorizontalAnim(horizontalNum);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ClearText::Clone()
{
	return ObjectFactory::Create<ClearText>();
}

void ButiEngine::ClearText::Animation()
{
	if (animationCount > 4)
	{
		animationCount = 0;
		horizontalNum++;
		shp_spriteAnimation->SetHorizontalAnim(horizontalNum);
	}
	else
	{
		animationCount++;
	}
}
