//======================================================
//
//プレイヤー処理[player.cpp]
//Author:石原颯馬
//
//======================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "input.h"
#include "camera.h"
#include "objectX.h"
#include "pict.h"
#include "building.h"

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
		//カーソルを元の位置に戻す
		POINT setCursorpos;
		setCursorpos.x = SCREEN_WIDTH / 2;
		setCursorpos.y = SCREEN_HEIGHT / 2;
		ClientToScreen(FindWindowA(CLASS_NAME, nullptr), &setCursorpos);
		SetCursorPos(setCursorpos.x, setCursorpos.y);
	}
	if (pMouse->GetPress(MOUSE_CLICK_LEFT) == true)
	{//位置特定
		Select();
	}
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

	rot.y -= pMouse->GetMove().x * CAMERA_MOU_ROT_SPEED;
	rot.x -= pMouse->GetMove().y * CAMERA_MOU_ROT_SPEED;

	pCamera->SetCameraRot(rot);
}

//=================================
//クリックした座標からワールド座標に変換
//=================================
D3DXVECTOR3 CPlayer::ConvertClickPosToWorld(float fZ)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得
	CCamera* pCamera = CManager::GetCamera();
	CInputMouse* pMouse = CManager::GetInputMouse();					//マウス取得

	D3DXMATRIX mtxView, mtxProj;
	D3DXMATRIX mtxViewPort;
	D3DXMATRIX mtx;
	D3DXVECTOR3 posClick = pMouse->GetPos();
	posClick.z = fZ;	//Z座標指定
	D3DXVECTOR3 posWorld;

	//ビューマトリ取得・逆行列化
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	D3DXMatrixInverse(&mtxView, NULL, &mtxView);

	//プロジェクションマトリ取得・逆行列化
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);
	D3DXMatrixInverse(&mtxProj, NULL, &mtxProj);

	//ビューポートマトリ設定・逆行列化
	D3DXMatrixIdentity(&mtxViewPort);
	mtxViewPort._11 = SCREEN_WIDTH * 0.5;
	mtxViewPort._22 = -SCREEN_HEIGHT * 0.5;
	mtxViewPort._41 = SCREEN_WIDTH * 0.5;
	mtxViewPort._42 = SCREEN_HEIGHT * 0.5;
	D3DXMatrixInverse(&mtxViewPort, NULL, &mtxViewPort);

	//全部掛ける
	mtx = mtxViewPort * mtxProj * mtxView;	//内部でD3DXMatrixMultiplyやってるみたい

	//これでワールド座標に変換できた
	D3DXVec3TransformCoord(&posWorld, &posClick, &mtx);

	return posWorld;
}

//=================================
//選択
//=================================
void CPlayer::Select(void)
{
	//オブジェクト選択（0.0～1.0）
	D3DXVECTOR3 posNear = ConvertClickPosToWorld(0.0f);
	D3DXVECTOR3 posFar = ConvertClickPosToWorld(1.0f);
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);	//オブジェクト取得

		if (pBuilding != NULL)	//ヌルチェ
		{//なんかある
			if (CObjectX::GetModel(pBuilding->GetModelIdx())->m_collision.CollisionCheck(posNear, posFar, pBuilding->GetPos(), pBuilding->GetRot()) == true)
			{//いったん消去
				CPict* pPict = CPict::GetPict(0);	//オブジェクト取得
				pPict->SetTarget(pBuilding);
				return;
			}
		}
	}

	////ピクトさん移動処理（0.0～0.5）
	//posNear = ConvertClickPosToWorld(0.0f);
	//posFar = ConvertClickPosToWorld(0.2f);
	//D3DXVECTOR3 posPlane = VEC3_ZERO;
	//D3DXVECTOR3 vecNorPlane = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXVECTOR3 posSelect;
	//D3DXPLANE plane;

	//D3DXPlaneFromPointNormal(&plane, &posPlane, &vecNorPlane);
	//D3DXPlaneIntersectLine(&posSelect, &plane, &posNear, &posFar);

	//if (D3DXVec3Length(&posSelect) <= PICT_MOVE_LENGTH)
	//{
	//	CPict* pPict = CPict::GetPict(0);	//オブジェクト取得

	//	if (pPict != NULL)	//ヌルチェ
	//	{//ピクトさん
	//		pPict->SetTargetPos(posSelect);
	//	}
	//}
}
