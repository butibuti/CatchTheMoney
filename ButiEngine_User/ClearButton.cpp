#include "stdafx_u.h"
#include "ClearButton.h"

void ButiEngine::ClearButton::OnUpdate()
{
}

void ButiEngine::ClearButton::OnSet()
{
}

void ButiEngine::ClearButton::Start()
{
	gameObject.lock()->transform->SetLocalScale(Vector3(0, 0, 1));
	defaultScale = Vector3(420, 160, 1);
}

void ButiEngine::ClearButton::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ClearButton::Clone()
{
	return ObjectFactory::Create<ClearButton>();
}

void ButiEngine::ClearButton::Appear()
{
	AddAnimation(defaultScale);
}

void ButiEngine::ClearButton::OnSelected()
{
	AddAnimation(defaultScale * 1.2f);
}

void ButiEngine::ClearButton::OnEndSelect()
{
	AddAnimation(defaultScale);
}

void ButiEngine::ClearButton::AddAnimation(const Vector3& arg_targetScale)
{
	auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (!anim)
	{
		anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	}

	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	anim->GetTargetTransform()->SetLocalScale(arg_targetScale);
	anim->SetSpeed(1.0f / 10);
	anim->SetEaseType(Easing::EasingType::EaseOutExpo);
}