#include "stdafx_u.h"
#include "Daikokuten.h"
#include "StageManager.h"
#include "PauseManager.h"
#include "Header/GameObjects/DefaultGameComponent/SimpleBoneAnimator.Component.h"

void ButiEngine::Daikokuten::OnUpdate()
{
	if (PauseManager::IsPause())
	{
		if (gameObject.lock()->GetGameObjectName() == "DaikokutenHand_L" ||
			gameObject.lock()->GetGameObjectName() == "DaikokutenHand_R")
		{
			gameObject.lock()->GetGameComponent<SimpleBoneAnimatorComponent>()->SetIsActive(false);
		}
		isActiveChange = true;
		return;
	}
	
	if(isActiveChange)
	{
		isActiveChange = false;

		if (gameObject.lock()->GetGameObjectName() == "DaikokutenHand_L" ||
			gameObject.lock()->GetGameObjectName() == "DaikokutenHand_R")
		{
			gameObject.lock()->GetGameComponent<SimpleBoneAnimatorComponent>()->SetIsActive(true);
		}
	}

}

void ButiEngine::Daikokuten::OnSet()
{
}

void ButiEngine::Daikokuten::Start()
{
	isActiveChange = false;
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
