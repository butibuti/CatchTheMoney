#include "stdafx_u.h"
#include "TalkText.h"
#include "StageSelect.h"
#include "InputManager.h"
#include "ShakeComponent.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"

bool ButiEngine::TalkText::isDelete = false;

void ButiEngine::TalkText::OnUpdate()
{
	if (isDelete)
	{
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, -3000, 0));
		return;
	}

	if (!isOnce)
	{
		isOnce = true;
		//GetManager().lock()->GetGameObject("TextWindow").lock()->transform->SetLocalScale(Vector3(2000, 720, 0));
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8, 30);
	}

	if (InputManager::OnTriggerDecisionKey() && waitTime > 12)
	{
		waitTime = 0;
		textCount++;
		ShakeAimation();
		if (textCount < shp_spriteAnimation->GetVarticalSplitScale())
		{
			shp_spriteAnimation->UpdateVarticalAnim(1);
		}
		else
		{
			isDelete = true;
		}
	}
	if (InputManager::OnTextSkipKey())
	{
		isDelete = true;
	}
	if(waitTime < 60)
	{
		waitTime++;
	}
}

void ButiEngine::TalkText::OnSet()
{
}

void ButiEngine::TalkText::Start()
{
	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	textCount = 0;
	waitTime = 0;
	isDelete = false;
	isOnce = false;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::TalkText::Clone()
{
	return ObjectFactory::Create<TalkText>();
}

void ButiEngine::TalkText::Delete()
{
	isDelete = true;
}

void ButiEngine::TalkText::ShakeAimation()
{
	if (StageSelect::GetStageNum() == 0)
	{
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
	}
	else if (StageSelect::GetStageNum() == 3)
	{
		if (textCount == 4 || textCount == 6)
		{
			//GetManager().lock()->GetGameObject("TextWindow").lock()->transform->SetLocalScale(Vector3(1920, 640, 0));
			return;
		}
		//GetManager().lock()->GetGameObject("TextWindow").lock()->transform->SetLocalScale(Vector3(2000, 720, 0));
		GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
	}
	else if (StageSelect::GetStageNum() == 6)
	{
		if (textCount == 0)
		{
			GetManager().lock()->GetGameObject("TextWindow").lock()->GetGameComponent<ShakeComponent>()->ShakeStart(8);
		}
	}
}
