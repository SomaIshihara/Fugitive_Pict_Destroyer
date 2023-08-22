//======================================================
//
//ゲームシーン[game.cpp]
//Author:石原颯馬
//
//======================================================
#include "game.h"
#include "player.h"
#include "meshField.h"
#include "slider.h"
#include "building.h"
#include "score.h"
#include "timer.h"
#include "objectX.h"
#include "pict.h"
#include "item.h"
#include "meshsky.h"
#include "point.h"
#include "input.h"

//静的メンバ変数
CPlayer* CGame::m_pPlayer = NULL;
CSlider* CGame::m_pSlider = NULL;
CMeshField* CGame::m_pMeshField = NULL;

//=================================
//コンストラクタ
//=================================
CGame::CGame()
{
}

//=================================
//デストラクタ
//=================================
CGame::~CGame()
{
}

//=================================
//初期化
//=================================
HRESULT CGame::Init(void)
{
	m_pSlider = CSlider::Create(D3DXVECTOR3(100.0f, SCREEN_HEIGHT - 20.0f, 0.0f), 40.0f, 40.0f, 3);
	m_pPlayer = new CPlayer;

	//プレイヤー初期化
	if (FAILED(m_pPlayer->Init()))
	{
		return E_FAIL;
	}

	//仮オブジェ生成
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(-1280.0f, 0.0f, 1280.0f), VEC3_ZERO, 64.0f, 64.0f, 40, 40);

	//建物パラメータ読み込み
	CBuilding::LoadParam("data\\CSV\\BuildingParam.csv");

	//オブジェクト生成+初期化
	CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), VEC3_ZERO, 40.0f, 64.0f);
	CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 72.0f);
	CBuilding* pTarget = CBuilding::Create(D3DXVECTOR3(64.0f, 0.0f, 580.0f), VEC3_ZERO, 0);
	CBuilding::Create(D3DXVECTOR3(-440.0f, 0.0f, -453.0f), VEC3_ZERO, 2);
	CObjectX* pAgit = CObjectX::Create(VEC3_ZERO, VEC3_ZERO, 4);
	CPict::SetAgit(pAgit);
	CMeshSky::Create(VEC3_ZERO, VEC3_ZERO, 10000.0f, 8, 8);

	//仮
	CPictPolice::Create(D3DXVECTOR3(-200.0f, 0.0f, 50.0f))->SetTargetObj(CBuilding::GetBuilding(0));
	CPictNormal::Create(D3DXVECTOR3(48.0f, 0.0f, 556.0f));
	CItemBullet::Create(D3DXVECTOR3(0.0f, 0.0f, 10.0f), VEC3_ZERO);

	//ポイント生成
	CPoint::Update();

	return S_OK;
}

//=================================
//終了
//=================================
void CGame::Uninit(void)
{
	CObject::ReleaseAll();

	//プレイヤー破棄
	if (m_pPlayer != NULL)
	{//プレイヤー終了
		m_pPlayer->Uninit();
		delete m_pPlayer;
		m_pPlayer = NULL;
	}
}

//=================================
//更新
//=================================
void CGame::Update(void)
{
	m_pPlayer->Update();

	if (CManager::GetInputKeyboard()->GetPress(DIK_F6))
	{
		CManager::SetMode(CScene::MODE_TITLE);
	}
}

//=================================
//描画
//=================================
void CGame::Draw(void)
{
}
