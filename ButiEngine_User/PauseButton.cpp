#include "stdafx_u.h"
#include "PauseButton.h"
#include"Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include"Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::PauseButton::OnUpdate()
{
}

void ButiEngine::PauseButton::OnSet()
{
}

void ButiEngine::PauseButton::Start()
{
	initPosition = gameObject.lock()->transform->GetWorldPosition();
	defaultPosition = initPosition;
	defaultPosition.y = -250.0f;
	defaultScale = gameObject.lock()->transform->GetWorldScale();
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::PauseButton::Clone()
{
	return ObjectFactory::Create<PauseButton>();
}

void ButiEngine::PauseButton::Appear()
{
	gameObject.lock()->transform->SetLocalScale(defaultScale);
	AddPositionAnimation(defaultPosition, 20, Easing::EasingType::EaseOutBack);
}

void ButiEngine::PauseButton::Disappear()
{
	AddPositionAnimation(initPosition, 20, Easing::EasingType::EaseInBack);
}

void ButiEngine::PauseButton::OnSelected()
{
	AddScaleAnimation(defaultScale * 1.2f, 40, Easing::EasingType::EaseOutElastic);
}

void ButiEngine::PauseButton::OnEndSelect()
{
	AddScaleAnimation(defaultScale, 40, Easing::EasingType::EaseOutElastic);
}

void ButiEngine::PauseButton::AddPositionAnimation(const Vector3& arg_targetPosition, int frame, Easing::EasingType easingType)
{
	auto anim_ = gameObject.lock()->GetGameComponent<PositionAnimation>();
	if (anim_)
	{
		gameObject.lock()->RemoveGameComponent("PositionAnimation");
	}
	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();

	anim->SetInitPosition(gameObject.lock()->transform->GetWorldPosition());
	anim->SetTargetPosition(arg_targetPosition);
	anim->SetSpeed(1.0f / frame);
	anim->SetEaseType(easingType);
}

void ButiEngine::PauseButton::AddScaleAnimation(const Vector3& arg_targetScale, int frame, Easing::EasingType easingType)
{
	auto anim_ = gameObject.lock()->GetGameComponent<ScaleAnimation>();
	if (anim_)
	{
		gameObject.lock()->RemoveGameComponent("ScaleAnimation");
	}
	auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();

	anim->SetInitScale(gameObject.lock()->transform->GetWorldScale());
	anim->SetTargetScale(arg_targetScale);
	anim->SetSpeed(1.0f / frame);
	anim->SetEaseType(easingType);
}
