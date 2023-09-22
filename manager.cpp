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
#include "xmodel.h"
#include "fade.h"

//シーン
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "ranking.h"

//乱数
#include <time.h>

//マクロ
#define FPS_SPEED	(500)	//FPS計測時間

//静的メンバ変数
CRenderer* CManager::m_pRenderer = nullptr;
CInputKeyboard* CManager::m_pInputKeyboard = nullptr;
CInputMouse* CManager::m_pInputMouse = nullptr;
CDebugProc* CManager::m_pDebProc = nullptr;
CSound* CManager::m_pSound = nullptr;
CCamera* CManager::m_pCamera = nullptr;
CLight* CManager::m_pLight = nullptr;
CTexture* CManager::m_pTexture = nullptr;
CScene* CManager::m_pScene = nullptr;
//仮
CXModel* CManager::m_pAgitModel = nullptr;
CXModel* CManager::m_pFragModel = nullptr;

//静的const
const int CManager::INT_ZERO = 0;
const float CManager::FLOAT_ZERO = 0.0f;
const D3DXVECTOR2 CManager::VEC2_ZERO = D3DXVECTOR2(0.0f, 0.0f);
const D3DXVECTOR3 CManager::VEC3_ZERO = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

bool CManager::m_bPause = false;

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
	//乱数を乱数にする
	srand((unsigned int)time(nullptr));

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
	m_pFragModel = CXModel::Load("data\\MODEL\\frag.x");
	m_pAgitModel = CXModel::Load("data\\MODEL\\Agit.x");

	//FPS計測器初期化
	m_nFPS = 0;
	m_dwFrameCount = 0;

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
	CXModel::UnloadAll();			//Xモデル

	//オブジェクト終了+破棄
	CObject::ReleaseAll();

	//テクスチャ破棄
	if (m_pTexture != nullptr)
	{//テクスチャ終了
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	//ライト破棄
	if (m_pLight != nullptr)
	{//ライト終了
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}

	//カメラ破棄
	if (m_pCamera != nullptr)
	{//カメラ終了
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	//デバッグ破棄
	if (m_pDebProc != nullptr)
	{//デバッグ終了
		m_pDebProc->Uninit();
		delete m_pDebProc;
		m_pDebProc = nullptr;
	}

	//サウンド破棄
	if (m_pSound != nullptr)
	{//サウンド終了
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	//マウス破棄
	if (m_pInputMouse != nullptr)
	{//マウス終了
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = nullptr;
	}

	//キーボード破棄
	if (m_pInputKeyboard != nullptr)
	{//キーボード終了
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	//レンダラー破棄
	if (m_pRenderer != nullptr)
	{//レンダラー終了
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

//=================================
//更新
//=================================
void CManager::Update(void)
{
	//入力系
	m_pInputKeyboard->Update();
	m_pInputMouse->Update();

	//描画系
	if (m_bPause == false)
	{//ポーズしていなければ全オブジェクト更新
		m_pRenderer->Update();
	}

	//3D系
	m_pCamera->Update();
	m_pLight->Update();

	//中身知らんけど何かしらのシーン
	m_pScene->Update();

	//この時点で死亡フラグが立っているオブジェを殺す
	CObject::Death();

	//FPS計測器の処理
	m_dwFrameCount++;

	//デバッグ表示
	m_pDebProc->Print("FPS:%d\n", m_nFPS);
#ifdef _DEBUG
	m_pDebProc->Print("[Debug]F3:タイマー設定(2秒カウントダウン)\n");
#endif // _DEBUG
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
	if (m_pScene != nullptr)
	{//なんか入ってる
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	if (m_pScene == nullptr)
	{//ぬるぬる
		//新モード生成
		m_pScene = CScene::Create(mode);
	}
}

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
	CScene* pScene = nullptr;

	if (pScene == nullptr)
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
		case MODE_TUTORIAL:
			pScene = new CTutorial;
			break;
		case MODE_RANKING:
			pScene = new CRanking;
			break;
		default:	//んなもんはない
			return nullptr;
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
		return nullptr;
	}
}
