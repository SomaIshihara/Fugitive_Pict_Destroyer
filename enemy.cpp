//======================================================
//
//敵処理[enemy.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "enemy.h"
#include "input.h"
#include "bullet.h"
#include "Culc.h"

//マクロ
#define PLAYER_SPEED	(5.0f)	//仮の移動速度

//静的メンバ変数
LPDIRECT3DTEXTURE9 CEnemy::m_pTexture = NULL;

//=================================
//コンストラクタ（デフォルト）
//=================================
CEnemy::CEnemy()
{
	m_move = VEC3_ZERO;
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CEnemy::CEnemy(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed, const int nLife) : CObjectAnim2D(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, true)
{
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
	CObjectAnim2D::Init();

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
CEnemy* CEnemy::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed, const int nLife)
{
	CEnemy* pEnemy = NULL;

	if (pEnemy == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pEnemy = new CEnemy(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, nLife);

		//初期化
		pEnemy->Init();

		//テクスチャ割り当て
		pEnemy->BindTexture(m_pTexture);

		return pEnemy;
	}
	else
	{
		return NULL;
	}
}

//=================================
//テクスチャ読み込み処理
//=================================
HRESULT CEnemy::Load(const char* pPath)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得
	
	//テクスチャ読み込み
	if (FAILED(D3DXCreateTextureFromFile(pDevice,
		pPath,
		&m_pTexture)))
	{//失敗
		return E_FAIL;
	}

	//成功
	return S_OK;
}

//=================================
//テクスチャ破棄処理
//=================================
void CEnemy::Unload(void)
{
	//テクスチャ破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
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