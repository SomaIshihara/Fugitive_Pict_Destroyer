//======================================================
//
//プレイヤー処理[player.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "sound.h"
#include "renderer.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "block.h"
#include "item.h"
#include "score.h"
#include "Culc.h"

//マクロ
#define PLAYER_SPEED	(5.0f)			//仮の移動速度
#define PLAYER_JUMP_HEIGHT	(10.0f)		//仮のジャンプ力
#define BLOCKCOLLISION_ERRORNUM	(10)	//ブロック当たり判定の誤差

//静的メンバ変数
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

//=================================
//コンストラクタ（デフォルト）
//=================================
CPlayer::CPlayer(int nPriority) : CObjectAnim2D(nPriority)
{
	m_nCounterJumpTime = 0;
	m_move = VEC3_ZERO;
	m_bJump = false;
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CPlayer::CPlayer(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nPatWidth, const int nPatHeight, const int nAnimSpeed, int nPriority) 
	: CObjectAnim2D(pos, rot, fWidth, fHeight, nPatWidth, nPatHeight, nAnimSpeed, true, nPriority)
{
	m_nCounterJumpTime = 0;
	m_move = VEC3_ZERO;
	m_bJump = false;
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
	m_move.x = 0.0f;

	//ジャンプカウンタ増やす
	m_nCounterJumpTime++;

	//ポインタ取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();
	CInputMouse* pMouse = CManager::GetInputMouse();

	//情報取得と移動
	//if (pKeyboard->GetPress(DIK_W))
	//{
	//	if (pKeyboard->GetPress(DIK_A))
	//	{//左上
	//		m_move.x = sinf(LEFT_UP) * PLAYER_SPEED;
	//		m_move.y = cosf(LEFT_UP) * PLAYER_SPEED;
	//	}
	//	else if (pKeyboard->GetPress(DIK_D))
	//	{//右上
	//		m_move.x = sinf(RIGHT_UP) * PLAYER_SPEED;
	//		m_move.y = cosf(RIGHT_UP) * PLAYER_SPEED;
	//	}
	//	else
	//	{//真上
	//		m_move.x = sinf(UP) * PLAYER_SPEED;
	//		m_move.y = cosf(UP) * PLAYER_SPEED;
	//	}
	//}
	//else if (pKeyboard->GetPress(DIK_S))
	//{
	//	if (pKeyboard->GetPress(DIK_A))
	//	{//左下
	//		m_move.x = sinf(LEFT_DOWN) * PLAYER_SPEED;
	//		m_move.y = cosf(LEFT_DOWN) * PLAYER_SPEED;
	//	}
	//	else if (pKeyboard->GetPress(DIK_D))
	//	{//右下
	//		m_move.x = sinf(RIGHT_DOWN) * PLAYER_SPEED;
	//		m_move.y = cosf(RIGHT_DOWN) * PLAYER_SPEED;
	//	}
	//	else
	//	{//真下
	//		m_move.x = sinf(DOWN) * PLAYER_SPEED;
	//		m_move.y = cosf(DOWN) * PLAYER_SPEED;
	//	}
	//}
	//else 
	if (pKeyboard->GetPress(DIK_A))
	{//左
		m_move.x = sinf(LEFT) * PLAYER_SPEED;
		//m_move.y = cosf(LEFT) * PLAYER_SPEED;
	}
	else if (pKeyboard->GetPress(DIK_D))
	{//右
		m_move.x = sinf(RIGHT) * PLAYER_SPEED;
		//m_move.y = cosf(RIGHT) * PLAYER_SPEED;
	}

	//位置設定(この時点ではまだオブジェクトの位置は更新されてない)
	pos.x += m_move.x;
	pos.y -= m_move.y - (ACCELERATION_GRAVITY * m_nCounterJumpTime / MAX_FPS);

	//ブロック当たり判定
	CollisionBlock(&pos);

	//アイテム当たり判定
	CollisionItem(&pos);

	//ジャンプ
	if (m_bJump == false && pKeyboard->GetRepeate(DIK_SPACE))
	{//ジャンプ処理
		m_bJump = true;
		m_nCounterJumpTime = 0;
		m_move.y = PLAYER_JUMP_HEIGHT;
	}

	//ブロックとの当たり判定とジャンプ
	if (pos.y + GetHeight() / 2 - m_move.y - (ACCELERATION_GRAVITY * m_nCounterJumpTime / MAX_FPS) > SCREEN_HEIGHT)
	{//いったん着地
		m_bJump = false;
		m_move.y = 0.0f;
		m_nCounterJumpTime = 0;
		pos.y = SCREEN_HEIGHT - GetHeight() / 2;
	}

	//最終的な位置の設定
	SetPos(pos);

	//弾
	if (pMouse->GetRepeate(MOUSE_CLICK_LEFT) == true)
	{
		D3DXVECTOR3 bulletPos = pos + D3DXVECTOR3(10.0f, 0.0f, 0.0f);
		CBullet::Create(bulletPos, GetRot(), 16.0f, 16.0f, 13.0f, TYPE_PLAYER);
		CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_SHOT);
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

//=================================
//ブロックとの衝突判定
//=================================
void CPlayer::CollisionBlock(D3DXVECTOR3* pPosNew)
{
	bool bHitHead = false;	//頭ぶつけた

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//オブジェクト取得

		if (pObj != NULL)	//ヌルチェ
		{//なんかある
			TYPE type = pObj->GetType();	//種類取得

			if (type == TYPE_BLOCK)
			{//ブロック
				if (pPosNew->x + (GetWidth() / 2) > pObj->GetPos().x - pObj->GetWidth() / 2 &&
					pPosNew->x - (GetWidth() / 2) < pObj->GetPos().x + pObj->GetWidth() / 2 &&
					pPosNew->y + (GetHeight() / 2) > pObj->GetPos().y - pObj->GetHeight() / 2 &&
					pPosNew->y - (GetHeight() / 2) < pObj->GetPos().y + pObj->GetHeight() / 2)
				{//何かしらめり込んだ 
					if (GetPos().y + (GetHeight() / 2) <= pObj->GetPos().y - (pObj->GetHeight() / 2) && 
						pPosNew->y + (GetHeight() / 2) > pObj->GetPos().y - (pObj->GetHeight() / 2))
					{
						m_bJump = false;
						m_move.y = 0.0f;
						m_nCounterJumpTime = 0;
						pPosNew->y = pObj->GetPos().y - (pObj->GetHeight() / 2) - (GetHeight() / 2);
					}
					else if (GetPos().y - (GetHeight() / 2) >= pObj->GetPos().y + (pObj->GetHeight() / 2) &&
							 pPosNew->y - (GetHeight() / 2) < pObj->GetPos().y + (pObj->GetHeight() / 2))
					{
						bHitHead = true;
					}

					if (pPosNew->y - (GetHeight() / 2) < pObj->GetPos().y + (pObj->GetHeight() / 2) &&
						pPosNew->y + (GetHeight() / 2) > pObj->GetPos().y - (pObj->GetHeight() / 2))
					{
						if (GetPos().x + (GetWidth() / 2) <= pObj->GetPos().x - (pObj->GetWidth() / 2) && 
							pPosNew->x + (GetWidth() / 2) > pObj->GetPos().x - (pObj->GetWidth() / 2))
						{
							pPosNew->x = pObj->GetPos().x - (pObj->GetWidth() / 2) - (GetWidth() / 2);
							m_move.x = 0.0f;
							bHitHead = false;
						}
						else if (GetPos().x - (GetWidth() / 2) >= pObj->GetPos().x + (pObj->GetWidth() / 2) &&
								 pPosNew->x - (GetWidth() / 2) < pObj->GetPos().x + (pObj->GetWidth() / 2))
						{
							pPosNew->x = pObj->GetPos().x + (pObj->GetWidth() / 2) + (GetWidth() / 2);
							m_move.x = 0.0f;
							bHitHead = false;
						}
					}
					if (bHitHead)
					{
						pPosNew->y = pObj->GetPos().y + (pObj->GetHeight() / 2) + (GetHeight() / 2);
						m_move.y = 0.0f;
						m_nCounterJumpTime = 0;
					}
					if ((pPosNew->y + (GetHeight() / 2)) - (pObj->GetPos().y - (pObj->GetHeight() / 2)) < BLOCKCOLLISION_ERRORNUM)
					{
						pPosNew->y = pObj->GetPos().y - (pObj->GetHeight() / 2) - (GetHeight() / 2);
					}
				}
			}
		}
	}
}

//=================================
//ブロックとの衝突判定
//=================================
void CPlayer::CollisionItem(D3DXVECTOR3* pPosNew)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(ITEM_PRIORITY, cnt);	//オブジェクト取得

		if (pObj != NULL)	//ヌルチェ
		{//なんかある
			TYPE type = pObj->GetType();	//種類取得

			if (type == TYPE_ITEM)
			{//アイテム
				if (pPosNew->x + (GetWidth() / 2) > pObj->GetPos().x - pObj->GetWidth() / 2 &&
					pPosNew->x - (GetWidth() / 2) < pObj->GetPos().x + pObj->GetWidth() / 2 &&
					pPosNew->y + (GetHeight() / 2) > pObj->GetPos().y - pObj->GetHeight() / 2 &&
					pPosNew->y - (GetHeight() / 2) < pObj->GetPos().y + pObj->GetHeight() / 2)
				{//何かしらめり込んだ 
					//スコア加算
					CScore::Add(10000);	//いったん決め打ち赤スパ

					//アイテム消す
					pObj->Uninit();
				}
			}
		}
	}
}