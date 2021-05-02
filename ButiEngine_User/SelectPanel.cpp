#include "stdafx_u.h"
#include "SelectPanel.h"
#include "ParentSelectPanel.h"

void ButiEngine::SelectPanel::OnUpdate()
{
}

void ButiEngine::SelectPanel::OnSet()
{
}

void ButiEngine::SelectPanel::Start()
{
	wkp_parentSelectPanel = GetManager().lock()->GetGameObject("ParentSelectPanel");
	gameObject.lock()->transform->SetBaseTransform(wkp_parentSelectPanel.lock()->transform);
}

void ButiEngine::SelectPanel::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SelectPanel::Clone()
{
	return ObjectFactory::Create<SelectPanel>();
}
