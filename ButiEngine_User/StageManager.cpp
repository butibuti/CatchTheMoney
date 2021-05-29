#include"stdafx_u.h"
#include"StageManager.h"
#include"PauseManager.h"
#include"InputManager.h"
#include"PanelManager.h"
#include"ScrollManager.h"
#include"Map.h"
#include"CameraController.h"
#include"StageSelect.h"
#include"Player.h"
#include"ShakeComponent.h"
#include"NumberComponent.h"
#include"SquareParticleEmitter.h"
#include"GameSettings.h"
#include"SceneChangeAnimation.h"
#include"FollowPanel.h"
#include"Panel.h"
#include"ClearButton.h"
#include"TalkText.h"
#include"Daikokuten.h"
#include"ParentDaikokuten.h"
#include"Frog.h"
#include"Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"

//#define OUTPUT_STAGERENDERTARGET
#ifdef DEBUG

static int f = 0;
#endif // DEBUG

ButiEngine::GameMode ButiEngine::StageManager::mode;

void ButiEngine::StageManager::OnUpdate()
{
	if (GameSettings::isTitle) { return; }
	StorePlayer();
	StoreFrog();

	if (wkp_player.lock()->GetGameComponent<Player>()->IsClear())
	{
		if (clearAnimationFrame >= CLEAR_FRAME)
		{
			if (!wkp_player.lock()->GetGameComponent<Player>()->GetHoldSita().lock())
			{
				GetManager().lock()->AddObjectFromCereal("ClearFlash", ObjectFactory::Create<Transform>(Vector3(0.0f, 0.0f, 1000.0f)));
			}
			else
			{
				auto frog = wkp_frog.lock()->GetGameComponent<Frog>();
				frog->Exprosion();
				frog->GetBackFrog().lock()->GetGameComponent<Frog>()->Exprosion();
			}
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_clear, GameSettings::masterVolume);
		}
		else if (clearAnimationFrame == CLEAR_FRAME - 10)
		{
			GetManager().lock()->AddObjectFromCereal("ClearBand");
		}
		else if (clearAnimationFrame == CLEAR_FRAME - 30)
		{
			GetManager().lock()->AddObjectFromCereal("ClearText");
		}
		clearAnimationFrame--;
	}

	FrogEatAnimation();
	OnGoal();
	ModeChange();
	ChangeUIAlpha();

	GetManager().lock()->GetApplication().lock()->GetGUIController()->SetGUIObject(GetThis<StageManager>());

	if (mode == GameMode::Edit) {
		auto swing = shp_scrollManager->GetCurrentScrollSwing()*360;

		shp_particleEmitter->SetRotation(particleScrollOffset+swing);
	}




#ifdef OUTPUT_STAGERENDERTARGET 
#ifdef DEBUG

	if (f==3) {
		const int stageNum = StageSelect::GetStageNum();

		std::string filePath = "Texture/StageSelect/stage_";


		if (stageNum < 10) {
			filePath += "0";
		}

		filePath += std::to_string(stageNum) + ".png";
		TextureTag output = TextureTag(":/wideScreen/1280/144");
		auto texture = GetManager().lock()->GetApplication().lock()->GetResourceContainer()->GetTexture(output).lock();
		if (texture && !GameSettings::isTitle) {
			texture->ToPNG(GlobalSettings::GetResourceDirectory() + filePath);
		}
		f = 0;
	}
	if (f > 0) {
		f++;
	}
#endif // DEBUG
#endif // OUTPUT_STAGERENDERTARGET
}

void ButiEngine::StageManager::OnSet()
{
}

