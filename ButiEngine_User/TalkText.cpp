#include "stdafx_u.h"
#include "TalkText.h"
#include "StageSelect.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"

bool ButiEngine::TalkText::isDelete = false;

void ButiEngine::TalkText::OnUpdate()
{
	if (isDelete)
	{
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, -3000, 0));
		return;
	}


	if (InputManager::OnTriggerDecisionKey())
	{
		textCount++;
		if (textCount < shp_spriteAnimation->GetVarticalSplitScale())
		{
			shp_spriteAnimation->UpdateVarticalAnim(1);
		}
		else
		{
			isDelete = true;
		}

	}
}

void ButiEngine::TalkText::OnSet()
{
}

void ButiEngine::TalkText::Start()
{
	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	textCount = 0;
	isDelete = false;

	switch (StageSelect::GetStageNum())
	{
	case 0:
		shp_spriteAnimation->SetVarticalAnim(0);
		break;
	default:
		break;
	}
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::TalkText::Clone()
{
	return ObjectFactory::Create<TalkText>();
}

void ButiEngine::TalkText::Delete()
{
	isDelete = true;
}

void ButiEngine::TalkText::Stage0Text()
{

}
