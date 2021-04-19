#include "stdafx_u.h"
#include "Player.h"
#include"GameSettings.h"
#include"Block.h"
#include"PauseManager.h"
#include"MapChipCollider.h"

void ButiEngine::Player::OnUpdate()
{
	if (shp_pauseManager->GetPause())
	{
		return;
	}
	Controll();
	Move();
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::Start()
{
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	wkp_screenScroll = GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<LightVariable>("LightBuffer");

	shp_mapChipCollider = gameObject.lock()->GetGameComponent<MapChipCollider>();

	velocity = Vector2(0.0f, 0.0f);
	speed = 1.0f;
	jump = true;
	gravity = 0.6f;

	gameObject.lock()->RegistReactionComponent(GetThis<GameComponent>());
}

void ButiEngine::Player::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Player::OnCollision(std::weak_ptr<GameObject> arg_other)
{
	if (abs(velocity.x) > abs(velocity.y))
	{
		BackX(arg_other);
		BackY(arg_other);
	}
	else
	{
		BackY(arg_other);
		BackX(arg_other);
	}
}

void ButiEngine::Player::OnCollisionEnd(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Player::OnShowUI()
{
	GUI::SliderFloat("gravity", &gravity, 0.0f, 1.0f);
	GUI::SliderFloat("speed", &speed, 0.0f, 50.0f);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::Controll()
{
	velocity.x = 0.0f;
	if (GameDevice::GetInput()->CheckKey(Keys::D))
	{
		velocity.x = 1.0f;
	}
	else if (GameDevice::GetInput()->CheckKey(Keys::A))
	{
		velocity.x = -1.0f;
	}
	if (GameDevice::GetInput()->TriggerKey(Keys::Space) && !jump)
	{
		velocity.y = -10.0f;
		jump = true;
	}
	if (jump)
	{
		velocity.y += gravity;
	}

	//velocity.Normalize();
}

void ButiEngine::Player::Move()
{
	velocity *= speed;

	auto transform = gameObject.lock()->transform;

	transform->TranslateX(velocity.x);
	transform->TranslateY(velocity.y);

	OnOutScreen();


	Vector3 position = gameObject.lock()->transform->GetWorldPosition();

	auto scroll =( position.x / GameSettings::windowWidth)- 0.0675;
	float dist = (scroll - wkp_screenScroll.lock()->Get().lightDir.x);
	wkp_screenScroll.lock()->Get().lightDir.x = scroll;
	//wkp_screenScroll.lock()->Get().lightDir.x +=abs( dist)*dist ;
}

void ButiEngine::Player::OnOutScreen()
{
	bool outScreen = false;
	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	float tmp = GameSettings::blockSize / 2.0f;
	Vector2 sizeHalf = Vector2(tmp, tmp);

	/*if (position.x +- sizeHalf.x < -GameSettings::windowWidth / 2)
	{
		position.x =- GameSettings::windowWidth / 2 + sizeHalf.x;
		outScreen = true;
	}
	else if (position.x + sizeHalf.x > GameSettings::windowWidth/2)
	{
		position.x = GameSettings::windowWidth/2 - sizeHalf.x;
		outScreen = true;
	}*/
	if (position.y - sizeHalf.y < -GameSettings::windowHeight/2)
	{
		position.y = -GameSettings::windowHeight/2 + sizeHalf.y + 0.1f;
		outScreen = true;
		velocity.y = 0;
	}
	else if (position.y + sizeHalf.y > GameSettings::windowHeight / 2)
	{
		position.y = -sizeHalf.y+ GameSettings::windowHeight / 2;
		outScreen = true;
		jump = false;
		velocity.y = 0;
	}

	if (outScreen)
	{
		gameObject.lock()->transform->SetWorldPosition(Vector3(position.x, position.y, -0.1f));
	}
}

void ButiEngine::Player::BackX(std::weak_ptr<GameObject> arg_other)
{
	auto otherAABB = arg_other.lock()->GetGameComponent<Collision::ColliderComponent>()->GetCollisionPrimitive();
	if (shp_mapChipCollider->IsHitRight(otherAABB))
	{
		float backLength = abs(arg_other.lock()->transform->GetWorldPosition().x - GameSettings::blockSize - gameObject.lock()->transform->GetWorldPosition().x) + 0.1f;
		gameObject.lock()->transform->TranslateX(-backLength);
	}
	else if (shp_mapChipCollider->IsHitLeft(otherAABB))
	{
		float backLength = abs(arg_other.lock()->transform->GetWorldPosition().x - GameSettings::blockSize - gameObject.lock()->transform->GetWorldPosition().x) + 0.1f;
		gameObject.lock()->transform->TranslateX(backLength);
	}
}

void ButiEngine::Player::BackY(std::weak_ptr<GameObject> arg_other)
{
	auto otherAABB = arg_other.lock()->GetGameComponent<Collision::ColliderComponent>()->GetCollisionPrimitive();
	if (shp_mapChipCollider->IsHitTop(otherAABB))
	{
		float backLength = abs(arg_other.lock()->transform->GetWorldPosition().y - GameSettings::blockSize - gameObject.lock()->transform->GetWorldPosition().y) + 0.1f;
		gameObject.lock()->transform->TranslateY(-backLength);
	}
	else if (shp_mapChipCollider->IsHitBottom(otherAABB))
	{
		float backLength = abs(arg_other.lock()->transform->GetWorldPosition().y - GameSettings::blockSize - gameObject.lock()->transform->GetWorldPosition().y) + 0.1f;
		gameObject.lock()->transform->TranslateY(backLength);
	}
}
