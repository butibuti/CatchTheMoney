#include "stdafx_u.h"
#include "Player.h"
#include"GameSettings.h"
#include"Block.h"
#include"PauseManager.h"

void ButiEngine::Player::OnUpdate()
{
	if (shp_pauseManager->GetPause())
	{
		return;
	}
	Controll();
	Move();
	CreateBlock();
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
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
	else if (GameDevice::GetInput()->CheckKey(Keys::A))
	{
		velocity.x -= 1.0f;
	}
	if (GameDevice::GetInput()->CheckKey(Keys::W))
	{
		velocity.y += 1.0f;
	}
	else if (GameDevice::GetInput()->CheckKey(Keys::S))
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

	if (position.x - sizeHalf.x < 0)
	{
		position.x = sizeHalf.x;
		outScreen = true;
	}
	else if (position.x + sizeHalf.x > GameSettings::windowWidth)
	{
		position.x = GameSettings::windowWidth - sizeHalf.x;
		outScreen = true;
	}
	if (position.y - sizeHalf.y < -GameSettings::windowHeight)
	{
		position.y = -GameSettings::windowHeight + sizeHalf.y;
		outScreen = true;
	}
	else if (position.y + sizeHalf.y > 0)
	{
		position.y = -sizeHalf.y;
		outScreen = true;
	}

	if (outScreen)
	{
		gameObject.lock()->transform->SetWorldPosition(Vector3(position.x, position.y, 0.0f));
	}
}

void ButiEngine::Player::CreateBlock()
{
	if (GameDevice::GetInput()->CheckKey(Keys::Space))
	{
		if (!startCreateBlock)
		{
			wkp_block = GetManager().lock()->AddObjectFromCereal("Block");
			wkp_block.lock()->transform->SetLocalScale(Vector3::Zero);

			auto block = wkp_block.lock()->GetGameComponent<Block>();
			Vector3 position = gameObject.lock()->transform->GetWorldPosition();
			Vector2 startPoint = Vector2(position.x, position.y);
			block->SetStartPoint(startPoint);
			startCreateBlock = true;
		}
	}
	else if (startCreateBlock)
	{
		auto block = wkp_block.lock()->GetGameComponent<Block>();
		block->FinishCreate();
		wkp_block = std::weak_ptr<GameObject>();
		startCreateBlock = false;
	}

	if (wkp_block.lock() && startCreateBlock)
	{
		auto block = wkp_block.lock()->GetGameComponent<Block>();
		Vector3 position = gameObject.lock()->transform->GetWorldPosition();
		Vector2 endPoint = Vector2(position.x, position.y);
		block->SetEndPoint(endPoint);
	}
}
