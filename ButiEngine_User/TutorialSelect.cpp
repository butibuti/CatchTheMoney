#include "stdafx_u.h"
#include "TutorialSelect.h"

void ButiEngine::TutorialSelect::OnUpdate()
{
	if (isSelect == isNowSelect)
	{
		auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
		if (!anim)
		{
			anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
			anim->SetTargetTransform(gameObject.lock()->transform->Clone());
			anim->GetTargetTransform()->SetLocalScale(Vector3(300, 114, 1));
			anim->SetSpeed(0.2f);
			anim->SetEaseType(Easing::EasingType::EaseOut);
		}
	}
	else
	{
		auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
		if (!anim)
		{
			anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
			anim->SetTargetTransform(gameObject.lock()->transform->Clone());
			anim->GetTargetTransform()->SetLocalScale(Vector3(200, 76, 1));
			anim->SetSpeed(0.2f);
			anim->SetEaseType(Easing::EasingType::EaseOut);
		}
	}

	//if (gameObject.lock()->GetGameObjectName() == "Tutorial_No")
	//{
	//	
	//}
	//else if (gameObject.lock()->GetGameObjectName() == "Tutorial_Yes")
	//{
	//	
	//}
}

void ButiEngine::TutorialSelect::OnSet()
{
}

void ButiEngine::TutorialSelect::Start()
{
	isNowSelect = false;
	if (gameObject.lock()->GetGameObjectName() == "Tutorial_No")
	{
		isSelect = false;
	}
	else if (gameObject.lock()->GetGameObjectName() == "Tutorial_Yes")
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
