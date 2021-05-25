#include "stdafx_u.h"
#include "Daikokuten.h"
#include "StageManager.h"

void ButiEngine::Daikokuten::OnUpdate()
{
	if (moveCount > 0)
	{
		moveCount--;
	}
	else
	{
		if (isAppear)
		{
			AlwaysAnimation();
		}
		return;
	}

	AppearAnimation();

}

void ButiEngine::Daikokuten::OnSet()
{
}

void ButiEngine::Daikokuten::Start()
{
	initPos = 0;
	stayPos = 0;
	moveCount = 0;
	animationTime = 0;
	isChange = false;
	isAppear = false;

	auto objctName = gameObject.lock()->GetGameObjectName();
	if (objctName == "Daikokuten")
	{
		initPos = 1800;
		stayPos = 50;
		animationPos = 150;
		gameObject.lock()->transform->SetLocalPostionY(initPos);
	}
	else if (objctName == "DaikokutenHand_L")
	{
		initPos = 2000;
		stayPos = 750;
		animationPos = 800;
		gameObject.lock()->transform->SetLocalPostionX(initPos);
	}
	else if (objctName == "DaikokutenHand_R")
	{
		initPos = -2000;
		stayPos = -750;
		animationPos = -800;
		gameObject.lock()->transform->SetLocalPostionX(initPos);
	}

	previousPos = initPos;
	currentPos = initPos;
}

void ButiEngine::Daikokuten::Disappear()
{
	isAppear = false;
	currentPos = initPos;
	moveCount = 40;
}

void ButiEngine::Daikokuten::Appear()
{
	isAppear = true;
	currentPos = stayPos;
	moveCount = 40;
}

void ButiEngine::Daikokuten::TalkDisappear()
{
	gameObject.lock()->transform->SetLocalPostionZ(900);
	gameObject.lock()->transform->SetLocalScale(Vector3(350, 350, 350));
}

void ButiEngine::Daikokuten::TalkAppear()
{
	gameObject.lock()->transform->SetLocalPostionZ(-200);
	gameObject.lock()->transform->SetLocalScale(Vector3(200, 200, 200));
	isAppear = true;
	currentPos = stayPos;
	moveCount = 40;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Daikokuten::Clone()
{
	return ObjectFactory::Create<Daikokuten>();
}

void ButiEngine::Daikokuten::AppearAnimation()
{
	const float LERP_SCALE = 0.12f;
	previousPos = previousPos * (1.0f - LERP_SCALE) + currentPos * LERP_SCALE;

	auto objctName = gameObject.lock()->GetGameObjectName();
	if (objctName == "Daikokuten")
	{
		gameObject.lock()->transform->SetLocalPostionY(previousPos);
	}
	else
	{
		gameObject.lock()->transform->SetLocalPostionX(previousPos);
	}
}

void ButiEngine::Daikokuten::AlwaysAnimation()
{
	if (animationTime > 1.0f)
	{
		isChange = true;
		currentPos = animationPos;
	}
	else if (animationTime < 0.0f)
	{
		isChange = false;
		currentPos = stayPos;
	}

	if (!isChange)
	{
		animationTime += 0.015f;
	}
	else
	{
		animationTime -= 0.015f;
	}

	const float LERP_SCALE = 0.015f;
	previousPos = previousPos * (1.0f - LERP_SCALE) + currentPos * LERP_SCALE;

	auto objctName = gameObject.lock()->GetGameObjectName();
	if (objctName == "Daikokuten")
	{
		gameObject.lock()->transform->SetLocalPostionY(previousPos);
	}
	else
	{
		gameObject.lock()->transform->SetLocalPostionX(previousPos);
	}
}
