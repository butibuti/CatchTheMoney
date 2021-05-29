#include "stdafx_u.h"
#include "TextWindow.h"
#include "TalkText.h"
#include "InputManager.h"

void ButiEngine::TextWindow::OnUpdate()
{
	if (TalkText::IsDelete())
	{
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, -3000, 0));
	}
	else
	{
		gameObject.lock()->transform->SetLocalPosition(initPos);
	}


}

void ButiEngine::TextWindow::OnSet()
{
}

void ButiEngine::TextWindow::Start()
{
	initPos = gameObject.lock()->transform->GetLocalPosition();
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::TextWindow::Clone()
{
	return ObjectFactory::Create<TextWindow>();
}
