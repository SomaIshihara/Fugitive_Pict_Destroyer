//======================================================
//
//プレイヤー処理[player.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "Culc.h"

//マクロ
#define PLAYER_SPEED	(5.0f)	//仮の移動速度

//静的メンバ変数
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

//=================================
//コンストラクタ（デフォルト）
//=================================
CPlayer::CPlayer()
{
	m_move = VEC3_ZERO;
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CPlayer::CPlayer(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed) : CObjectAnim2D(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, true)
{

}

//=================================
//デストラクタ
//=================================
CPlayer::~CPlayer()
{
}

//=================================
//初期化
//=================================
HRESULT CPlayer::Init(void)
{
	CObjectAnim2D::Init();

	SetType(TYPE_PLAYER);	//種類設定

	return S_OK;
}

//=================================
//終了
//=================================
void CPlayer::Uninit(void)
{
	CObjectAnim2D::Uninit();
}

//=================================
//更新
//=================================
void CPlayer::Update(void)
{
	//ローカル
	D3DXVECTOR3 pos = GetPos();
	m_move = VEC3_ZERO;

	//ポインタ取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	//情報取得と移動
	if (pKeyboard->GetPress(DIK_W))
	{
		if (pKeyboard->GetPress(DIK_A))
		{//左上
			m_move.x = sinf(LEFT_UP) * PLAYER_SPEED;
			m_move.y = cosf(LEFT_UP) * PLAYER_SPEED;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{//右上
			m_move.x = sinf(RIGHT_UP) * PLAYER_SPEED;
			m_move.y = cosf(RIGHT_UP) * PLAYER_SPEED;
		}
		else
		{//真上
			m_move.x = sinf(UP) * PLAYER_SPEED;
			m_move.y = cosf(UP) * PLAYER_SPEED;
		}
	}
	else if (pKeyboard->GetPress(DIK_S))
	{
		if (pKeyboard->GetPress(DIK_A))
		{//左下
			m_move.x = sinf(LEFT_DOWN) * PLAYER_SPEED;
			m_move.y = cosf(LEFT_DOWN) * PLAYER_SPEED;
		}
		else if (pKeyboard->GetPress(DIK_D))
		{//右下
			m_move.x = sinf(RIGHT_DOWN) * PLAYER_SPEED;
			m_move.y = cosf(RIGHT_DOWN) * PLAYER_SPEED;
		}
		else
		{//真下
			m_move.x = sinf(DOWN) * PLAYER_SPEED;
			m_move.y = cosf(DOWN) * PLAYER_SPEED;
		}
	}
	else if (pKeyboard->GetPress(DIK_A))
	{//左
		m_move.x = sinf(LEFT) * PLAYER_SPEED;
		m_move.y = cosf(LEFT) * PLAYER_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_D))
	{//右
		m_move.x = sinf(RIGHT) * PLAYER_SPEED;
		m_move.y = cosf(RIGHT) * PLAYER_SPEED;
	}

	//位置設定
	pos += m_move;
	SetPos(pos);

	//弾
	if (pKeyboard->GetRepeate(DIK_SPACE))
	{
		D3DXVECTOR3 bulletPos = pos + D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		CBullet::Create(bulletPos, GetRot(), 16.0f, 16.0f, 13.0f);
	}

	//アニメーション
	CObjectAnim2D::Update();
}

//=================================
//描画
//=================================
void CPlayer::Draw(void)
{
	CObjectAnim2D::Draw();
}

//=================================
//生成処理
//=================================
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed)
{
	CPlayer* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pPlayer = new CPlayer(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed);

		//初期化
		pPlayer->Init();

		//テクスチャ割り当て
		pPlayer->BindTexture(m_pTexture);

		return pPlayer;
	}
	else
	{
		return NULL;
	}
}

//=================================
//テクスチャ読み込み処理
//=================================
HRESULT CPlayer::Load(const char* pPath)
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
void CPlayer::Unload(void)
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
void CPlayer::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//ダメージ付与

	//死亡判定
	if (m_nLife <= DEATH_LIFE)
	{
		Uninit();	//終了処理
	}
}