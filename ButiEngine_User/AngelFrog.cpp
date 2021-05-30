#include "stdafx_u.h"
#include "AngelFrog.h"

void ButiEngine::AngelFrog::OnUpdate()
{
	if (life > 0)
	{
		life--;
	}
	else
	{
		//ライフがなくなったら飛ばす
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, -3000, 0));
		return;
	}
	
	Move();

}

void ButiEngine::AngelFrog::OnSet()
{
}

void ButiEngine::AngelFrog::Start()
{
	life = 180;
	accel = 1.0f;
	velocity = Vector3::Zero;
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::AngelFrog::Clone()
{
	return ObjectFactory::Create<AngelFrog>();
}

void ButiEngine::AngelFrog::SetDefaultGravity(bool arg_top)
{
}

void ButiEngine::AngelFrog::SetUp(float arg_scaleY)
{
	//gameObject.lock()->transform->SetLocalScale()
}

void ButiEngine::AngelFrog::Move()
{
	/*if (accel > 0)
	{
		accel -= 0.01f;
	}
	else
	{
		accel = 0;
	}*/
	velocity = Vector3::Zero;
	if (gameObject.lock()->transform->GetLocalScale().y > 0)
	{
		velocity.y += 1.0f;
	}
	else
	{
		velocity.y -= 1.0f;
	}

	gameObject.lock()->transform->Translate(velocity);
}
