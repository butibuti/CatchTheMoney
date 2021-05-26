#include "stdafx_u.h"
#include "Daikokuten.h"
#include "StageManager.h"

void ButiEngine::Daikokuten::OnUpdate()
{
}

void ButiEngine::Daikokuten::OnSet()
{
}

void ButiEngine::Daikokuten::Start()
{
}

void ButiEngine::Daikokuten::TalkScale()
{
	gameObject.lock()->transform->SetLocalScale(Vector3(100, 100, 100));
}

void ButiEngine::Daikokuten::NormalScale()
{
	gameObject.lock()->transform->SetLocalScale(Vector3(350, 350, 350));
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Daikokuten::Clone()
{
	return ObjectFactory::Create<Daikokuten>();
}
