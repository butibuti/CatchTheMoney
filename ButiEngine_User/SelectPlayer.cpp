#include "stdafx_u.h"
#include "SelectPlayer.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"

void ButiEngine::SelectPlayer::OnUpdate()
{
	if (isAway)
	{
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, -3000, 0));
		return;
	}
	if (isDecision)
	{
		DecisionAnimation();
	}    
	else
	{
		if (animationFrame < 4)
		{
			animationFrame++;
		}
		else
		{
			animationFrame = 0;
			shp_spriteAnimation->UpdateHorizontalAnim(1);
		}
	}
	gameObject.lock()->transform->SetLocalPosition(position);
}

void ButiEngine::SelectPlayer::OnSet()
{
}

void ButiEngine::SelectPlayer::Start()
{
	isDecision = false;
	animationFrame = 0;
	animationTime = 0;
	velocityX = -1.0f;
	position = Vector3(-400, -400, 0);
	gameObject.lock()->transform->SetLocalPosition(position);
	gameObject.lock()->transform->SetLocalScale(Vector3(320, 320, 0));
	gameObject.lock()->transform->SetLocalRotation(Vector3(0, 0, 0));
	shp_spriteAnimation = gameObject.lock()->GetGameComponent<SpliteAnimationComponent>();
	shp_spriteAnimation->SetHorizontalAnim(0);
}

void ButiEngine::SelectPlayer::Decision()
{
	isDecision = true;
	shp_spriteAnimation->SetHorizontalAnim(1);
}

void ButiEngine::SelectPlayer::Away()
{
	isAway = true;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::SelectPlayer::Clone()
{
	return ObjectFactory::Create<SelectPlayer>();
}

void ButiEngine::SelectPlayer::DecisionAnimation()
{
	animationTime++;

	if (animationTime < 30)
	{
		velocityX += 0.05f;
		position.x += velocityX;
	}
	else if (animationTime == 30)
	{
		position.y = -4000;
	}
	else if (animationTime == 50)
	{
		position = Vector3(100, 400, 0);
		gameObject.lock()->transform->SetLocalScale(Vector3(80, 80, 1));
		gameObject.lock()->transform->SetLocalRotation(Vector3(-180, 0, -180));
	}

}
