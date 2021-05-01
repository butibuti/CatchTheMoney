#include "stdafx_u.h"
#include "ParentPanel.h"
#include"GameSettings.h"
#include"Panel.h"

void ButiEngine::ParentPanel::OnUpdate()
{
	SetChildScale();
}

void ButiEngine::ParentPanel::OnSet()
{
}

void ButiEngine::ParentPanel::Start()
{
	scale = Vector3(1.0f, 1.0f, 1.0f);
}

void ButiEngine::ParentPanel::OnShowUI()
{
	GUI::SliderFloat3("scale", scale, 0.0f, 3.0f);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ParentPanel::Clone()
{
	return ObjectFactory::Create<ParentPanel>();
}

void ButiEngine::ParentPanel::SetFrontPanel(std::weak_ptr<GameObject> arg_frontPanel)
{
	wkp_frontPanel = arg_frontPanel;
	wkp_frontPanel.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_frontPanel.lock()->GetGameComponent<Panel>()->SetParentPanelNum(panelNum);
}

void ButiEngine::ParentPanel::SetBackPanel(std::weak_ptr<GameObject> arg_backPanel)
{
	wkp_backPanel = arg_backPanel;
	wkp_backPanel.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_backPanel.lock()->GetGameComponent<Panel>()->SetParentPanelNum(panelNum);
}

void ButiEngine::ParentPanel::SetPanelNum(int arg_num, bool arg_addAnimation)
{
	return;
	panelNum = arg_num;
	SetChildPanelNum();
	if (!arg_addAnimation) { return; }
	AddTransformAnimation();
}

void ButiEngine::ParentPanel::SetChildPanelNum()
{
	return;
	if (wkp_frontPanel.lock())
	{
		wkp_frontPanel.lock()->GetGameComponent<Panel>()->SetParentPanelNum(panelNum);
	}
	if (wkp_backPanel.lock())
	{
		wkp_backPanel.lock()->GetGameComponent<Panel>()->SetParentPanelNum(panelNum);
	}
}

void ButiEngine::ParentPanel::SetChildScale()
{
	gameObject.lock()->transform->SetLocalScale(1.0f);

	if (wkp_frontPanel.lock())
	{
		wkp_frontPanel.lock()->transform->SetLocalScale(scale);
	}
	if (wkp_backPanel.lock())
	{
		wkp_backPanel.lock()->transform->SetLocalScale(scale);
	}
}

void ButiEngine::ParentPanel::AddTransformAnimation()
{
	return;
	Vector3 targetPos;
	targetPos.x = (panelNum - GameSettings::panelCount / 2) * GameSettings::panelWidth + GameSettings::panelWidth * 0.5f;

	auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	anim->GetTargetTransform()->SetWorldPosition(targetPos);
	anim->SetSpeed(1.0f / 120.0f);
	anim->SetEaseType(Easing::EasingType::EaseOutQuart);
}
