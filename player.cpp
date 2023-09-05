//======================================================
//
//プレイヤー処理[player.cpp]
//Author:石原颯馬
//
//======================================================
#include "player.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "objectX.h"
#include "pict.h"
#include "building.h"
#include "button.h"
#include "slider.h"
#include "building.h"
#include "xmodel.h"

//=================================
//コンストラクタ
//=================================
CPlayer::CPlayer()
{
	m_bControllPict = false;

	m_nHaveDestroyer = CManager::INT_ZERO;
	m_nHaveBlocker = CManager::INT_ZERO;
	m_nHaveNormal = CManager::INT_ZERO;

	m_cursorPos = CManager::VEC3_ZERO;
	m_pButtonATK = NULL;
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
	m_bControllPict = false;

	//スライダー初期設定
	CGame::GetSlider()->SetSelectIdx(CPictTaxi::MODE_SABO);

	//仮：人数設定
	m_nHaveDestroyer = 1;
	m_nHaveBlocker = 2;
	m_nHaveNormal = 1500;

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
	Move();

	if (pMouse->GetPress(MOUSE_CLICK_RIGHT) == true)
	{//回転
		Rotate();
	}
	else
	{//マウスカーソル位置保存
		m_cursorPos = pMouse->GetPos();
	}
	//ボタンが押されたか検知
	if (m_pButtonATK != NULL && m_pButtonATK->IsClickTrigger() == true)
	{
		Attack();
		m_pButtonATK->Uninit();
		m_pButtonATK = NULL;
	}
	else if (pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
	{//位置特定
		Select();
	}

	CSlider* slider = CGame::GetSlider();
	slider->SetSelectIdx(slider->GetSelectIdx() - (pMouse->GetWheel() / 120));

	//タクシーモード
	CPictTaxi* pTaxi = CPictTaxi::GetPict(0);
	int nIdxSlider = CGame::GetSlider()->GetSelectIdx();

	//動けって言ってんのにタクシーいない
	if (nIdxSlider != CPictTaxi::MODE_SABO && pTaxi == NULL)
	{//いったん表出す
		CPictTaxi::Create(CPict::GetAgitPos());
	}

	if (pTaxi != NULL)
	{//タクシーいる
		pTaxi->SetMode((CPictTaxi::MODE)nIdxSlider);
	}
}

//=================================
//攻撃
//=================================
void CPlayer::Attack(void)
{
	if (m_pSelectBuilding != NULL)
	{//建物が選択されている
		CPictDestroyer* pict = CPictDestroyer::Create(CPict::GetAgitPos());
		pict->SetTargetObj((CObject*)m_pSelectBuilding);
		pict->SetState(CPict::STATE_FACE);
	}
	else if (m_pSelectPict != NULL)
	{//ピクト（なんでも）が選択されている
		//警察調べる
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//全オブジェクト見る
			CPictPolice* pPict = CPictPolice::GetPict(cnt);	//オブジェクト取得

			if (m_pSelectPict == pPict)
			{//選択しているピクトさんと警察リストのポインタが一致
				CPictBlocker::Create(CPict::GetAgitPos())->SetTargetObj(pPict);
			}
		}
	}
}

//=================================
//収容
//=================================
void CPlayer::AddPict(const int nDestroyer, const int nBlocker, const int nNormal)
{//追加
	m_nHaveDestroyer += nDestroyer;
	m_nHaveBlocker += nBlocker;
	m_nHaveNormal += nNormal;
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

	if (m_bControllPict == true)
	{//操縦設定
		CPict::GetPict(0)->Controll(move);
	}

	//移動
	pCamera->SetCameraPos(move);

	//ピクトさん操縦設定
	if (pKeyboard->GetTrigger(DIK_H) == true)
	{
		CPict::GetPict(0)->SetControll();
		m_bControllPict = true;
	}
	else if (pKeyboard->GetTrigger(DIK_J) == true)
	{
		CPict::GetPict(0)->Uncontroll();
		m_bControllPict = false;
	}
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

	pCamera->SetCameraRot(rot);

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

	//ボタン削除
	if (m_pButtonATK != NULL)
	{
		m_pButtonATK->Uninit();
		m_pButtonATK = NULL;
	}

	//建物
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);	//オブジェクト取得

		if (pBuilding != NULL)	//ヌルチェ
		{//なんかある
			if (pBuilding->GetModel()->GetCollision().CollisionCheck(posNear, posFar, pBuilding->GetPos(), pBuilding->GetRot()) == true &&
				pBuilding->GetEndurance() > 0)
			{//建物選択
				m_pSelectPict = NULL;
				m_pSelectBuilding = pBuilding;
				
				//ボタン生成
				m_pButtonATK = CButton2D::Create(mouse->GetPos(), CManager::VEC3_ZERO, 40.0f, 40.0f);
				return;
			}
		}
	}

	//ピクトさん
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPict* pPict = CPict::GetPict(cnt);	//オブジェクト取得

		if (pPict != NULL)	//ヌルチェ
		{//なんかある
			if (pPict->GetCollision().CollisionCheck(posNear, posFar, pPict->GetPos(), pPict->GetRot()) == true)
			{//ピクト選択
				m_pSelectBuilding = NULL;
				m_pSelectPict = pPict;

				//ボタン生成
				m_pButtonATK = CButton2D::Create(mouse->GetPos(), CManager::VEC3_ZERO, 40.0f, 40.0f);
				return;
			}
		}
	}

	//何も選択していないので選択解除
	m_pSelectBuilding = NULL;
	m_pSelectPict = NULL;
}
