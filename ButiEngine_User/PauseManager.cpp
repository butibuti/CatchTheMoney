#include "stdafx_u.h"
#include "PauseManager.h"
#include"PauseButton.h"

void ButiEngine::PauseManager::OnUpdate()
{
	if (GameDevice::GetInput()->TriggerKey(Keys::P) && !pushPauseKey)
	{
		pushPauseKey = true;
		if (!pause)
		{
			AppearUI();
		}
		else
		{
			DisappearUI();
		}
	}
	SwitchPause();
}

void ButiEngine::PauseManager::OnSet()
{
}

void ButiEngine::PauseManager::Start()
{
	wkp_text = GetManager().lock()->AddObjectFromCereal("PauseText");
	wkp_background = GetManager().lock()->AddObjectFromCereal("PauseBackground");
	wkp_button_back = GetManager().lock()->AddObjectFromCereal("PauseButton_Back");
	wkp_button_reset = GetManager().lock()->AddObjectFromCereal("PauseButton_Reset");
	wkp_button_select = GetManager().lock()->AddObjectFromCereal("PauseButton_Select");

	initBGScale = Vector3(0.0f, 0.0f, 1.0f);
	defaultBGScale = Vector3(1920.0f, 1080.0f, 1.0f);
	initTextPos = wkp_text.lock()->transform->GetWorldPosition();
	textScale = wkp_text.lock()->transform->GetWorldScale();
	defaultTextPos = Vector3(0.0f, 100.0f, -0.5f);
	BGPos = Vector3(0.0f, 0.0f, -0.4f);

	pause = false;
	pushPauseKey = false;
	progress = 0;
}

void ButiEngine::PauseManager::OnShowUI()
{
}

void ButiEngine::PauseManager::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::PauseManager::Clone()
{
	return ObjectFactory::Create<PauseManager>();
}

void ButiEngine::PauseManager::SwitchPause()
{
	if (!pushPauseKey) { return; }
	if (pause)
	{
		if (wkp_button_back.lock()->GetGameComponent<TransformAnimation>()) { return; }
		pause = false;
		pushPauseKey = false;
	}
	else
	{
		pause = true;
		pushPauseKey = false;
	}
}

void ButiEngine::PauseManager::AppearUI()
{
	AddAnimation(wkp_text, defaultTextPos, textScale, 30, Easing::EasingType::EaseOutBack);
	AddAnimation(wkp_background, BGPos, defaultBGScale, 10, Easing::EasingType::EaseInQuad);
	wkp_button_back.lock()->GetGameComponent<PauseButton>()->Appear();
	wkp_button_reset.lock()->GetGameComponent<PauseButton>()->Appear();
	wkp_button_select.lock()->GetGameComponent<PauseButton>()->Appear();
}

void ButiEngine::PauseManager::DisappearUI()
{
	AddAnimation(wkp_text, initTextPos, textScale, 30, Easing::EasingType::EaseInBack);
	AddAnimation(wkp_background, BGPos, initBGScale, 10, Easing::EasingType::EaseOutQuad);
	wkp_button_back.lock()->GetGameComponent<PauseButton>()->Disappear();
	wkp_button_reset.lock()->GetGameComponent<PauseButton>()->Disappear();
	wkp_button_select.lock()->GetGameComponent<PauseButton>()->Disappear();
}

void ButiEngine::PauseManager::AddAnimation(std::weak_ptr<GameObject> arg_object, const Vector3& arg_targetPosition, const Vector3& arg_targetScale, int frame, Easing::EasingType easingType)
{
	auto anim_ = arg_object.lock()->GetGameComponent<TransformAnimation>();
	if (anim_)
	{
		arg_object.lock()->RemoveGameComponent("TransformAnimation");
	}
	auto anim = arg_object.lock()->AddGameComponent<TransformAnimation>();

	anim->SetTargetTransform(arg_object.lock()->transform->Clone());
	anim->GetTargetTransform()->SetWorldPosition(arg_targetPosition);
	anim->GetTargetTransform()->SetLocalScale(arg_targetScale);
	anim->SetSpeed(1.0f / frame);
	anim->SetEaseType(easingType);
}
