#include "stdafx_u.h"
#include "ClearText.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"

void ButiEngine::ClearText::OnUpdate()
{
	bool isAnimation = horizontalNum < 3;
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

	auto transform = gameObject.lock()->transform;
	transform->SetLocalPosition(Vector3(-14.5f, 0, -980.0f));
	transform->SetLocalScale(Vector3(5.0f, 5.0f, 1.0f));
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ClearText::Clone()
{
	return ObjectFactory::Create<ClearText>();
}

void ButiEngine::ClearText::Animation()
{
	if (animationCount > 60)
	{
		horizontalNum++;
		shp_spriteAnimation->SetHorizontalAnim(horizontalNum);
	}
	else
	{
		animationCount++;
	}
}
