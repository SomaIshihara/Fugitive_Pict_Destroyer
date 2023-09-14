//======================================================
//
//パーティクル処理[particle.cpp]
//Author:石原颯馬
//
//======================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "effect.h"
#include "Culc.h"

//マクロ
#define ROT_ACCU	(100)	//角度の精度
#define SPEED_ACCU	(100)	//速度の精度
#define PART_EFFECT_LIFE	(20)	//エフェクト用パーティクルの寿命

//****************************************
//2Dパーティクル
//****************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CParticle2D::CParticle2D()
{
	//パーティクルのクリア
	m_particle.pos = CManager::VEC3_ZERO;
	m_particle.nLife = CManager::INT_ZERO;
	m_particle.nEffeceNum = CManager::INT_ZERO;
	m_particle.fSpeedBace = CManager::FLOAT_ZERO;
	m_particle.fSpeedDegree = CManager::FLOAT_ZERO;
	m_particle.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_particle.fWidth = CManager::FLOAT_ZERO;
	m_particle.fHeight = CManager::FLOAT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CParticle2D::CParticle2D(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
	const D3DXCOLOR col, const float fWidth, const float fHeight)
{
	//パーティクルの初期化
	m_particle.pos = pos;
	m_particle.nLife = nLife;
	m_particle.nEffeceNum = nEffeceNum;
	m_particle.fSpeedBace = fSpeedBace;
	m_particle.fSpeedDegree = fSpeedDegree;
	m_particle.col = col;
	m_particle.fWidth = fWidth;
	m_particle.fHeight = fHeight;
}

//=================================
//デストラクタ
//=================================
CParticle2D::~CParticle2D()
{
}

//=================================
//初期化
//=================================
HRESULT CParticle2D::Init(void)
{
	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CParticle2D::Uninit(void)
{
	//パーティクル管理オブジェクト破棄
	Release();
}

//=================================
//更新
//=================================
void CParticle2D::Update(void)
{
	//エフェクト追加処理
	for (int cntEffectNum = 0; cntEffectNum < m_particle.nEffeceNum; cntEffectNum++)
	{
		//位置（パーティクルの位置と同じ）
		//移動量
		float fRot = (float)(rand() % (int)(ROT_ACCU * TWO_PI + 1) - (int)(D3DX_PI * ROT_ACCU)) / (float)ROT_ACCU;
		float fSpeed = (rand() % (int)(m_particle.fSpeedDegree * SPEED_ACCU)) / SPEED_ACCU + m_particle.fSpeedBace;
		D3DXVECTOR3 move;
		move.x = sinf(fRot) * fSpeed;	//X
		move.y = cosf(fRot) * fSpeed;	//Y
		move.z = 0.0f;

		//幅高さ（パーティクルにある）
		//色（パーティクルにある）
		//寿命（マクロ）

		//エフェクトを出す
		CEffect2D* pEffect;
		pEffect = pEffect->Create(m_particle.pos, move, m_particle.fWidth, m_particle.fHeight, m_particle.col, PART_EFFECT_LIFE);
		pEffect->Init();

		//AllEffectのほうのfor文を抜ける
		break;
	}

	//エフェクト寿命管理
	m_particle.nLife--;

	if (m_particle.nLife <= CManager::INT_ZERO)
	{
		Uninit();
	}
}

//=================================
//生成処理
//=================================
CParticle2D* CParticle2D::Create(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
	const D3DXCOLOR col, const float fWidth, const float fHeight)
{
	CParticle2D* pParticle = nullptr;

	if (pParticle == nullptr)
	{
		//パーティクル管理オブジェクト生成
		pParticle = new CParticle2D(pos, nLife, nEffeceNum, fSpeedBace, fSpeedDegree, col, fWidth, fHeight);

		//パーティクル管理オブジェクト初期化
		pParticle->Init();

		return pParticle;
	}
	else
	{
		return nullptr;
	}
}

//****************************************
//ビルボードパーティクル
//****************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CParticleBillboard::CParticleBillboard()
{
	//パーティクルのクリア
	m_particle.pos = CManager::VEC3_ZERO;
	m_particle.nLife = CManager::INT_ZERO;
	m_particle.nEffeceNum = CManager::INT_ZERO;
	m_particle.fSpeedBace = CManager::FLOAT_ZERO;
	m_particle.fSpeedDegree = CManager::FLOAT_ZERO;
	m_particle.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_particle.fWidth = CManager::FLOAT_ZERO;
	m_particle.fHeight = CManager::FLOAT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CParticleBillboard::CParticleBillboard(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
	const D3DXCOLOR col, const float fWidth, const float fHeight)
{
	//パーティクルの初期化
	m_particle.pos = pos;
	m_particle.nLife = nLife;
	m_particle.nEffeceNum = nEffeceNum;
	m_particle.fSpeedBace = fSpeedBace;
	m_particle.fSpeedDegree = fSpeedDegree;
	m_particle.col = col;
	m_particle.fWidth = fWidth;
	m_particle.fHeight = fHeight;
}

//=================================
//デストラクタ
//=================================
CParticleBillboard::~CParticleBillboard()
{
}

//=================================
//初期化
//=================================
HRESULT CParticleBillboard::Init(void)
{
	//できた
	return S_OK;
}

//=================================
//終了
//=================================
void CParticleBillboard::Uninit(void)
{
	//パーティクル管理オブジェクト破棄
	Release();
}

//=================================
//更新
//=================================
void CParticleBillboard::Update(void)
{
	//エフェクト追加処理
	for (int cntEffectNum = 0; cntEffectNum < m_particle.nEffeceNum; cntEffectNum++)
	{
		//位置（パーティクルの位置と同じ）
		//移動量
		float fRotZ = (float)(rand() % (int)(ROT_ACCU * TWO_PI + 1) - (int)(D3DX_PI * ROT_ACCU)) / (float)ROT_ACCU;
		float fRotY = (float)(rand() % (int)(ROT_ACCU * TWO_PI + 1) - (int)(D3DX_PI * ROT_ACCU)) / (float)ROT_ACCU;
		float fSpeed = (rand() % (int)(m_particle.fSpeedDegree * SPEED_ACCU)) / SPEED_ACCU + m_particle.fSpeedBace;
		D3DXVECTOR3 move;
		move.x = sinf(fRotZ) * cosf(fRotY) * fSpeed;	//X
		move.y = cosf(fRotZ) * fSpeed;					//Y
		move.z = cosf(fRotZ) * cosf(fRotY) * fSpeed;	//Z

		//幅高さ（パーティクルにある）
		//色（パーティクルにある）
		//寿命（マクロ）

		//エフェクトを出す
		CEffectBillboard* pEffect;
		pEffect = pEffect->Create(m_particle.pos, move, m_particle.fWidth, m_particle.fHeight, m_particle.col, PART_EFFECT_LIFE);
		pEffect->Init();

		//AllEffectのほうのfor文を抜ける
		break;
	}

	//エフェクト寿命管理
	m_particle.nLife--;

	if (m_particle.nLife <= CManager::INT_ZERO)
	{
		Uninit();
	}
}

//=================================
//生成処理
//=================================
CParticleBillboard* CParticleBillboard::Create(const D3DXVECTOR3 pos, const int nLife, const int nEffeceNum, const float fSpeedBace, const float fSpeedDegree,
	const D3DXCOLOR col, const float fWidth, const float fHeight)
{
	CParticleBillboard* pParticle = nullptr;

	if (pParticle == nullptr)
	{
		//パーティクル管理オブジェクト生成
		pParticle = new CParticleBillboard(pos, nLife, nEffeceNum, fSpeedBace, fSpeedDegree, col, fWidth, fHeight);

		//パーティクル管理オブジェクト初期化
		pParticle->Init();

		return pParticle;
	}
	else
	{
		return nullptr;
	}
}