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
#include "objectX.h"

//シーン
#include "title.h"
#include "game.h"
#include "result.h"
#include "ranking.h"

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
CScene* CManager::m_pScene = NULL;

int CManager::m_nFPS = 0;
DWORD CManager::m_dwFrameCount = 0;

//************************************************
//マネージャクラス
//************************************************
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
	if (FAILED(m_pTexture->Load("data\\preload.txt")))
	{
		return E_FAIL;
	}

	//3Dモデル読み込み
	CObjectX::Load("data\\MODEL\\jobi.x");
	CObjectX::Load("data\\MODEL\\zahyoukanban002.x");
	CObjectX::Load("data\\MODEL\\DoshinBill_01.x");
	CObjectX::Load("data\\MODEL\\hako.x");
	CObjectX::Load("data\\MODEL\\Agit.x");

	//FPS計測器初期化
	m_nFPS = 0;
	m_dwFrameCount = 0;

	//サウンド流す
	m_pSound->Play(CSound::SOUND_LABEL_BGM_TITLE);

	//モード設定
	SetMode(CScene::MODE_TITLE);

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
	CObject::ReleaseAll();

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

	m_pScene->Update();

	//この時点で死亡フラグが立っているオブジェを殺す
	CObject::Death();

	//FPS計測器の処理
	m_dwFrameCount++;

	//デバッグ表示
	m_pDebProc->Print("FPS:%d\n", m_nFPS);
	m_pDebProc->Print("[操作方法]\n");
	m_pDebProc->Print("左クリック+移動:視点移動\n");
	m_pDebProc->Print("WASD:プレイヤー（カメラ）移動\n");
	m_pDebProc->Print("Space:弾発射\n");
	m_pDebProc->Print("[モデル・ピクトに向かって]マウス左クリック:選択\n");
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
//モード設定
//=================================
void CManager::SetMode(CScene::MODE mode)
{
	//音止める
	m_pSound->Stop();

	//現在のモード破棄
	if (m_pScene != NULL)
	{//なんか入ってる
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = NULL;
	}

	if (m_pScene == NULL)
	{//ぬるぬる
		//新モード生成
		m_pScene = CScene::Create(mode);
	}
}

#if 0
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
}
#endif

//************************************************
//シーン（抽象）クラス
//************************************************
//=================================
//コンストラクタ
//=================================
CScene::CScene()
{
}

//=================================
//デストラクタ
//=================================
CScene::~CScene()
{
}

//=================================
//生成
//=================================
CScene* CScene::Create(MODE mode)
{
	CScene* pScene = NULL;

	if (pScene == NULL)
	{
		//シーンの生成
		switch (mode)
		{
		case MODE_TITLE:
			pScene = new CTitle;
			break;
		case MODE_GAME:
			pScene = new CGame;
			break;
		case MODE_RESULT:
			pScene = new CResult;
			break;
		case MODE_RANKING:
			pScene = new CRanking;
			break;
		default:	//んなもんはない
			return NULL;
			break;
		}

		//初期化
		pScene->Init();

		//モード入れとく
		pScene->m_mode = mode;

		return pScene;
	}
	else
	{
		return NULL;
	}
}
