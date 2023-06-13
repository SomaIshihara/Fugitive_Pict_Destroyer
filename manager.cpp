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
#include "objectBillboard.h"

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
	m_pInputMouse = new CInputMouse;
	m_pSound = new CSound;
	m_pRenderer = new CRenderer;
	m_pDebProc = new CDebugProc;
	m_pCamera = new CCamera;
	m_pLight = new CLight;

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

	//テクスチャ読み込み
	//2D
	CPlayer::Load("data\\TEXTURE\\runningman000.png");	//プレイヤー
	CBG::Load("data\\TEXTURE\\wasitu01_.jpg");			//1枚背景
	CBullet::Load("data\\TEXTURE\\EnergyBullet_01.png");//弾
	CEnemy::Load("data\\TEXTURE\\Enemy_01.png");		//敵
	CExplosion::Load("data\\TEXTURE\\bomb0.png");		//爆発
	for (int cnt = 0; cnt < MAX_EFFECT; cnt++)		//多重背景
	{//1枚分読み込む
		CMultipleBG::Load(c_apTexturePathMultiBG[cnt], cnt);
	}
	CEffect2D::Load("data\\TEXTURE\\effect000.jpg");			//エフェクト2D
	CEffectBillboard::Load("data\\TEXTURE\\effect000.jpg");		//エフェクトビルボード
	CParticle2D::Load("data\\TEXTURE\\effect000.jpg");			//パーティクル2D
	CParticleBillboard::Load("data\\TEXTURE\\effect000.jpg");			//パーティクルビルボード
	CScore::Load("data\\TEXTURE\\Number_Rank_01.png", 10, 1);	//スコア
	CTimer::Load("data\\TEXTURE\\Number_Rank_01.png", 10, 1);	//タイマー
	CBlock::Load("data\\TEXTURE\\Block_R_01.png");				//ブロック
	CItem::Load("data\\TEXTURE\\Item_05.png");					//アイテム

	//3D
	//CObjectX::Load("data\\MODEL\\jobi.x", 0);	//モデル読み込み
	CObjectX::Load("data\\MODEL\\zahyoukanban002.x", 0);	//モデル読み込み

	//オブジェクト生成+初期化
	//CBG::Create();
	//CMultipleBG::Create(0.0075f,0.01f,0.02f);
	CPlayer::Create(D3DXVECTOR3(640.0f, 420.0f, 0.0f), VEC3_ZERO,100.0f, 200.0f, 8, 1, 2);
	CScore::Create(D3DXVECTOR3(SCREEN_WIDTH - 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 64.0f);
	CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f + 24.0f, 32.0f, 0.0f), VEC3_ZERO, 48.0f, 64.0f);
	CObject3D::Create(VEC3_ZERO, VEC3_ZERO, 100.0f, 100.0f);
	CObjectX::Create(VEC3_ZERO, VEC3_ZERO, 0);

	//地面ブロック（うん。かつて添削会で見たことある光景。）
	CBlock::Create(D3DXVECTOR3(0.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(64.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(128.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(192.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(256.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(320.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(384.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(448.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(512.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(576.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(640.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(704.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(768.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(832.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(896.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(960.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(1024.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(1088.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(1152.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(1216.0f, 700.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(1280.0f, 700.0f, 0.0f), 64.0f, 64.0f);

	//空中ブロック
	CBlock::Create(D3DXVECTOR3(964.0f, 450.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(900.0f, 450.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(836.0f, 450.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(1100.0f, 300.0f, 0.0f), 64.0f, 64.0f);
	CBlock::Create(D3DXVECTOR3(1100.0f, 236.0f, 0.0f), 64.0f, 64.0f);
	//破壊可能オブジェクト
	ResetObj();

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
	
	//テクスチャ破棄
	CItem::Unload();			//アイテム
	CBlock::Unload();			//ブロック
	CTimer::Unload();			//タイマー
	CScore::Unload();			//スコア
	CParticle2D::Unload();		//パーティクル
	CEffect2D::Unload();			//エフェクト
	CMultipleBG::Unload();		//多重背景（まとめて破棄される）
	CExplosion::Unload();		//爆発
	CEnemy::Unload();			//敵
	CBullet::Unload();			//弾
	CBG::Unload();				//1枚背景
	CPlayer::Unload();			//プレイヤー

	//オブジェクト終了+破棄
	CObject2D::ReleaseAll();

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

	//再配置ボタンが押された
	if (m_pInputKeyboard->GetTrigger(DIK_F5) == true)
	{//再配置実行
		ResetObj();
	}

	//FPS計測器の処理
	m_dwFrameCount++;

	//デバッグ表示
	m_pDebProc->Print("FPS:%d\n", m_nFPS);
	m_pDebProc->Print("[操作方法]\n");
	m_pDebProc->Print("マウス左クリック:弾発射\n");
	m_pDebProc->Print("Space:ジャンプ\n");
	m_pDebProc->Print("F5:再配置\n");
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
	CEnemy::Create(D3DXVECTOR3(500.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60, 1);
	CEnemy::Create(D3DXVECTOR3(300.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60, 1);
	CEnemy::Create(D3DXVECTOR3(700.0f, 300.0f, 0.0f), VEC3_ZERO, 84.0f, 60.0f, 2, 1, 60, 1);
	CItem::Create(D3DXVECTOR3(900.0f, 350.0f, 0.0f), 48.0f, 20.0f);
	CItem::Create(D3DXVECTOR3(1100.0f, 150.0f, 0.0f), 48.0f, 20.0f);
}
