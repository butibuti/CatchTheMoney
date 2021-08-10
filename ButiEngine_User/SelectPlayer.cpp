#include "stdafx_u.h"
#include "SelectPlayer.h"
#include "Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"

void ButiEngine::SelectPlayer::OnUpdate()
{
	if (isAway)
	{
		const float AWAY_POS = -4000;
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, AWAY_POS, 0));
		return;
	}
	if (isDecision)
	{
		DecisionAnimation();
	}    
	else
	{
		const int ANIMATION_RATE = 4;
		if (animationFrame < ANIMATION_RATE)
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

	const int RUN_UP_FRAME = 30;
	const int RESPAWN_FRAME = 50;
	if (animationTime < RUN_UP_FRAME)
	{
		const float FRICTION = 0.05f;
		velocityX += FRICTION;
		position.x += velocityX;
	}
	else if (animationTime == RUN_UP_FRAME)
	{
		const float AWAY_POS = -4000;
		position.y = AWAY_POS;
	}
	else if (animationTime == RESPAWN_FRAME)
	{
		position = Vector3(100, 400, 0);
		gameObject.lock()->transform->SetLocalScale(Vector3(80, 80, 1));
		gameObject.lock()->transform->SetLocalRotation(Vector3(-180, 0, -180));
	}
}
