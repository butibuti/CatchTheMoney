#include "stdafx_u.h"
#include "Player.h"
#include"MobiusMover.h"

void ButiEngine::Player::OnUpdate()
{
	const float speed = 10.0f;
	if (GameDevice::GetInput()->CheckKey(Keys::A))
	{
		progress += speed;
	}
	if (GameDevice::GetInput()->CheckKey(Keys::D))
	{
		progress -= speed;
	}
	gameObject.lock()->GetGameComponent<MobiusMover>()->SetProgress(progress);
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::Start()
{
	progress = 0.0f;
}

void ButiEngine::Player::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}
