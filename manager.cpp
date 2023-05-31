//======================================================
//
//マネージャ[manager.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "object.h"
#include "object2D.h"
#include "objectAnim2D.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"

//マクロ
#define FPS_SPEED	(500)	//FPS計測時間

//静的メンバ変数
CRenderer* CManager::m_pRenderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CDebugProc* CManager::m_pDebProc = NULL;
int CManager::m_nFPS = 0;
DWORD CManager::m_dwFrameCount = 0;

//=================================
//コンストラクタ
//=================================
CManager::CManager()
{
	
}

//=================================
//デストラクタ
//=================================
CManager::~CManager()
{}

//=================================
//初期化
//=================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	//生成
	m_pInputKeyboard = new CInputKeyboard;
	m_pRenderer = new CRenderer;
	m_pDebProc = new CDebugProc;

	//レンダラー初期化
	if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
	{
		return E_FAIL;
	}

	//キーボード初期化
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}
	//デバッグ初期化
	m_pDebProc->Init();

	//テクスチャ読み込み
	CPlayer::Load("data\\TEXTURE\\runningman000.png");
	CBG::Load("data\\TEXTURE\\wasitu01_.jpg");
	CBullet::Load("data\\TEXTURE\\EnergyBullet_01.png");
	CEnemy::Load("data\\TEXTURE\\Enemy_01.png");
	CExplosion::Load("data\\TEXTURE\\bomb0.png");

	//オブジェクト生成+初期化
	CBG::Create();
	CPlayer::Create(D3DXVECTOR3(640.0f, 420.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),200.0f, 400.0f, 8, 1, 2);
	CEnemy::Create(D3DXVECTOR3(500.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 84.0f, 60.0f, 2, 1, 60,1);
	CEnemy::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 84.0f, 60.0f, 2, 1, 60,1);
	CEnemy::Create(D3DXVECTOR3(700.0f, 300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 84.0f, 60.0f, 2, 1, 60,1);

	//FPS計測器初期化
	m_nFPS = 0;
	m_dwFrameCount = 0;

	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CManager::Uninit(void)
{
	//テクスチャ破棄
	CBullet::Unload();
	CBG::Unload();
	CObjectAnim2D::Unload();
	CPlayer::Unload();

	//オブジェクト終了+破棄
	CObject2D::ReleaseAll();

	//デバッグ破棄
	if (m_pDebProc != NULL)
	{//デバッグ終了
		m_pDebProc->Uninit();
		delete m_pDebProc;
		m_pDebProc = NULL;
	}

	//キーボード破棄
	if (m_pInputKeyboard != NULL)
	{//キーボード終了
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//レンダラー破棄
	if (m_pRenderer != NULL)
	{//レンダラー終了
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//=================================
//更新
//=================================
void CManager::Update(void)
{
	m_pInputKeyboard->Update();
	m_pRenderer->Update();

	//FPS計測器の処理
	m_dwFrameCount++;

	//デバッグ表示
	m_pDebProc->Print("FPS:%d\n", m_nFPS);
}

//=================================
//描画
//=================================
void CManager::Draw(void)
{
	m_pRenderer->Draw();
}

//=================================
//FPS計測
//=================================
void CManager::CheckFPS(DWORD dwCurrentTime, DWORD dwExecLastTime)
{
	m_nFPS = (m_dwFrameCount * 1000) / (dwCurrentTime - dwExecLastTime);
	m_dwFrameCount = 0;
}