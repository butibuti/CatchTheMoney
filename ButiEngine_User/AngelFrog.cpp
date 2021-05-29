#include "stdafx_u.h"
#include "AngelFrog.h"

void ButiEngine::AngelFrog::OnUpdate()
{
	//if (life > 0)
	//{
	//	life--;
	//}
	//else
	//{
	//	//ƒ‰ƒCƒt‚ª‚È‚­‚È‚Á‚½‚ç”ò‚Î‚·
	//	gameObject.lock()->transform->SetLocalPosition(Vector3(0, -3000, 0));
	//	return;
	//}
	
	Move();

}

void ButiEngine::AngelFrog::OnSet()
{
}

void ButiEngine::AngelFrog::Start()
{
	life = 120;
	accel = 1.0f;
	position = gameObject.lock()->transform->GetLocalPosition();
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
	if (accel > 0)
	{
		accel -= 0.02f;
	}
	else
	{
		accel = 0;
	}
	if (gameObject.lock()->transform->GetLocalScale().y > 0)
	{
		position.y += accel;
	}
	else
	{
		position.y -= accel;
	}

	gameObject.lock()->transform->SetLocalPosition(position);
}
