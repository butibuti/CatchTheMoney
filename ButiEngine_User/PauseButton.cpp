#include "stdafx_u.h"
#include "PauseButton.h"

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
	AddAnimation(defaultPosition, defaultScale, 30, Easing::EasingType::EaseOutBack);
}

void ButiEngine::PauseButton::Disappear()
{
	AddAnimation(initPosition, defaultScale, 30, Easing::EasingType::EaseInBack);
}

void ButiEngine::PauseButton::OnSelected()
{
	AddAnimation(defaultPosition, defaultScale * 1.2f, 40, Easing::EasingType::EaseOutElastic);
}

void ButiEngine::PauseButton::OnEndSelect()
{
	AddAnimation(defaultPosition, defaultScale, 40, Easing::EasingType::EaseOutElastic);
}

void ButiEngine::PauseButton::AddAnimation(const Vector3& arg_targetPosition, const Vector3& arg_targetScale, int frame, Easing::EasingType easingType)
{
	auto anim_ = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (anim_)
	{
		gameObject.lock()->RemoveGameComponent("TransformAnimation");
	}
	auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();

	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	anim->GetTargetTransform()->SetWorldPosition(arg_targetPosition);
	anim->GetTargetTransform()->SetLocalScale(arg_targetScale);
	anim->SetSpeed(1.0f / frame);
	anim->SetEaseType(easingType);
}
