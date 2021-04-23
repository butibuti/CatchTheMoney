#include "stdafx_u.h"
#include "Player.h"
#include"GameSettings.h"
#include"PauseManager.h"
#include"MobiusLoop.h"

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
	shp_mobiusLoop = gameObject.lock()->GetGameComponent<MobiusLoop>();
	shp_AABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.999f, 0.999f, 1.0f), gameObject.lock()->transform);
	wkp_screenScroll = GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<LightVariable>("LightBuffer");

	velocity = Vector3::Zero;
	speed = 4.0f;
	grounded = false;
	gravity = -0.6f;
	jumpForce = 10.0f;

	wkp_bottom = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(), "Player_Bottom");
	wkp_bottom.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	wkp_bottom.lock()->transform->SetLocalPosition(Vector3(0.0f, -24.0f / GameSettings::blockSize, 0.0f));
	wkp_bottom.lock()->transform->SetLocalScale(Vector3(1.0f, 0.5f, 1.0f));
	
	shp_bottomAABB = ObjectFactory::Create<Collision::CollisionPrimitive_Box_AABB>(Vector3(0.999f, 1.0f, 1.0f), wkp_bottom.lock()->transform);
	

	gameObject.lock()->RegistReactionComponent(GetThis<GameComponent>());
}

void ButiEngine::Player::OnCollisionEnter(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Player::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Player::OnCollisionEnd(std::weak_ptr<GameObject> arg_other)
{
}

void ButiEngine::Player::OnShowUI()
{
	GUI::SliderFloat("gravity", &gravity, 0.0f, 1.0f);
	GUI::SliderFloat("jump", &jumpForce, -20.0f, 20.0f);
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

	if (grounded)
	{
		if (GameDevice::GetInput()->TriggerKey(Keys::W))
		{
			velocity.y = jumpForce;
			grounded = false;
		}
	}
	else
	{
		velocity.y += gravity;
	}
}

void ButiEngine::Player::Move()
{
	velocity.x *= speed;

	if (fabsf(velocity.x) > fabsf(velocity.y))
	{
		MoveX();
		MoveY();
	}
	else
	{
		MoveY();
		MoveX();
	}

	shp_AABB->Update();
	shp_bottomAABB->Update();

	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_bottomAABB, 0);
	if (hitObjects.size() == 0)
	{
		grounded = false;
	}

	OnOutScreen();


	//Vector3 position = gameObject.lock()->transform->GetWorldPosition();

	//auto scroll =( position.x / GameSettings::windowWidth)- 0.0675;
	//float dist = (scroll - wkp_screenScroll.lock()->Get().lightDir.x);
	//wkp_screenScroll.lock()->Get().lightDir.x = scroll;
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
		grounded = true;
		velocity.y = 0;
	}

	if (outScreen)
	{
		gameObject.lock()->transform->SetWorldPosition(Vector3(position.x, position.y, -0.1f));
	}
}

void ButiEngine::Player::MoveX()
{
	//if (velocity.x == 0) { return; }
	gameObject.lock()->transform->TranslateX(velocity.x);
	shp_AABB->Update();
	shp_bottomAABB->Update();
	shp_mobiusLoop->UpdateAABB();
	shp_mobiusLoop->BackXRight(velocity);
	shp_AABB->Update();
	shp_mobiusLoop->BackXLeft(velocity);
	shp_AABB->Update();
	BackX();
}

void ButiEngine::Player::MoveY()
{
	//if (velocity.y == 0) { return; }
	gameObject.lock()->transform->TranslateY(velocity.y);
	shp_AABB->Update();
	shp_bottomAABB->Update();
	shp_mobiusLoop->UpdateAABB();
	shp_mobiusLoop->BackYRight(velocity);
	shp_AABB->Update();
	shp_mobiusLoop->BackYLeft(velocity);
	shp_AABB->Update();
	BackY();
}

void ButiEngine::Player::BackX()
{
	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_AABB, 0);

	if (hitObjects.size() != 0)
	{
		auto end = hitObjects.end();
		for (auto itr = hitObjects.begin(); itr != end; ++itr)
		{
			if ((*itr) == gameObject.lock()) { continue; }

			float widthHalf = (*itr)->transform->GetWorldScale().x * 0.5f;
			if (velocity.x > 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x - widthHalf - GameSettings::blockSize * 0.5f - gameObject.lock()->transform->GetWorldPosition().x;
				gameObject.lock()->transform->TranslateX(backLength);
				shp_AABB->Update();
				shp_bottomAABB->Update();
			}
			else if (velocity.x < 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().x + widthHalf + GameSettings::blockSize * 0.5f - gameObject.lock()->transform->GetWorldPosition().x;
				gameObject.lock()->transform->TranslateX(backLength);
				shp_AABB->Update();
				shp_bottomAABB->Update();
			}
		}
		velocity.x = 0;
	}
}

void ButiEngine::Player::BackY()
{
	auto hitObjects = GetCollisionManager().lock()->GetWillHitObjects(shp_AABB, 0);

	if (hitObjects.size() != 0)
	{
		auto end = hitObjects.end();
		for (auto itr = hitObjects.begin(); itr != end; ++itr)
		{
			if ((*itr) == gameObject.lock()) { continue; }

			if (velocity.y > 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().y - GameSettings::blockSize - gameObject.lock()->transform->GetWorldPosition().y;
				gameObject.lock()->transform->TranslateY(backLength);
				shp_AABB->Update();
				shp_bottomAABB->Update();
			}
			else if (velocity.y < 0)
			{
				float backLength = (*itr)->transform->GetWorldPosition().y + GameSettings::blockSize - gameObject.lock()->transform->GetWorldPosition().y;
				gameObject.lock()->transform->TranslateY(backLength);
				shp_AABB->Update();
				shp_bottomAABB->Update();
				grounded = true;
			}
		}
		velocity.y = 0;
	}
}