void ButiEngine::StageManager::Start()
{
	fadeCount = 0;
	isNext = false;
	isOnce = false;

	shp_map = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	shp_panelManager = GetManager().lock()->GetGameObject("PanelManager").lock()->GetGameComponent<PanelManager>();
	shp_scrollManager = GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<ScrollManager>();
	shp_cameraController = GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraController>();

	CreateUI();
	if (!GameSettings::isTitle)
	{
		wkp_daikokutenHead = GetManager().lock()->GetGameObject("Daikokuten");
		wkp_daikokutenHandLeft = GetManager().lock()->GetGameObject("DaikokutenHand_L");
		wkp_daikokutenHandRight = GetManager().lock()->GetGameObject("DaikokutenHand_R");

		wkp_daikokutenStay = GetManager().lock()->GetGameObject("Daikokuten_Stay");
		wkp_daikokutenAppear = GetManager().lock()->GetGameObject("Daikokuten_Appear");
		wkp_daikokutenReaction = GetManager().lock()->GetGameObject("Daikokuten_Reaction");
		wkp_daikokutenRHand = GetManager().lock()->GetGameObject("Daikokuten_Hand_R");
		wkp_daikokutenLHand = GetManager().lock()->GetGameObject("Daikokuten_Hand_L");
		wkp_daikokutenRAppear = GetManager().lock()->GetGameObject("Daikokuten_Appear_R");
		wkp_daikokutenLAppear = GetManager().lock()->GetGameObject("Daikokuten_Appear_L");

		wkp_daikokutenAppear.lock()->transform->SetBaseTransform(wkp_daikokutenReaction.lock()->transform);
		wkp_daikokutenStay.lock()->transform->SetBaseTransform(wkp_daikokutenAppear.lock()->transform);
		wkp_daikokutenHead.lock()->transform->SetBaseTransform(wkp_daikokutenStay.lock()->transform);
		wkp_daikokutenRHand.lock()->transform->SetBaseTransform(wkp_daikokutenRAppear.lock()->transform);
		wkp_daikokutenLHand.lock()->transform->SetBaseTransform(wkp_daikokutenLAppear.lock()->transform);
		wkp_daikokutenHandRight.lock()->transform->SetBaseTransform(wkp_daikokutenRHand.lock()->transform);
		wkp_daikokutenHandLeft.lock()->transform->SetBaseTransform(wkp_daikokutenLHand.lock()->transform);


		wkp_stageNumber = GetManager().lock()->AddObjectFromCereal("ParentNumber", ObjectFactory::Create<Transform>(Vector3(-550, 350, 0), Vector3::Zero, Vector3(80, 80, 0)));
		auto numComponent = wkp_stageNumber.lock()->GetGameComponent<NumberComponent>();
		numComponent->SetNumber(StageSelect::GetStageNum() + 1);
	}

	{
		auto stageNum = StageSelect::GetStageNum();
		if (stageNum == TalkStageNum::FIRST_TALK && !GameSettings::isTitle)
		{
			//最初の会話用テキスト追加
			wkp_talkText = GetManager().lock()->AddObjectFromCereal("FirstTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
			CommonTextObject();
		}
		else if(stageNum == TalkStageNum::PANEL_TALK && !GameSettings::isTitle)
		{
			//パネル移動説明用テキスト追加
			wkp_talkText = GetManager().lock()->AddObjectFromCereal("PanelTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
			CommonTextObject();
		}
		else if (stageNum == TalkStageNum::REVERSE_TALK && !GameSettings::isTitle)
		{
			//反転説明用テキスト追加
			wkp_talkText = GetManager().lock()->AddObjectFromCereal("ReverseTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
			CommonTextObject();
		}
		else if (stageNum == TalkStageNum::REVERSE_RE_TALK && !GameSettings::isTitle)
		{
			//反転再警告用テキスト追加
			wkp_talkText = GetManager().lock()->AddObjectFromCereal("ReverseRemindTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
			CommonTextObject();
		}
		else if (stageNum == TalkStageNum::GRAVITY_TALK && !GameSettings::isTitle)
		{
			//重力コア説明用テキスト追加
			wkp_talkText = GetManager().lock()->AddObjectFromCereal("GravityTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
			CommonTextObject();
		}
		else if (stageNum == TalkStageNum::FROG_TALK && !GameSettings::isTitle)
		{
			frogEatAnimationCount = 240;
			TalkText::Revive();
		}
		else if (stageNum == TalkStageNum::LAST_TALK && !GameSettings::isTitle)
		{
			//最後の会話用テキスト追加
			wkp_talkText = GetManager().lock()->AddObjectFromCereal("LastTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
			CommonTextObject();
		}
		else
		{
			//テキスト出さないステージはDelete
			TalkText::Delete();
		}
	}

	wkp_fadeObject = GetManager().lock()->AddObjectFromCereal("FadeObject2", ObjectFactory::Create<Transform>(Vector3(0, 0, -0.7f), Vector3::Zero, Vector3(1920, 1080, 1)));

	shp_map->PutTile();

	clearAnimationFrame = CLEAR_FRAME;

	mode = GameMode::Chara;
	shp_particleEmitter= GetManager().lock()->GetGameObject("SquareParticleEmitter").lock()->GetGameComponent<SquareParticleEmitter>();

	modeUIPosition = Vector3(750.0f, -410.0f, -0.1f);

	clearButtonAnimation = false;
	selectedNext = true;

	for (int i = 0; i < StageSelect::GetMaxStage(); i++)
	{
		std::string stageName = "Stage" + std::to_string(i);
		if (stageName == StageSelect::GetRemoveStageName())
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			sceneManager->RemoveScene(stageName);
		}
	}

	bgm = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/BGM.wav");
	se_clear = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Clear.wav");
	se_enter = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Enter.wav");
	se_select = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Select-Click.wav");
	se_panelMode = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Panel_Pick.wav");
	se_charaMode = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Panel_Drop.wav");
	if (!GameSettings::isTitle)
	{
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlayBGM(bgm, GameSettings::masterVolume);
	}

	const int stageNum = StageSelect::GetStageNum();
	nextSceneName = "Stage" + std::to_string(stageNum + 1);

#ifdef DEBUG
	f = 1;
#endif // DEBUG

}

void ButiEngine::StageManager::ShowGUI()
{
}

void ButiEngine::StageManager::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::StageManager::Clone()
{
	return ObjectFactory::Create<StageManager>();
}

void ButiEngine::StageManager::OnGoal()
{
	if (clearAnimationFrame == 30)
	{
		shp_buttonNext->Appear();
		if (StageSelect::GetStageNum() != StageSelect::GetMaxStage())
		{
			shp_buttonSelect->Appear();
		}
	}
	else if (!clearButtonAnimation && clearAnimationFrame < 0)
	{
		clearButtonAnimation = true;
		shp_buttonNext->OnSelected();
	}
	ClearButtonUpdate();
	if (isNext)
	{
		fadeCount++;
	}
	if (fadeCount == 1)
	{
		GetManager().lock()->AddObjectFromCereal("FadeObject2", ObjectFactory::Create<Transform>(Vector3(0, 1080, -0.7f), Vector3::Zero, Vector3(1920, 1080, 1)));
	}
	if (fadeCount > 30)
	{
		shp_map->DestoryBlock();
		//ChangeScene("StageSelect");
		//int nextStageNum = StageSelect::GetStageNum() + 1;
		//StageSelect::SetStageNum(nextStageNum);
		
		int nextStageNum = StageSelect::GetStageNum() + 1;
		if (nextStageNum > StageSelect::GetMaxStage())
		{
			StageSelect::SetRemoveStageName("none");
			std::string sceneName = "ClearScene";
			ChangeScene(sceneName);
		}
		else
		{
			std::string sceneName = "Stage" + std::to_string(StageSelect::GetStageNum());
			if (nextSceneName == "StageSelect")
			{
				sceneName = "none";
			}
			StageSelect::SetRemoveStageName(sceneName);
			ChangeScene(nextSceneName);
		}

		StageSelect::SetStageNum(nextStageNum);
	}
#ifdef _DEBUG
	if (GameDevice::GetInput()->TriggerKey(Keys::C) && GameDevice::GetInput()->CheckKey(Keys::LeftCtrl))
	{
		StageSelect::SetRemoveStageName("none");
		shp_map->DestoryBlock();
		ChangeScene("StageSelect");
	}
#endif
}

void ButiEngine::StageManager::ChangeScene(const std::string& arg_sceneName)
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	sceneManager->RemoveScene(arg_sceneName);
	sceneManager->LoadScene(arg_sceneName);
	sceneManager->ChangeScene(arg_sceneName);
}

void ButiEngine::StageManager::ModeChange()
{
	if (InputManager::OnTriggerModeChangeKey())
	{
		if (shp_cameraController->IsAnimation()) { return; }
		if (shp_panelManager->IsAnimation()) { return; }
		if (shp_pauseManager->IsPause()) { return; }
		if (StageSelect::GetStageNum() == 0) { return; }
		if (wkp_player.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel().lock()->GetGameComponent<Panel>()->IsLock()) { return; }
		if (wkp_player.lock()->GetGameComponent<Player>()->IsClear()) { return; }
		if (wkp_player.lock()->GetGameComponent<Player>()->IsFreeze()) { return; }
		if (wkp_frog.lock() && wkp_frog.lock()->GetGameComponent<Frog>()->IsAnimation()) { return; }

		shp_scrollManager->ResetScroll();
		
		if (mode == GameMode::Chara)
		{
			mode = GameMode::Edit;

			wkp_edit.lock()->transform->TranslateZ(1000);
			wkp_chara.lock()->transform->SetWorldPosition(modeUIPosition);

			shp_cameraController->ZoomOut();
			shp_panelManager->ResetMoveNum();
			shp_panelManager->ResetMoveHistories();

			shp_particleEmitter->SetIsEdit(true);

			auto nowPosX = wkp_player.lock()->transform->GetWorldPosition();
			particleScrollOffset = nowPosX.x-shp_panelManager->GetClosestPanel(nowPosX.x).lock()->transform->GetWorldPosition().x;

			particleScrollOffset = 360 * (particleScrollOffset / (float)GameSettings::windowWidth);

			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_panelMode, GameSettings::masterVolume * 3.0f);
		
			wkp_daikokutenHead.lock()->GetGameComponent<Daikokuten>()->NormalScale();
			wkp_daikokutenAppear.lock()->GetGameComponent<ParentDaikokuten>()->Appear();
			wkp_daikokutenRAppear.lock()->GetGameComponent<ParentDaikokuten>()->Appear();
			wkp_daikokutenLAppear.lock()->GetGameComponent<ParentDaikokuten>()->Appear();
		}
		else
		{
			mode = GameMode::Chara;
			shp_cameraController->ZoomIn(); 

			shp_particleEmitter->SetIsEdit(false);

			wkp_chara.lock()->transform->TranslateZ(1000);
			wkp_edit.lock()->transform->SetWorldPosition(modeUIPosition);

			shp_cameraController->ZoomIn();

			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_charaMode, GameSettings::masterVolume * 3.0f);

			wkp_daikokutenAppear.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
			wkp_daikokutenRAppear.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
			wkp_daikokutenLAppear.lock()->GetGameComponent<ParentDaikokuten>()->Disappear();
		}
	}
}

void ButiEngine::StageManager::CreateUI()
{
	if (GameSettings::isTitle) return;
	int stageNum = StageSelect::GetStageNum();

	if (stageNum == 0)
	{
		GetManager().lock()->AddObjectFromCereal("Control");
	}
	else
	{
		wkp_x = GetManager().lock()->AddObjectFromCereal("X");
		wkp_edit = GetManager().lock()->AddObjectFromCereal("Edit");
		wkp_chara = GetManager().lock()->AddObjectFromCereal("Chara");

		shp_XMesh = wkp_x.lock()->GetGameComponent<MeshDrawComponent>();
		shp_EditMesh = wkp_edit.lock()->GetGameComponent<MeshDrawComponent>();
		shp_CharaMesh = wkp_chara.lock()->GetGameComponent<MeshDrawComponent>();

		wkp_chara.lock()->transform->TranslateZ(1000);
	}

	wkp_buttonNext = GetManager().lock()->AddObjectFromCereal("Button_Next");
	wkp_buttonSelect = GetManager().lock()->AddObjectFromCereal("Button_Select");
	shp_buttonNext = wkp_buttonNext.lock()->GetGameComponent<ClearButton>();
	shp_buttonSelect = wkp_buttonSelect.lock()->GetGameComponent<ClearButton>();

	wkp_grab = GetManager().lock()->AddObjectFromCereal("BTextWindow");
}

void ButiEngine::StageManager::ChangeUIAlpha()
{
	auto closestPanel = wkp_player.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel().lock();
	if (!closestPanel) { return; }
	bool isLock = closestPanel->GetGameComponent<Panel>()->IsLock();

	float alpha = 1.0f;
	if (isLock)
	{
		alpha = 0.5f;
	}
	if (shp_XMesh && shp_EditMesh && shp_CharaMesh)
	{
		shp_XMesh->GetCBuffer<LightVariable>("LightBuffer")->Get().lightDir.w = alpha;
		shp_EditMesh->GetCBuffer<LightVariable>("LightBuffer")->Get().lightDir.w = alpha;
		shp_CharaMesh->GetCBuffer<LightVariable>("LightBuffer")->Get().lightDir.w = alpha;
	}
}

void ButiEngine::StageManager::ClearButtonUpdate()
{
	if (!clearButtonAnimation) { return; }

	if (StageSelect::GetStageNum() != StageSelect::GetMaxStage() && !isNext)
	{
		if (InputManager::OnTriggerRightKey())
		{
			if (!selectedNext)
			{
				GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);
			}
			selectedNext = true;
			shp_buttonNext->OnSelected();
			shp_buttonSelect->OnEndSelect();
			nextSceneName = "Stage" + std::to_string(StageSelect::GetStageNum() + 1);
		}
		else if (InputManager::OnTriggerLeftKey())
		{
			if (selectedNext)
			{
				GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);
			}
			selectedNext = false;
			shp_buttonNext->OnEndSelect();
			shp_buttonSelect->OnSelected();
			nextSceneName = "StageSelect";
		}
	}

	if (InputManager::OnTriggerDecisionKey() && !isNext)
	{
		isNext = true;
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_enter, GameSettings::masterVolume);
		if (selectedNext)
		{
			wkp_buttonNext.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
		}
		else
		{
			wkp_buttonSelect.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
		}
	}
}

