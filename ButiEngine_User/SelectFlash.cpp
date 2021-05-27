#include "stdafx_u.h"
#include "SelectFlash.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"
#include "Header//GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::SelectFlash::OnUpdate()
{
	auto transform = gameObject.lock()->transform;
	if (life < 0)
	{
		transform->SetLocalPosition(Vector3(0, -3000, 0));
		return;
	}
	else
	{
		life--;
	}

	const int ANIMATION_RATE = 5;
	if (animationFrame < ANIMATION_RATE)
	{
		animationFrame++;
		return;
	}
	else
	{
		shp_spriteAnimation->SetHorizontalAnim(1);
	}

	const int AWAY_RATE = 1;
	if (awayCount < AWAY_RATE)
	{
		awayCount++;
	}
	else
	{
		awayCount = 0;
		awayFlag = !awayFlag;
	}

	const Vector3 awayPos = Vector3(0, -3000, 0);
	if (!awayFlag)
	{
		gameObject.lock()->transform->SetLocalPosition(initPosition);
	}
	else
	{
		gameObject.lock()->transform->SetLocalPosition(awayPos);
	}

}

void ButiEngine::SelectFlash::OnSet()
{
}

void ButiEngine::SelectFlash::Start()
{
	life = 30;
	animationFrame = 0;
	awayCount = 0;
	awayFlag = false;
	const Vector3 initRotate = Vector3(0, 0, 45);
	initPosition = Vector3(100, 400, -0.04f);
	gameObject.lock()->transform->SetLocalRotation(initRotate);
	gameObject.lock()->transform->SetLocalPosition(initPosition);

	const Vector3 endScale = Vector3(210, 210, 1);
	auto anim = gameObject.lock()->GetGameComponent<ScaleAnimation>();
	if (!anim)
	{
		anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
		anim->SetInitScale(gameObject.lock()->transform->GetWorldScale());
		anim->SetTargetScale(endScale);
		anim->SetSpeed(0.025f);
		anim->SetEaseType(Easing::EasingType::EaseOutExpo);
	}

	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	shp_spriteAnimation->SetHorizontalAnim(0);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SelectFlash::Clone()
{
	return ObjectFactory::Create<SelectFlash>();
}
