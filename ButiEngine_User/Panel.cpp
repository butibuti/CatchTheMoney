#include "stdafx_u.h"
#include "Panel.h"
#include"GameSettings.h"

void ButiEngine::Panel::OnUpdate()
{
}

void ButiEngine::Panel::OnSet()
{
}

void ButiEngine::Panel::Start()
{
	Vector3 position;
	position.z = -0.1f;
	Vector3 scale;
	scale.x = GameSettings::panelWidth;
	scale.y = GameSettings::panelHeight;
	wkp_drawObject = GetManager().lock()->AddObjectFromCereal("PanelForDraw", ObjectFactory::Create<Transform>(position, Vector3::Zero, scale));
	wkp_drawObject.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
}

void ButiEngine::Panel::OnShowUI()
{
	GUI::Text("x:%f", gameObject.lock()->transform->GetWorldPosition().x);
	GUI::Text("y:%f", gameObject.lock()->transform->GetWorldPosition().y);
	GUI::Text("panelNum:%d", panelNum);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Panel::Clone()
{
	return ObjectFactory::Create<Panel>();
}

void ButiEngine::Panel::AddTransformAnimation()
{
	Vector3 targetPos;
	targetPos.x = (panelNum - GameSettings::panelCount / 2) * GameSettings::panelWidth + GameSettings::panelWidth * 0.5f;

	auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	anim->GetTargetTransform()->SetWorldPosition(targetPos);
	anim->SetSpeed(1.0f / 120.0f);
	anim->SetEaseType(Easing::EasingType::EaseOutQuart);

	if (panelNum < 0)
	{
		panelNum = GameSettings::panelCount + panelNum;
	}
	else if (panelNum >= GameSettings::panelCount)
	{
		panelNum -= GameSettings::panelCount;
	}
}
