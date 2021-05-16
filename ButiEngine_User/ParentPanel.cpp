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
	wkp_frontPanel.lock()->GetGameComponent<Panel>()->SetParentPanel(gameObject);
}

void ButiEngine::ParentPanel::SetBackPanel(std::weak_ptr<GameObject> arg_backPanel)
{
	wkp_backPanel = arg_backPanel;
	wkp_backPanel.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_backPanel.lock()->GetGameComponent<Panel>()->SetParentPanel(gameObject);
}

void ButiEngine::ParentPanel::SetChildScale()
{
	gameObject.lock()->transform->SetLocalScale(Vector3(1.0f, 1.0f, 1.0f));

	if (wkp_frontPanel.lock())
	{
		wkp_frontPanel.lock()->transform->SetLocalScale(scale);
	}
	if (wkp_backPanel.lock())
	{
		wkp_backPanel.lock()->transform->SetLocalScale(scale);
	}
}
