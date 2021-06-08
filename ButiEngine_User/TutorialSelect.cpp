#include "stdafx_u.h"
#include "TutorialSelect.h"

void ButiEngine::TutorialSelect::OnUpdate()
{
	if (isSelect == isNowSelect)
	{
		const Vector3 MAX_SCALE = Vector3(300, 114, 1);
		ScaleAnimation(MAX_SCALE);
	}
	else
	{
		const Vector3 MIN_SCALE = Vector3(200, 76, 1);
		ScaleAnimation(MIN_SCALE);
	}
}

void ButiEngine::TutorialSelect::OnSet()
{
}

void ButiEngine::TutorialSelect::Start()
{
	isNowSelect = false;

	auto objName = gameObject.lock()->GetGameObjectName();
	if (objName == "Tutorial_No")
	{
		isSelect = false;
	}
	else if (objName == "Tutorial_Yes")
	{
		isSelect = true;
	}
}

void ButiEngine::TutorialSelect::SetNowSelect(bool arg_select)
{
	isNowSelect = arg_select;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::TutorialSelect::Clone()
{
	return ObjectFactory::Create<TutorialSelect>();
}

void ButiEngine::TutorialSelect::ScaleAnimation(const Vector3& arg_scale)
{
	auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (!anim)
	{
		anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(gameObject.lock()->transform->Clone());
		anim->GetTargetTransform()->SetLocalScale(arg_scale);
		anim->SetSpeed(0.2f);
		anim->SetEaseType(Easing::EasingType::EaseOut);
	}
}
