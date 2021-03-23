#include"stdafx_u.h"
#include "TestBehavior.h"
#include"Header/Common/CollisionPrimitive.h"

void ButiEngine::TestBehavior::OnUpdate()
{
	/*static bool isForcus = false;

	if (isForcus) {

		Vector2 mouseMove = GameDevice::GetInput()->GetMouseMove();
		GameDevice::GetInput()->SetMouseCursor(gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->GetApplication().lock()->GetWindow()->GetWindowCenterPosition());
		shp_camera->shp_transform->RollLocalRotationX_Degrees(-mouseMove.y / 3.0f);
		shp_camera->shp_transform->RollWorldRotationY_Degrees(-mouseMove.x / 3.0f);
	}*/
	if (GameDevice::GetInput()->CheckKey(Keys::W)) {
		auto objPos = gameObject.lock()->transform->GetWorldPosition();
		Vector3 rayStart = objPos + gameObject.lock()->transform->GetFront() * 0.20f;
		Line ray = Line((Vector3)(rayStart+gravityVeloc * 0.5) * shp_floorTransform->GetMatrix().Inverse(), Vector3(0, -1, 0) * shp_floorTransform->GetWorldRotation().Inverse());
		bool isRayHit = shp_stageMesh->GetPosRayCast(&rayPos, &rayNormal, ray);

		if (isRayHit) {
			rayPos = rayPos * shp_floorTransform->GetMatrix();
			auto veloc = rayPos - objPos;
			value = gravityVeloc.Dot(veloc);

			if (value < 0.2) {

				gameObject.lock()->transform->Translate((veloc).Normalize() * 0.15f);
				gameObject.lock()->transform->SetLookAtRotation(rayPos, rayNormal);
			}

		}

	}
	if (GameDevice::GetInput()->CheckKey(Keys::S)) {

	}
	if (GameDevice::GetInput()->CheckKey(Keys::A)) {
		gameObject.lock()->transform->RollLocalRotationY_Degrees(-2);

	}
	if (GameDevice::GetInput()->CheckKey(Keys::D)) {
		gameObject.lock()->transform->RollLocalRotationY_Degrees(2);

	}
	if (GameDevice::GetInput()->CheckKey(Keys::Space)) {
		//moveForce -= gameObject.lock()->transform->GetUp();

	}/*
	if (GameDevice::GetInput()->TriggerKey(Keys::Space)) {
		isForcus = !isForcus;

	}*/
	
	if (shp_timer->Update()) {
		shp_timer->Stop();
		//gameObject.lock()->GetGameObjectManager().lock()->GetScene().lock()->GetSceneManager().lock()->ReloadScene();
	}
}


void ButiEngine::TestBehavior::OnSet()
{
}

void ButiEngine::TestBehavior::Start()
{
	shp_timer = ObjectFactory::Create<RelativeTimer>(5);
	shp_timer->Start();

	shp_stageMesh = gameObject.lock()->GetResourceContainer()->GetMesh(MeshTag("Model/Stage/ground.b3m")).lock();
	shp_floorTransform = GetManager().lock()->GetGameObject("floor").lock()->transform;
}

void ButiEngine::TestBehavior::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::TestBehavior::Clone()
{
	return ObjectFactory::Create<TestBehavior>();
}