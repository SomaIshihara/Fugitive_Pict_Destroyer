//======================================================
//
//マネージャ[manager.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "debugproc.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "object.h"
#include "object2D.h"
#include "objectAnim2D.h"
#include "player.h"
#include "bg.h"
#include "multiplebg.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "block.h"
#include "item.h"
#include "effect.h"
#include "particle.h"
#include "score.h"
#include "timer.h"
#include "object3D.h"
#include "objectX.h"
#include "building.h"
#include "objectBillboard.h"
#include "pict.h"
#include "meshField.h"

//マクロ
#define FPS_SPEED	(500)	//FPS計測時間

//静的メンバ変数
CRenderer* CManager::m_pRenderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CDebugProc* CManager::m_pDebProc = NULL;
CSound* CManager::m_pSound = NULL;
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CTexture* CManager::m_pTexture = NULL;
CPlayer* CManager::m_pPlayer = NULL;
int CManager::m_nFPS = 0;
DWORD CManager::m_dwFrameCount = 0;

//仮
CMeshField* CManager::m_pMeshField = NULL;

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
	m_pInputMouse = new CInputMouse;
	m_pSound = new CSound;
	m_pRenderer = new CRenderer;
	m_pDebProc = new CDebugProc;
	m_pCamera = new CCamera;
	m_pLight = new CLight;
	m_pTexture = new CTexture;
	m_pPlayer = new CPlayer;

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

	//マウス初期化
	if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//サウンド初期化
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}

	//デバッグ初期化
	m_pDebProc->Init();

	//カメラ初期化
	if (FAILED(m_pCamera->Init()))
	{
		return E_FAIL;
	}

	//ライト初期化
	if (FAILED(m_pLight->Init()))
	{
		return E_FAIL;
	}

	//テクスチャ初期化
	if (FAILED(m_pTexture->Load()))
	{
		return E_FAIL;
	}

	//プレイヤー初期化
	if (FAILED(m_pPlayer->Init()))
	{
		return E_FAIL;
	}

	//3Dモデル読み込み
	CObjectX::Load("data\\MODEL\\jobi.x", 0);	//モデル読み込み
	CObjectX::Load("data\\MODEL\\zahyoukanban002.x", 1);	//モデル読み込み
	CObjectX::Load("data\\MODEL\\DoshinBill_01.x", 2);	//モデル読み込み
	CObjectX::Load("data\\MODEL\\hako.x", 3);

	//オブジェクト生成+初期化
	//CBG::Create();
	//CMultipleBG::Create(0.0075f,0.01f,0.02f);
	//CPlayer::Create(D3DXVECTOR3(640.0f, 420.0f, 0.0f), VEC3_ZERO,100.0f, 200.0f, 2);
	CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 64.0f);
	CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 64.0f);
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(-1024.0f, 0.0f, 1024.0f), VEC3_ZERO, 64.0f, 64.0f, 32, 32);
	CBuilding::Create(D3DXVECTOR3(64.0f,0.0f,580.0f), VEC3_ZERO, 0);
	CBuilding::Create(D3DXVECTOR3(-440.0f,0.0f,-453.0f), VEC3_ZERO, 2);
	CPict::Create(D3DXVECTOR3(100.0f,0.0f,0.0f));

	//破壊可能オブジェクト
	//ResetObj();

	//FPS計測器初期化
	m_nFPS = 0;
	m_dwFrameCount = 0;

	//サウンド流す
	m_pSound->Play(CSound::SOUND_LABEL_BGM_TITLE);

	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CManager::Uninit(void)
{
	//モデル破棄
	CObjectX::Unload();			//Xモデル

	//オブジェクト終了+破棄
	CObject2D::ReleaseAll();

	//プレイヤー破棄
	if (m_pPlayer != NULL)
	{//プレイヤー終了
		m_pPlayer->Uninit();
		delete m_pPlayer;
		m_pPlayer = NULL;
	}

	//テクスチャ破棄
	if (m_pTexture != NULL)
	{//テクスチャ終了
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = NULL;
	}

	//ライト破棄
	if (m_pLight != NULL)
	{//ライト終了
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	//カメラ破棄
	if (m_pCamera != NULL)
	{//カメラ終了
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//デバッグ破棄
	if (m_pDebProc != NULL)
	{//デバッグ終了
		m_pDebProc->Uninit();
		delete m_pDebProc;
		m_pDebProc = NULL;
	}

	//サウンド破棄
	if (m_pSound != NULL)
	{//サウンド終了
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	//マウス破棄
	if (m_pInputMouse != NULL)
	{//マウス終了
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
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
	m_pInputMouse->Update();
	m_pRenderer->Update();
	m_pCamera->Update();
	m_pLight->Update();
	m_pPlayer->Update();

	//FPS計測器の処理
	m_dwFrameCount++;

	//デバッグ表示
	m_pDebProc->Print("FPS:%d\n", m_nFPS);
	m_pDebProc->Print("[操作方法]\n");
	m_pDebProc->Print("右クリック:視点移動\n");
	m_pDebProc->Print("WASD:プレイヤー（カメラ）移動\n");
	m_pDebProc->Print("Space:弾発射\n");
	m_pDebProc->Print("[モデルに向かって]マウス左クリック:ピクトさん移動\n");
	m_pDebProc->Print("[Debug]F1:スコア設定(12345678)\n");
	m_pDebProc->Print("[Debug]F2:スコア加算(+100)\n");
	m_pDebProc->Print("[Debug]F3:タイマー設定(120秒カウントダウン)\n");
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

//=================================
//再配置
//=================================
void CManager::ResetObj(void)
{
	//ぶっ壊す
	for (int cntPriority = 0; cntPriority < PRIORITY_MAX; cntPriority++)
	{
		for (int cntObj = 0; cntObj < MAX_OBJ; cntObj++)
		{//全オブジェクト見る
			CObject* pObj = CObject::GetObject(cntPriority, cntObj);	//オブジェクト取得

			if (pObj != NULL)	//ヌルチェ
			{//なんかある
				CObject::TYPE type = pObj->GetType();	//種類取得

				if (type == CObject::TYPE_ENEMY || type == CObject::TYPE_ITEM)
				{//破壊対象
					pObj->Uninit();	//デストロイ
				}
			}
		}
	}

	//再配置
	CEnemy::Create(D3DXVECTOR3(500.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 60, 1);
	CEnemy::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 60, 1);
	CEnemy::Create(D3DXVECTOR3(700.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 60, 1);
	CItem::Create(D3DXVECTOR3(900.0f, 350.0f, 0.0f), 48.0f, 20.0f);
	CItem::Create(D3DXVECTOR3(1100.0f, 150.0f, 0.0f), 48.0f, 20.0f);
}
