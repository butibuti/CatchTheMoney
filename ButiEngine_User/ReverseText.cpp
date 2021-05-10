#include "stdafx_u.h"
#include "ReverseText.h"

void ButiEngine::ReverseText::OnUpdate()
{
	Animation();
}

void ButiEngine::ReverseText::OnSet()
{
}

void ButiEngine::ReverseText::Start()
{
	play = false;
	x = 0.0f;
	y = 0.0f;
	progress = 0.0f;
	progressFrame = 0;
}

void ButiEngine::ReverseText::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ReverseText::Clone()
{
	return ObjectFactory::Create<ReverseText>();
}

void ButiEngine::ReverseText::Animation()
{
	progressFrame++;
	if (progressFrame > SHOW_FRAME)
	{
		gameObject.lock()->transform->SetLocalScale(Vector3(1.0f, 0.0f, 1.0f));
		return;
	}

	if (!play) { return; }

	progress += 0.03f;
	if (progress > 1.0f)
	{
		progress = 1.0f;
		play = false;
	}

	//x = 1.0f + Easing::Parabola(progress) * 0.5f;
	y = Easing::EaseOutElastic(progress);
	x = 2.0f - y;
	if (x > 1.0f)
	{
		x = 1.0f;
	}

	gameObject.lock()->transform->SetLocalScale(Vector3(x, y, 1.0f));

}
