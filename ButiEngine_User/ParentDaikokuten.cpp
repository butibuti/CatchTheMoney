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
	isReactionScale = false;
	animationCount = 0;
	movePos = 0;
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

void ButiEngine::ParentDaikokuten::Reaction(bool arg_isReactionScale)
{
	isReactionScale = arg_isReactionScale;
	isOneLoop = true;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ParentDaikokuten::Clone()
{
	return ObjectFactory::Create<ParentDaikokuten>();
}

void ButiEngine::ParentDaikokuten::StayUpdate()
{
	//const float LERP_SCALE = 0.02f;
	//previousPos.y = previousPos.y * (1.0f - LERP_SCALE) + currentPos.y * LERP_SCALE;
	//gameObject.lock()->transform->SetLocalPostionY(previousPos.y);
	//ChangeTimer(0, 100);

	LoopAnimation(0, 50, false);

	//auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	//if (!anim && !isChange)
	//{
	//	anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	//	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	//	anim->GetTargetTransform()->SetLocalPostionY(50);
	//	anim->SetSpeed(0.0083f);
	//	anim->SetEaseType(Easing::EasingType::EaseInOut);
	//	isChange = true;
	//}
	//else if (!anim)
	//{
	//	anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	//	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	//	anim->GetTargetTransform()->SetLocalPostionY(0);
	//	anim->SetSpeed(0.0083f);
	//	anim->SetEaseType(Easing::EasingType::EaseInOut);
	//	isChange = false;
	//}
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

	float speed = 0;
	if (isReactionScale)
	{
		movePos = -100;
		speed = 0.05f;
	}
	else
	{
		movePos = -50;
		speed = 0.15f;
	}

	auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (!anim && !isChange)
	{
		//o‚é‚Æ‚«
		anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(gameObject.lock()->transform->Clone());
		anim->GetTargetTransform()->SetLocalPostionZ(movePos);
		anim->SetSpeed(speed);
		anim->SetEaseType(Easing::EasingType::EaseOutExpo);
		isChange = true;
	}
	else if (!anim)
	{
		//ˆø‚­‚Æ‚«
		anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(gameObject.lock()->transform->Clone());
		anim->GetTargetTransform()->SetLocalPostionZ(0);
		anim->SetSpeed(speed);
		anim->SetEaseType(Easing::EasingType::EaseInExpo);
		isChange = false;
		isOneLoop = false;
	}
}

void ButiEngine::ParentDaikokuten::RHandUpdate()
{
	//const float LERP_SCALE = 0.02f;
	//previousPos.x = previousPos.x * (1.0f - LERP_SCALE) + currentPos.x * LERP_SCALE;
	//gameObject.lock()->transform->SetLocalPostionX(previousPos.x);
	//ChangeTimer(0, 50);

	LoopAnimation(0, 100, true);

	//auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	//if (!anim && !isChange)
	//{
	//	anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	//	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	//	anim->GetTargetTransform()->SetLocalPostionX(100);
	//	anim->SetSpeed(0.0083f);
	//	anim->SetEaseType(Easing::EasingType::EaseInOut);
	//	isChange = true;
	//}
	//else if (!anim)
	//{
	//	anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	//	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	//	anim->GetTargetTransform()->SetLocalPostionX(0);
	//	anim->SetSpeed(0.0083f);
	//	anim->SetEaseType(Easing::EasingType::EaseInOut);
	//	isChange = false;
	//}
}

void ButiEngine::ParentDaikokuten::LHandUpdate()
{
	//const float LERP_SCALE = 0.02f;
	//previousPos.x = previousPos.x * (1.0f - LERP_SCALE) + currentPos.x * LERP_SCALE;
	//gameObject.lock()->transform->SetLocalPostionX(previousPos.x);
	//ChangeTimer(0, -50);

	LoopAnimation(0, -100, true);

	//auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	//if (!anim && !isChange)
	//{
	//	anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	//	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	//	anim->GetTargetTransform()->SetLocalPostionX(-100);
	//	anim->SetSpeed(0.0083f);
	//	anim->SetEaseType(Easing::EasingType::EaseInOut);
	//	isChange = true;
	//}
	//else if (!anim)
	//{
	//	anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	//	anim->SetTargetTransform(gameObject.lock()->transform->Clone());
	//	anim->GetTargetTransform()->SetLocalPostionX(0);
	//	anim->SetSpeed(0.0083f);
	//	anim->SetEaseType(Easing::EasingType::EaseInOut);
	//	isChange = false;
	//}
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

void ButiEngine::ParentDaikokuten::LoopAnimation(const float arg_startPos, const float arg_endPos, bool arg_isX)
{
	auto anim = gameObject.lock()->GetGameComponent<TransformAnimation>();
	if (!anim && !isChange)
	{
		anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(gameObject.lock()->transform->Clone());
		if (arg_isX)
		{
			anim->GetTargetTransform()->SetLocalPostionX(arg_endPos);
		}
		else
		{
			anim->GetTargetTransform()->SetLocalPostionY(arg_endPos);
		}
		anim->SetSpeed(0.0166f);
		anim->SetEaseType(Easing::EasingType::EaseInOut);
		isChange = true;
	}
	else if (!anim)
	{
		anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
		anim->SetTargetTransform(gameObject.lock()->transform->Clone());
		if (arg_isX)
		{
			anim->GetTargetTransform()->SetLocalPostionX(arg_startPos);
		}
		else
		{
			anim->GetTargetTransform()->SetLocalPostionY(arg_startPos);
		}
		anim->SetSpeed(0.0166f);
		anim->SetEaseType(Easing::EasingType::EaseInOut);
		isChange = false;
	}
}
