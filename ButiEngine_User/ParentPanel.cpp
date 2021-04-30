#include "stdafx_u.h"
#include "ParentPanel.h"

void ButiEngine::ParentPanel::OnUpdate()
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
