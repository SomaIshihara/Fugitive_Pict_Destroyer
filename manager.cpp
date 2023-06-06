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
#include "object.h"
#include "object2D.h"
#include "objectAnim2D.h"
#include "player.h"
#include "bg.h"
#include "multiplebg.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "score.h"
#include "timer.h"

//マクロ
#define FPS_SPEED	(500)	//FPS計測時間

//静的メンバ変数
CRenderer* CManager::m_pRenderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CDebugProc* CManager::m_pDebProc = NULL;
CSound* CManager::m_pSound = NULL;
int CManager::m_nFPS = 0;
DWORD CManager::m_dwFrameCount = 0;

//テクスチャパス
const char* c_apTexturePathMultiBG[MAX_EFFECT] =
{
	"data\\TEXTURE\\bg100.png",
	"data\\TEXTURE\\bg101.png",
	"data\\TEXTURE\\bg102.png"
};

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
	m_pSound = new CSound;
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

	//サウンド初期化
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}
	//デバッグ初期化
	m_pDebProc->Init();

	//テクスチャ読み込み
	CPlayer::Load("data\\TEXTURE\\runningman000.png");	//プレイヤー
	CBG::Load("data\\TEXTURE\\wasitu01_.jpg");			//1枚背景
	CBullet::Load("data\\TEXTURE\\EnergyBullet_01.png");//弾
	CEnemy::Load("data\\TEXTURE\\Enemy_01.png");		//敵
	CExplosion::Load("data\\TEXTURE\\bomb0.png");		//爆発
	for (int cnt = 0; cnt < MAX_EFFECT; cnt++)		//多重背景
	{//1枚分読み込む
		CMultipleBG::Load(c_apTexturePathMultiBG[cnt], cnt);
	}
	CEffect::Load("data\\TEXTURE\\effect000.jpg");				//エフェクト
	CParticle::Load("data\\TEXTURE\\effect000.jpg");			//パーティクル
	CScore::Load("data\\TEXTURE\\Number_Rank_01.png", 10, 1);	//スコア
	CTimer::Load("data\\TEXTURE\\Number_Rank_01.png", 10, 1);	//タイマー

	//オブジェクト生成+初期化
	//CBG::Create();
	CMultipleBG::Create(0.0075f,0.01f,0.02f);
	CPlayer::Create(D3DXVECTOR3(640.0f, 420.0f, 0.0f), VEC3_ZERO,100.0f, 200.0f, 8, 1, 2);
	CEnemy::Create(D3DXVECTOR3(500.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60,1);
	CEnemy::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60,1);
	CEnemy::Create(D3DXVECTOR3(700.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60,1);
	CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 64.0f);
	CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 64.0f);

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
	//テクスチャ破棄
	CTimer::Unload();			//タイマー
	CScore::Unload();			//スコア
	CParticle::Unload();		//パーティクル
	CEffect::Unload();			//エフェクト
	CMultipleBG::Unload();		//多重背景（まとめて破棄される）
	CExplosion::Unload();		//爆発
	CEnemy::Unload();			//敵
	CBullet::Unload();			//弾
	CBG::Unload();				//1枚背景
	CPlayer::Unload();			//プレイヤー

	//オブジェクト終了+破棄
	CObject2D::ReleaseAll();

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
	m_pDebProc->Print("[操作方法]\n");
	m_pDebProc->Print("Space:弾発射\n");
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