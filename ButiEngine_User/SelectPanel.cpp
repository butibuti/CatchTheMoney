#include "stdafx_u.h"
#include "SelectPanel.h"
#include "ParentSelectPanel.h"
#include "NumberComponent.h"

void ButiEngine::SelectPanel::OnUpdate()
{
	CreateNumber();
}

void ButiEngine::SelectPanel::OnSet()
{
}

void ButiEngine::SelectPanel::Start()
{
	frontStageNum = 0;
	backStageNum = 0;
	isOnce = false;

	wkp_parentSelectPanel = GetManager().lock()->GetGameObject("ParentSelectPanel");
	gameObject.lock()->transform->SetBaseTransform(wkp_parentSelectPanel.lock()->transform);
	
	Vector3 scale = Vector3(1.0f, 1.0f, 0.1f);
	gameObject.lock()->transform->SetLocalScale(scale);

	//wkp_frontNumber = GetManager().lock()->AddObjectFromCereal("ParentNumber", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetLocalPosition(), gameObject.lock()->transform->GetLocalRotation(), gameObject.lock()->transform->GetLocalScale()));
	//wkp_backNumber = GetManager().lock()->AddObjectFromCereal("ParentNumber", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetLocalPosition(), gameObject.lock()->transform->GetLocalRotation(), gameObject.lock()->transform->GetLocalScale()));
	//wkp_frontNumber.lock()->transform->SetBaseTransform(gameObject.lock()->transform->GetBaseTransform());
	//wkp_backNumber.lock()->transform->SetBaseTransform(gameObject.lock()->transform->GetBaseTransform());
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SelectPanel::Clone()
{
	return ObjectFactory::Create<SelectPanel>();
}

void ButiEngine::SelectPanel::SetStageNumbers(const int arg_front, const int arg_back)
{
	frontStageNum = arg_front;
	backStageNum = arg_back;
}

void ButiEngine::SelectPanel::CreateNumber()
{
	if (isOnce) return;
	isOnce = true;

	//number create!!!! 

}
