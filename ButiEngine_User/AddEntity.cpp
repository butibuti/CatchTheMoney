#include "stdafx_u.h"
#include "AddEntity.h"

void ButiEngine::AddEntity::OnUpdate()
{
	if (GameDevice::GetInput()->CheckKey(Keys::Space))
	{
		count++;
		if (count % 5 == 0)
		{
			auto entity = GetManager().lock()->AddObjectFromCereal("Entity", ObjectFactory::Create<Transform>(Vector3(0.0f, 6.7f, 0.0f), Vector3::Zero, Vector3(0.6f, 1.0f, 0.6f)));
			auto center = GetManager().lock()->AddObjectFromCereal("Center", ObjectFactory::Create<Transform>(Vector3::Zero, Vector3::Zero, 1.0f));
			entity.lock()->transform->SetBaseTransform(center.lock()->transform);
			center.lock()->transform->SetBaseTransform(GetManager().lock()->GetGameObject("Mobius").lock()->transform);
		}
	}
}

void ButiEngine::AddEntity::OnSet()
{
}

void ButiEngine::AddEntity::Start()
{
	count = 0;
}

void ButiEngine::AddEntity::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::AddEntity::Clone()
{
	return ObjectFactory::Create<AddEntity>();
}
