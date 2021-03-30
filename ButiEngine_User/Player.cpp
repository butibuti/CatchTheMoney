#include "stdafx_u.h"
#include "Player.h"
#include"GameSettings.h"

void ButiEngine::Player::OnUpdate()
{
	Controll();
	Move();
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::Start()
{
	velocity = Vector2(0.0f, 0.0f);
	speed = 10.0f;
}

void ButiEngine::Player::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::Controll()
{
	velocity = Vector2(0.0f, 0.0f);

	if (GameDevice::GetInput()->CheckKey(Keys::D))
	{
		velocity.x += 1.0f;
	}
	if (GameDevice::GetInput()->CheckKey(Keys::A))
	{
		velocity.x -= 1.0f;
	}
	if (GameDevice::GetInput()->CheckKey(Keys::W))
	{
		velocity.y += 1.0f;
	}
	if (GameDevice::GetInput()->CheckKey(Keys::S))
	{
		velocity.y -= 1.0f;
	}

	velocity.Normalize();
}

void ButiEngine::Player::Move()
{
	velocity *= speed;

	auto transform = gameObject.lock()->transform;

	transform->TranslateX(velocity.x);
	transform->TranslateY(velocity.y);

	OnOutScreen();
}

void ButiEngine::Player::OnOutScreen()
{
	bool outScreen = false;
	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	Vector2 sizeHalf = size / 2.0f;
	Vector2 windowSizeHalf = Vector2(GameSettings::windowWidth / 2.0f, GameSettings::windowHeight / 2.0f);

	if (position.x - sizeHalf.x <= -windowSizeHalf.x)
	{
		position.x = -windowSizeHalf.x + sizeHalf.x;
		outScreen = true;
	}
	else if (position.x + sizeHalf.x > windowSizeHalf.x)
	{
		position.x = windowSizeHalf.x - sizeHalf.x;
		outScreen = true;
	}
	if (position.y - sizeHalf.y <= -windowSizeHalf.y)
	{
		position.y = -windowSizeHalf.y + sizeHalf.y;
		outScreen = true;
	}
	else if (position.y + sizeHalf.y > windowSizeHalf.y)
	{
		position.y = windowSizeHalf.y - sizeHalf.y;
		outScreen = true;
	}

	if (outScreen)
	{
		gameObject.lock()->transform->SetWorldPosition(Vector3(position.x, position.y, 0.0f));
	}
}