void ButiEngine::StageManager::CommonTextObject()
{
	wkp_textWindow = GetManager().lock()->AddObjectFromCereal("TextWindow", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.12f), Vector3::Zero, Vector3(1920, 640, 1)));
	GetManager().lock()->AddObjectFromCereal("Abutton", ObjectFactory::Create<Transform>(Vector3(790, -380, -0.16f), Vector3::Zero, Vector3(180, 180, 1)));
	GetManager().lock()->AddObjectFromCereal("SkipYbutton", ObjectFactory::Create<Transform>(Vector3(675, -200, -0.16f), Vector3::Zero, Vector3(80, 80, 1)));
	GetManager().lock()->AddObjectFromCereal("SkipText", ObjectFactory::Create<Transform>(Vector3(780, -200, -0.16f), Vector3::Zero, Vector3(160, 80, 1)));
}

void ButiEngine::StageManager::FrogEatAnimation()
{
	if (StageSelect::GetStageNum() != TalkStageNum::FROG_TALK || frogEatAnimationCount <= 0) return;

	frogEatAnimationCount--;
	const int ZOOM_IN_START = 230;
	if (frogEatAnimationCount == ZOOM_IN_START)
	{
		shp_cameraController->FrogZoomIn();
	}
	else if (frogEatAnimationCount == ZOOM_IN_START - 160)
	{
		shp_cameraController->FrogZoomOut();
	}

	if (frogEatAnimationCount == 1)
	{
		//カエル説明用テキスト追加
		wkp_talkText = GetManager().lock()->AddObjectFromCereal("FrogTalkText", ObjectFactory::Create<Transform>(Vector3(0, -310, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		CommonTextObject();
	}
}

void ButiEngine::StageManager::StorePlayer()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
	}
}

void ButiEngine::StageManager::StoreFrog()
{
	if (!wkp_frog.lock())
	{
		wkp_frog = GetManager().lock()->GetGameObject("Frog");
	}
}
