//======================================================
//
//プレイヤー処理[player.cpp]
//Author:石原颯馬
//
//======================================================
#include "player.h"
#include "manager.h"
#include "game.h"
#include "tutorial.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "objectX.h"
#include "picto.h"
#include "building.h"
#include "button.h"
#include "slider.h"
#include "building.h"
#include "xmodel.h"
#include "havenum.h"
#include "sound.h"
//#include "progressbar.h"

//=================================
//コンストラクタ
//=================================
CPlayer::CPlayer()
{
	m_bControllPicto = false;

	m_cursorPos = CManager::VEC3_ZERO;
	m_pButtonATK = nullptr;
	m_pProgBar = nullptr;
}

//=================================
//デストラクタ
//=================================
CPlayer::~CPlayer()
{}

//=================================
//初期化
//=================================
HRESULT CPlayer::Init(void)
{
	m_bControllPicto = false;

	return S_OK;
}

//=================================
//終了
//=================================
void CPlayer::Uninit(void)
{
	
}

//=================================
//更新
//=================================
void CPlayer::Update(void)
{
	CInputMouse* pMouse = CManager::GetInputMouse();	//マウス取得
	CSound* pSound = CManager::GetSound();
	Move();

	//体力バー設定
	if (m_pObject != nullptr)
	{

		//建物かピクトか知る
		if (m_pObject->GetType() == CObject::TYPE_BUILDING)
		{//建物
			for (int cnt = 0; cnt < MAX_OBJ; cnt++)
			{
				CBuilding* pBuilding = CBuilding::GetBuilding(cnt);

				if (m_pObject == pBuilding)
				{
					int nEndurance;
					int nMaxEndurance;
					if (pBuilding->GetUnique() == false)
					{//計算算出
						nEndurance = pBuilding->GetEndurance();
						nMaxEndurance = HAVE_LIFE(pBuilding->GetLv());
					}
					else
					{//個別
						nMaxEndurance = pBuilding->GetSigEndurance() * pow(10, pBuilding->GetPowEndurance());
						nEndurance = pBuilding->GetEndurance();
					}
					//m_pProgBar->SetPercent((float)nEndurance / nMaxEndurance);
				}
			}
		}
		else
		{//自動的にピクトとわかる
			for (int cnt = 0; cnt < MAX_OBJ; cnt++)
			{
				CPicto* pPicto = CPicto::GetPicto(cnt);

				if (m_pObject == pPicto)
				{
					int nMaxLife;

					//ピクト種類確認
					switch (pPicto->GetType())
					{
					case CPicto::TYPE_DESTROYER:
						nMaxLife = HAVE_LIFE(CPictoDestroyer::GetLv());
						break;
					case CPicto::TYPE_BLOCKER:
						nMaxLife = HAVE_LIFE(CPictoBlocker::GetLv());
						break;
					case CPicto::TYPE_POLICE:
						//どの警察か調べる
						for (int cnt = 0; cnt < MAX_OBJ; cnt++)
						{
							CPictoPolice* pPolice = CPictoPolice::GetPicto(cnt);

							if (pPicto == pPolice)
							{
								nMaxLife = HAVE_LIFE(pPolice->GetLv());
							}
						}
						break;
					case CPicto::TYPE_TAXI:
						nMaxLife = PICTO_TAXI_MAXLIFE;
					}

					//m_pProgBar->SetPercent(((float)pPicto->GetLife() / nMaxLife));
				}
			}
		}
	}

	if (pMouse->GetPress(MOUSE_CLICK_RIGHT) == true)
	{//回転
		Rotate();
	}
	else
	{//マウスカーソル位置保存
		m_cursorPos = pMouse->GetPos();
	}
	//ボタンが押されたか検知
	if (m_pButtonATK != nullptr && m_pButtonATK->IsClickTrigger() == true)
	{
		Attack();
		pSound->Play(CSound::SOUND_LABEL_SE_DISPATCH);
		m_pButtonATK->Uninit();
		m_pButtonATK = nullptr;
	}
	else if (pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
	{//位置特定
		Select();
		if (m_pObject != nullptr)
		{//なんか選択した
			pSound->Play(CSound::SOUND_LABEL_SE_SELECT);
		}
	}

	//スライダーとタクシー挙動
	int nIdxSlider = -1;
	CPictoTaxi* pTaxi = nullptr;

	//スライダー変更
	CSlider* slider = CManager::GetScene()->GetSlider();
	slider->SetSelectIdx(slider->GetSelectIdx() - (pMouse->GetWheel() / 120));

	//タクシーモード
	pTaxi = CPictoTaxi::GetPicto(0);
	nIdxSlider = CManager::GetScene()->GetSlider()->GetSelectIdx();

	//動けって言ってんのにタクシーいない
	if (nIdxSlider != CPictoTaxi::MODE_SABO && pTaxi == nullptr)
	{//いったん表出す
		CPictoTaxi::Create(CPicto::GetAgitPos());
	}

	if (pTaxi != nullptr)
	{//タクシーいる
		pTaxi->SetMode((CPictoTaxi::MODE)nIdxSlider);
	}
}

//=================================
//攻撃
//=================================
void CPlayer::Attack(void)
{
	if (m_pObject != nullptr)
	{//何かしら選択している
		//現在のモードに応じて所持数オブジェクト取得
		CScene::MODE mode = CManager::GetMode();
		CHaveNum** ppHaveNumObj = nullptr;
		if (mode == CScene::MODE_GAME)
		{//ゲーム
			ppHaveNumObj = CGame::GetHaveNumObj();
		}
		else if (mode == CScene::MODE_TUTORIAL)
		{//チュートリアル
			ppHaveNumObj = CTutorial::GetHaveNumObj();
		}

		//オブジェクト取得
		CObject::TYPE type = m_pObject->GetType();

		if (type == CObject::TYPE_BUILDING && ppHaveNumObj[CPicto::TYPE_DESTROYER]->GetHaveNum() > 0)
		{//建物が選択されているかつ一人以上いる
			CPictoDestroyer* picto = CPictoDestroyer::Create(CPicto::GetAgitPos());
			picto->SetTargetObj(m_pObject);
			picto->SetState(CPicto::STATE_FACE);
		}
		else if (type == CObject::TYPE_PICTO)
		{//ピクト（なんでも）が選択されている
			 //調べる
			for (int cnt = 0; cnt < MAX_OBJ; cnt++)
			{//全オブジェクト見る
				CPicto* pPicto = CPicto::GetPicto(cnt);	//ピクト全体取得
				if (m_pObject == pPicto)
				{
					switch (pPicto->GetType())
					{
					case CPicto::TYPE_POLICE:	//警察
						if (ppHaveNumObj[CPicto::TYPE_BLOCKER]->GetHaveNum() > 0)
						{//一人以上いる
							CPictoBlocker::Create(CPicto::GetAgitPos())->SetTargetObj(pPicto);	//ブロッカーを向かわせる
						}
						break;

					case CPicto::TYPE_DESTROYER:	//デストロイヤー
						pPicto->UnsetTarget();
						break;

					case CPicto::TYPE_BLOCKER:	//ブロッカー
						pPicto->UnsetTarget();
						break;
					}
				}
			}
		}
	}
}

//=================================
//収容
//=================================
void CPlayer::AddPicto(const int nDestroyer, const int nBlocker, const int nNormal)
{//追加
	CScene::MODE mode = CManager::GetMode();

	if (mode == CScene::MODE_GAME)
	{//ゲーム
		CHaveNum** ppHaveNum = CGame::GetHaveNumObj();
		ppHaveNum[0]->AddNum(nDestroyer);
		ppHaveNum[1]->AddNum(nBlocker);
		ppHaveNum[2]->AddNum(nNormal);
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//チュートリアル
		CHaveNum** ppHaveNum = CTutorial::GetHaveNumObj();
		ppHaveNum[0]->AddNum(nDestroyer);
		ppHaveNum[1]->AddNum(nBlocker);
		ppHaveNum[2]->AddNum(nNormal);
	}
}

//=================================
//移動
//=================================
void CPlayer::Move(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//キーボード取得
	CCamera* pCamera = CManager::GetCamera();					//カメラ取得
	D3DXVECTOR3 move = CManager::VEC3_ZERO;
	D3DXVECTOR3 rot = pCamera->GetRot();
	//移動
	if (pKeyboard->GetPress(DIK_A) == true)
	{
		move.x += -cosf(rot.y) * CAMERA_MOVE_SPEED;
		move.z += -sinf(rot.y) * CAMERA_MOVE_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{
		move.x += cosf(rot.y) * CAMERA_MOVE_SPEED;
		move.z += sinf(rot.y) * CAMERA_MOVE_SPEED;
	}

	if (pKeyboard->GetPress(DIK_W) == true)
	{
		move.x += -sinf(rot.y) * CAMERA_MOVE_SPEED;
		move.z += cosf(rot.y) * CAMERA_MOVE_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{
		move.x += sinf(rot.y) * CAMERA_MOVE_SPEED;
		move.z += -cosf(rot.y) * CAMERA_MOVE_SPEED;
	}

	//移動
	pCamera->SetPos(move);
}

//=================================
//回転
//=================================
void CPlayer::Rotate(void)
{
	CCamera* pCamera = CManager::GetCamera();
	CInputMouse* pMouse = CManager::GetInputMouse();
	D3DXVECTOR3 rot = CManager::VEC3_ZERO;

	D3DXVECTOR3 move = CManager::VEC3_ZERO;
	move.x = pMouse->GetMove().x;
	move.y = pMouse->GetMove().y;

	rot.y -= move.x * CAMERA_MOU_ROT_SPEED;
	rot.x -= move.y * CAMERA_MOU_ROT_SPEED;

	pCamera->SetRot(rot);

	//カーソルを元の位置に戻す
	POINT setCursorpos;
	setCursorpos.x = m_cursorPos.x;
	setCursorpos.y = m_cursorPos.y;
	ClientToScreen(FindWindowA(CLASS_NAME, nullptr), &setCursorpos);
	SetCursorPos(setCursorpos.x, setCursorpos.y);
}

//=================================
//選択
//=================================
void CPlayer::Select(void)
{
	CInputMouse* mouse = CManager::GetInputMouse();	//マウス取得

	//オブジェクト選択（0.0～1.0）
	D3DXVECTOR3 posNear = mouse->ConvertClickPosToWorld(0.0f);
	D3DXVECTOR3 posFar = mouse->ConvertClickPosToWorld(1.0f);

	//近い何か
	CObject* pObject = nullptr;
	float fLengthNear = CManager::FLOAT_ZERO;

	//ボタン削除
	if (m_pButtonATK != nullptr)
	{
		m_pButtonATK->Uninit();
		m_pButtonATK = nullptr;

		//ゲージも一緒に消す
		//m_pProgBar->Uninit();
		//m_pProgBar = nullptr;
	}

	//建物
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);	//オブジェクト取得

		if (pBuilding != nullptr)	//ヌルチェ
		{//なんかある
			if (pBuilding->GetModel()->GetCollision().CollisionCheck(posNear, posFar, pBuilding->GetPos(), pBuilding->GetRot()) == true &&
				pBuilding->GetEndurance() > 0)
			{//建物選択
				float fLength = D3DXVec3Length(&(pBuilding->GetPos() - posNear));

				if (pObject == nullptr || fLengthNear > fLength)
				{//近い
					pObject = pBuilding;
					fLengthNear = fLength;
				}
			}
		}
	}

	//ピクトさん
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPicto* pPicto = CPicto::GetPicto(cnt);	//オブジェクト取得

		if (pPicto != nullptr)	//ヌルチェ
		{//なんかある
			if (pPicto->GetCollision().CollisionCheck(posNear, posFar, pPicto->GetPos(), pPicto->GetRot()) == true 
				&& pPicto->GetType() != CPicto::TYPE_NORMAL && pPicto->GetType() != CPicto::TYPE_TAXI)
			{//ピクト選択
				float fLength = D3DXVec3Length(&(pPicto->GetPos() - posNear));

				if (pObject == nullptr || fLengthNear > fLength)
				{//近い
					pObject = pPicto;
					fLengthNear = fLength;
				}
			}
		}
	}

	if (pObject != nullptr)
	{//何かしら選択できた
		m_pObject = pObject;	//覚える

		//ボタン生成
		m_pButtonATK = CButton2D::Create(mouse->GetPos(), CManager::VEC3_ZERO, 40.0f, 40.0f);
		m_pButtonATK->BindTexture(CTexture::PRELOAD_HIRE);
		//m_pProgBar = CProgressBar::Create(D3DXVECTOR3(1100.0f, 700.0f, 0.0f), 200.0f, 30.0f);
		//m_pProgBar->SetPercent(1.0f);	//仮設定
	}
	else
	{//何も選択してない
		m_pObject = nullptr;
	}
}
