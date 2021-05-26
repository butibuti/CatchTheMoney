#include "stdafx_u.h"
#include "ParentDaikokuten.h"

void ButiEngine::ParentDaikokuten::OnUpdate()
{
	auto objName = gameObject.lock()->GetGameObjectName();
	if (objName == "Daikokuten_Stay")
	{
		StayUpdate();
	}
	else if (objName == "Daikokuten_Appear")
	{
		AppearUpdate();
	}
	else if (objName == "Daikokuten_Reaction")
	{
		ReactionUpdate();
	}
	else if (objName == "Daikokuten_Hand_R")
	{
		RHandUpdate();
	}
	else if (objName == "Daikokuten_Hand_L")
	{
		LHandUpdate();
	}
	else if (objName == "Daikokuten_Appear_R")
	{
		RAppearUpdate();
	}
	else if (objName == "Daikokuten_Appear_L")
	{
		LAppearUpdate();
	}
}

void ButiEngine::ParentDaikokuten::OnSet()
{
}

void ButiEngine::ParentDaikokuten::Start()
{
	isChange = false;
	animationCount = 0;
	previousPos = Vector3::Zero;
	currentPos = Vector3::Zero;


	//auto objName = gameObject.lock()->GetGameObjectName();
	//if (objName == "Daikokuten_Stay")
	//{
	//}
	//else if (objName == "Daikokuten_Appear")
	//{
	//}
	//else if (objName == "Daikokuten_Reaction")
	//{
	//}
	//else if (objName == "Daikokuten_Hand_R")
	//{
	//}
	//else if (objName == "Daikokuten_Hand_L")
	//{
	//}
}

void ButiEngine::ParentDaikokuten::TalkAppear()
{
	gameObject.lock()->transform->SetLocalPostionZ(-1300);
	currentPos.y = -1750;
}

void ButiEngine::ParentDaikokuten::Appear()
{
	auto objName = gameObject.lock()->GetGameObjectName();
	if (objName == "Daikokuten_Appear")
	{
		gameObject.lock()->transform->SetLocalPostionZ(0);
		currentPos.y = -1750;
	}
	else if (objName == "Daikokuten_Appear_R")
	{
		currentPos.x = 1200;
	}
	else if (objName == "Daikokuten_Appear_L")
	{
		currentPos.x = -1200;
	}
}

void ButiEngine::ParentDaikokuten::Disappear()
{
	auto objName = gameObject.lock()->GetGameObjectName();
	if (objName == "Daikokuten_Appear")
	{
		currentPos.y = 0;
	}
	else if (objName == "Daikokuten_Appear_R")
	{
		currentPos.x = 0;
	}
	else if (objName == "Daikokuten_Appear_L")
	{
		currentPos.x = 0;
	}

}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ParentDaikokuten::Clone()
{
	return ObjectFactory::Create<ParentDaikokuten>();
}

void ButiEngine::ParentDaikokuten::StayUpdate()
{
	const float LERP_SCALE = 0.02f;
	previousPos.y = previousPos.y * (1.0f - LERP_SCALE) + currentPos.y * LERP_SCALE;
	gameObject.lock()->transform->SetLocalPostionY(previousPos.y);
	ChangeTimer(0, 100);
}

void ButiEngine::ParentDaikokuten::AppearUpdate()
{
	const float LERP_SCALE = 0.12f;
	previousPos.y = previousPos.y * (1.0f - LERP_SCALE) + currentPos.y * LERP_SCALE;
	gameObject.lock()->transform->SetLocalPostionY(previousPos.y);
}

void ButiEngine::ParentDaikokuten::ReactionUpdate()
{

}

void ButiEngine::ParentDaikokuten::RHandUpdate()
{
	const float LERP_SCALE = 0.02f;
	previousPos.x = previousPos.x * (1.0f - LERP_SCALE) + currentPos.x * LERP_SCALE;
	gameObject.lock()->transform->SetLocalPostionX(previousPos.x);
	ChangeTimer(0, 50);
}

void ButiEngine::ParentDaikokuten::LHandUpdate()
{
	const float LERP_SCALE = 0.02f;
	previousPos.x = previousPos.x * (1.0f - LERP_SCALE) + currentPos.x * LERP_SCALE;
	gameObject.lock()->transform->SetLocalPostionX(previousPos.x);
	ChangeTimer(0, -50);
}

void ButiEngine::ParentDaikokuten::RAppearUpdate()
{
	const float LERP_SCALE = 0.12f;
	previousPos.x = previousPos.x * (1.0f - LERP_SCALE) + currentPos.x * LERP_SCALE;
	gameObject.lock()->transform->SetLocalPostionX(previousPos.x);
}

void ButiEngine::ParentDaikokuten::LAppearUpdate()
{
	const float LERP_SCALE = 0.12f;
	previousPos.x = previousPos.x * (1.0f - LERP_SCALE) + currentPos.x * LERP_SCALE;
	gameObject.lock()->transform->SetLocalPostionX(previousPos.x);
}

void ButiEngine::ParentDaikokuten::ChangeTimer(const float arg_startPos, const float arg_endPos)
{
	if (isChange)
	{
		currentPos.x = arg_endPos;
		currentPos.y = arg_endPos;
		animationCount--;
	}
	else
	{
		currentPos.x = arg_startPos;
		currentPos.y = arg_startPos;
		animationCount++;
	}

	if (animationCount > 60 || animationCount < 0)
	{
		isChange = !isChange;
	}
}
