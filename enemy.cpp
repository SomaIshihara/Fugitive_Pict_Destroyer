//======================================================
//
//敵処理[enemy.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "enemy.h"
#include "input.h"
#include "bullet.h"
#include "Culc.h"

//マクロ
#define PLAYER_SPEED	(5.0f)	//仮の移動速度

//=================================
//コンストラクタ（デフォルト）
//=================================
CEnemy::CEnemy(int nPriority) : CObjectAnim2D(nPriority)
{
	m_nIdxTexture = -1;
	m_move = CManager::VEC3_ZERO;
	m_nLife = 1;
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CEnemy::CEnemy(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nAnimSpeed, const int nLife, int nPriority)
	: CObjectAnim2D(pos, rot, fWidth, fHeight, nAnimSpeed, true, nPriority)
{
	m_nIdxTexture = -1;
	m_move = CManager::VEC3_ZERO;
	m_nLife = nLife;
}

//=================================
//デストラクタ
//=================================
CEnemy::~CEnemy()
{
}

//=================================
//初期化
//=================================
HRESULT CEnemy::Init(void)
{
	//親クラス処理
	if (FAILED(CObjectAnim2D::Init()))
	{
		return E_FAIL;
	}

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Enemy_01.png", 2, 1);

	//種類設定
	SetType(TYPE_ENEMY);

	return S_OK;
}

//=================================
//終了
//=================================
void CEnemy::Uninit(void)
{
	CObjectAnim2D::Uninit();
}

//=================================
//更新
//=================================
void CEnemy::Update(void)
{
	//アニメーション
	CObjectAnim2D::Update();
}

//=================================
//描画
//=================================
void CEnemy::Draw(void)
{
	CObjectAnim2D::Draw();
}

//=================================
//生成処理
//=================================
CEnemy* CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nAnimSpeed, const int nLife)
{
	CEnemy* pEnemy = nullptr;

	if (pEnemy == nullptr)
	{
		//敵の生成
		pEnemy = new CEnemy(pos, rot, fWidth, fHeight, nAnimSpeed, nLife);

		//初期化
		pEnemy->Init();

		//テクスチャ設定
		pEnemy->BindTexture(pEnemy->m_nIdxTexture);

		return pEnemy;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//ダメージ設定および死亡判定処理
//=================================
void CEnemy::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//ダメージ付与

	//死亡判定
	if (m_nLife <= DEATH_LIFE)
	{
		Uninit();	//終了処理
	}
}