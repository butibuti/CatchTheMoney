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
	isOneLoop = false;
	animationCount = 0;
	previousPos = Vector3::Zero;
	currentPos = Vector3::Zero;
}

void ButiEngine::ParentDaikokuten::TalkAppear()
{
	gameObject.lock()->transform->SetLocalPostionZ(-1500);
	currentPos.y = -1800;
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

void ButiEngine::ParentDaikokuten::Reaction()
{
	isOneLoop = true;
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
	if (!isOneLoop)
	{
		return;
	}
	const float initPos = 0;
	const float movePos = -180;

	auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (!anim && !isChange)
	{
		//o‚é‚Æ‚«
		anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(gameObject.lock()->transform->Clone());
		anim->GetTargetTransform()->SetLocalPostionZ(movePos);
		anim->SetSpeed(0.2f);
		anim->SetEaseType(Easing::EasingType::EaseOut);
		isChange = true;
	}
	else if (!anim)
	{
		//ˆø‚­‚Æ‚«
		anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(gameObject.lock()->transform->Clone());
		anim->GetTargetTransform()->SetLocalPostionZ(initPos);
		anim->SetSpeed(0.2f);
		anim->SetEaseType(Easing::EasingType::EaseIn);
		isChange = false;
		isOneLoop = false;
	}
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
