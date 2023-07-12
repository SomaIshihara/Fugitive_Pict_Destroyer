//======================================================
//
//プレイヤー処理[player.cpp]
//Author:石原颯馬
//
//======================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "object.h"
#include "input.h"
#include "camera.h"
#include "objectX.h"
#include "pict.h"
#include "building.h"
#include "slider.h"

#define CAMERA_MOVE_SPEED		(10.0f)		//カメラ移動速度
#define CAMERA_MOU_ROT_SPEED	(0.0012f)	//マウス移動での回転速度
#define PICT_MOVE_LENGTH		(1000.0f)	//ピクトさん移動処理選択半径

//=================================
//コンストラクタ
//=================================
CPlayer::CPlayer()
{
	m_nChacePictNum = -1;
	m_bControllPict = false;

	m_nHaveDestroyer = INT_ZERO;
	m_nHaveBlocker = INT_ZERO;
	m_nHaveNormal = INT_ZERO;

	m_cursorPos = VEC3_ZERO;
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
	m_nChacePictNum = -1;
	m_bControllPict = false;

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

	if (pMouse->GetPress(MOUSE_CLICK_LEFT) == true)
	{//回転
		Rotate();
	}
	else
	{//マウスカーソル位置保存
		m_cursorPos = pMouse->GetPos();
	}
	if (pMouse->GetTrigger(MOUSE_CLICK_LEFT) == true)
	{//位置特定
		Select();
	}
	if (pMouse->GetPress(MOUSE_CLICK_RIGHT) == true)
	{//モード変更
		CSlider* slider = CManager::GetSlider();
		slider->SetSelectIdx(slider->GetSelectIdx() - (pMouse->GetWheel() / 120));
	}

	//タクシーモード
	CPictTaxi* pTaxi = CPictTaxi::GetPict(0);
	int nIdxSlider = CManager::GetSlider()->GetSelectIdx();

	//動けって言ってんのにタクシーいない
	if (nIdxSlider != CPictTaxi::MODE_SABO && pTaxi == NULL)
	{//いったん表出す
		CPictTaxi::Create(CPict::GetAgitPos());
	}

	if (pTaxi != NULL)
	{//タクシーいる
		pTaxi->SetMode((CPictTaxi::MODE)nIdxSlider);
	}

	//デバッグ
	if (m_pSelectBuilding != NULL)
	{
		CManager::GetDebProc()->Print("[BUILDING]\n");
	}
	if (m_pSelectPict != NULL)
	{
		CManager::GetDebProc()->Print("[PICT]\n");
	}
}

//=================================
//攻撃
//=================================
void CPlayer::Attack(void)
{
	if (m_pSelectBuilding != NULL)
	{//建物が選択されている
		//CPictDestroyer::GetPict(0)->SetTarget(m_pSelectBuilding);
		CPictDestroyer* pict = CPictDestroyer::Create(CPict::GetAgitPos());
		pict->SetTarget(m_pSelectBuilding);
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
				CPictBlocker::Create(CPict::GetAgitPos())->SetTarget(pPict);
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
	D3DXVECTOR3 move = VEC3_ZERO;
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
	else if (m_nChacePictNum != -1)
	{//追従設定
		move = CPict::GetPict(m_nChacePictNum)->GetMove();
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
	D3DXVECTOR3 rot = VEC3_ZERO;

	D3DXVECTOR3 move = VEC3_ZERO;
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

	//建物
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);	//オブジェクト取得

		if (pBuilding != NULL)	//ヌルチェ
		{//なんかある
			if (CObjectX::GetModel(pBuilding->GetModelIdx())->m_collision.CollisionCheck(posNear, posFar, pBuilding->GetPos(), pBuilding->GetRot()) == true &&
				pBuilding->GetEndurance() > 0)
			{//建物選択
				m_pSelectPict = NULL;
				m_pSelectBuilding = pBuilding;
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
				return;
			}
		}
	}

	//何も選択していないので選択解除
	m_pSelectBuilding = NULL;
	m_pSelectPict = NULL;
}
