#include "stdafx_u.h"
#include "RotateButtonUI.h"
#include"Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"

void ButiEngine::RotateButtonUI::OnUpdate()
{
}

void ButiEngine::RotateButtonUI::OnSet()
{
}

void ButiEngine::RotateButtonUI::Start()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::RotateButtonUI::Clone()
{
	return ObjectFactory::Create<RotateButtonUI>();
}

void ButiEngine::RotateButtonUI::Appear()
{
	auto scaleAnim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	scaleAnim->SetInitScale(Vector3(290.0f, 290.0f, 1.0f));
	scaleAnim->SetTargetScale(Vector3(310.0f, 310.0f, 1.0f));
	scaleAnim->SetReverse(true);
	scaleAnim->SetSpeed(1.0f / 30);

	auto appearScaleAnim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	appearScaleAnim->SetInitScale(0.0f);
	appearScaleAnim->SetTargetScale(Vector3(300.0f, 300.0f, 1.0f));
	appearScaleAnim->SetSpeed(1.0f / 30);
}

void ButiEngine::RotateButtonUI::Disappear()
{
	gameObject.lock()->RemoveGameComponent("ScaleAnimation");

	auto scaleAnim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	scaleAnim->SetInitScale(Vector3(300.0f, 300.0f, 1.0f));
	scaleAnim->SetTargetScale(0.0f);
	scaleAnim->SetSpeed(1.0f / 30);
}
