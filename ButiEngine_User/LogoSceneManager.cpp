#include "stdafx_u.h"
#include "LogoSceneManager.h"

void ButiEngine::LogoSceneManager::OnUpdate()
{

    if (shp_timer->IsOn()) {
        gameObject.lock()->transform->SetLocalScale(gameObject.lock()->transform->GetLocalScale() * 1.001f);
        logoTime += logoApperanceSpeed;
        shp_logoBuffer->Get().lightDir.w = Easing::Parabola(logoTime);
    }
    if (shp_logoTimer->IsOn()) {

        teamLogoTime += logoApperanceSpeed;
        shp_teamBuffer->Get().lightDir.w = Easing::Parabola(teamLogoTime);
    }

    if (shp_timer->Update()) 
    {shp_timer->Stop();
        shp_logoTimer->Start();
    }
    if (shp_logoTimer->Update()) {
        GetManager().lock()->AddObjectFromCereal("LogoSceneScreenObject", ObjectFactory::Create<Transform>(Vector3(0, 1134, 0.1), Vector3::Zero, Vector3(2112, 1188, 1)));

        shp_logoTimer->Stop();
        shp_transTimer->Start();
    }
    if (shp_transTimer->Update()) {

        auto app = GetManager().lock()->GetApplication().lock();
        app->InitLoadResources();
        app->GetGraphicDevice()->SetClearColor(Vector4((255.0f / 255.0f), (254.0f / 255.0f), (250.0f / 255.0f), 1.0f));
        app->GetSceneManager()->LoadScene("Title");
        app->GetSceneManager()->ChangeScene("Title");
        shp_transTimer->Stop();
    }
}

void ButiEngine::LogoSceneManager::OnSet()
{
}

void ButiEngine::LogoSceneManager::Start()
{
    shp_logoBuffer = gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ObjectInformation>("ObjectInformation");
    shp_teamBuffer = GetManager().lock()->GetGameObject("TeamLogo").lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<ObjectInformation>("ObjectInformation");
    if (!shp_timer) {
        shp_timer = ObjectFactory::Create<RelativeTimer>(60.0f);
    }
    if (!shp_transTimer) {
        shp_transTimer = ObjectFactory::Create<RelativeTimer>(60.0f);
    }
    shp_logoTimer = ObjectFactory::Create<RelativeTimer>(120.0f);


    shp_timer->Start();
}

void ButiEngine::LogoSceneManager::OnShowUI()
{
    if (GUI::TreeNode("Timer")) {
        shp_timer->ShowGUI();
        GUI::TreePop();
    }
    if (GUI::TreeNode("TransitionTimer")) {
        shp_transTimer->ShowGUI();
        GUI::TreePop();
    }
    GUI::BulletText("logoSpeed");
    GUI::DragFloat("##logoapp", logoApperanceSpeed, 0.001f);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::LogoSceneManager::Clone()
{
    auto output = ObjectFactory::Create<LogoSceneManager>();
    if (shp_timer) {
        output->shp_timer = shp_timer->Clone();
    }if (shp_transTimer) {
        output->shp_transTimer = shp_transTimer->Clone();
    }
    return output;
}
