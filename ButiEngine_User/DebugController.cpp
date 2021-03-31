#include "stdafx_u.h"
#include "DebugController.h"

void ButiEngine::DebugController::OnUpdate()
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

	velocity *= speed;
	gameObject.lock()->transform->Translate(Vector3(velocity.x, velocity.y, 0.0f));
}

void ButiEngine::DebugController::OnSet()
{
	speed = 20.0f;
}

void ButiEngine::DebugController::Start()
{
}

void ButiEngine::DebugController::OnShowUI()
{
	GUI::SliderFloat("##speed", speed, 0.0f, 30.0f);
}

void ButiEngine::DebugController::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::DebugController::Clone()
{
	return ObjectFactory::Create<DebugController>();
}
