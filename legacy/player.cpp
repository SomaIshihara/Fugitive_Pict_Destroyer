//======================================================
//
//プレイヤー処理[player.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "texture.h"
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
#define PLAYER_JUMP_HEIGHT	(12.0f)		//仮のジャンプ力
#define BLOCKCOLLISION_ERRORNUM	(10)	//ブロック当たり判定の誤差
#define BULLET_ROT_NUM		(32)		//弾を撃つ角度の数

//=================================
//コンストラクタ（デフォルト）
//=================================
CPlayer::CPlayer(int nPriority) : CObjectAnim2D(nPriority)
{
	m_nIdxTexture = -1;
	m_nCounterJumpTime = 0;
	m_fBulletRot = 0.0f;
	m_move = VEC3_ZERO;
	m_bJump = false;
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CPlayer::CPlayer(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight,
	const int nAnimSpeed, int nPriority) 
	: CObjectAnim2D(pos, rot, fWidth, fHeight, nAnimSpeed, true, nPriority)
{
	m_nIdxTexture = -1;
	m_nCounterJumpTime = 0;
	m_fBulletRot = 0.0f;
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

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\runningman000.png", 8, 1);

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
	CollisionBlockX(&pos);

	pos.y -= m_move.y - (ACCELERATION_GRAVITY * m_nCounterJumpTime / MAX_FPS);

	//ブロック当たり判定
	if (CollisionBlockY(&pos) == true)
	{
		m_bJump = false;
		//ジャンプ
		if (pKeyboard->GetRepeate(DIK_SPACE))
		{//ジャンプ処理
			m_bJump = true;
			m_nCounterJumpTime = 0;
			m_move.y = PLAYER_JUMP_HEIGHT;
		}
	}
	
	//アイテム当たり判定
	CollisionItem(&pos);

	//最終的な位置の設定
	SetPos(pos);

	//弾
	if (pMouse->GetRepeate(MOUSE_CLICK_LEFT) == true)
	{
		m_fBulletRot = FIX_ROT(m_fBulletRot + (D3DX_PI / BULLET_ROT_NUM));
		CBullet::Create(GetPos(),D3DXVECTOR3(0.0f, 0.0f, m_fBulletRot), 16.0f, 16.0f, 13.0f, TYPE_PLAYER);
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
CPlayer* CPlayer::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const int nAnimSpeed)
{
	CPlayer* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pPlayer = new CPlayer(pos, rot, fWidth, fHeight, nAnimSpeed);

		//初期化
		pPlayer->Init();

		//テクスチャ割り当て
		pPlayer->BindTexture(pPlayer->m_nIdxTexture);

		return pPlayer;
	}
	else
	{
		return NULL;
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
//ブロックとの衝突判定(X)
//=================================
void CPlayer::CollisionBlockX(D3DXVECTOR3* pPosNew)
{
	float fPlayerWidth = GetWidth() * 0.5f, fPlayerHeight = GetHeight() * 0.5f;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//オブジェクト取得

		if (pObj != NULL)	//ヌルチェ
		{//なんかある
			TYPE type = pObj->GetType();	//種類取得

			if (type == TYPE_BLOCK)
			{//ブロック
				float fOtherWidth = pObj->GetWidth() * 0.5f, fOtherHeight = pObj->GetHeight() * 0.5f;
				D3DXVECTOR3 otherPos = pObj->GetPos();
				if (pPosNew->y - fPlayerHeight < otherPos.y + fOtherHeight &&
					pPosNew->y + fPlayerHeight > otherPos.y - fOtherHeight)
				{
					if (GetPos().x + fPlayerWidth <= otherPos.x - fOtherWidth &&
						pPosNew->x + fPlayerWidth > otherPos.x - fOtherWidth)
					{
						pPosNew->x = otherPos.x - fOtherWidth - fPlayerWidth;
						m_move.x = 0.0f;
					}
					else if (GetPos().x - fPlayerWidth >= otherPos.x + fOtherWidth &&
						pPosNew->x - fPlayerWidth < otherPos.x + fOtherWidth)
					{
						pPosNew->x = otherPos.x + fOtherWidth + fPlayerWidth;
						m_move.x = 0.0f;
					}
				}
			}
		}
	}
}

//=================================
//ブロックとの衝突判定(Y)
//=================================
bool CPlayer::CollisionBlockY(D3DXVECTOR3* pPosNew)
{
	float fPlayerWidth = GetWidth() * 0.5f, fPlayerHeight = GetHeight() * 0.5f;
	bool bLand = false;		//着地した

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//オブジェクト取得

		if (pObj != NULL)	//ヌルチェ
		{//なんかある
			TYPE type = pObj->GetType();	//種類取得

			if (type == TYPE_BLOCK)
			{//ブロック
				float fOtherWidth = pObj->GetWidth() * 0.5f, fOtherHeight = pObj->GetHeight() * 0.5f;
				D3DXVECTOR3 otherPos = pObj->GetPos();
				if (pPosNew->x - fPlayerWidth < otherPos.x + fOtherWidth &&
					pPosNew->x + fPlayerWidth > otherPos.x - fOtherWidth)
				{
					if (GetPos().y + fPlayerHeight <= otherPos.y - fOtherHeight &&
						pPosNew->y + fPlayerHeight > otherPos.y - fOtherHeight)
					{
						bLand = true;
						m_move.y = 0.0f;
						m_nCounterJumpTime = 0;
						pPosNew->y = otherPos.y - fOtherHeight - fPlayerHeight;
					}
					else if (GetPos().y - fPlayerHeight >= otherPos.y + fOtherHeight &&
						pPosNew->y - fPlayerHeight < otherPos.y + fOtherHeight)
					{
						pPosNew->y = otherPos.y + fOtherHeight + fPlayerHeight;
						m_move.y = 0.0f;
						m_nCounterJumpTime = 0;
					}
				}
			}
		}
	}

	//床当たり判定
	if (pPosNew->y + GetHeight() * 0.5f > SCREEN_HEIGHT)
	{//いったん着地
		bLand = true;
		m_move.y = 0.0f;
		m_nCounterJumpTime = 0;
		pPosNew->y = SCREEN_HEIGHT - GetHeight() * 0.5f;
	}

	return bLand;
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
				if (pPosNew->x + (GetWidth() * 0.5f) > pObj->GetPos().x - pObj->GetWidth() * 0.5f &&
					pPosNew->x - (GetWidth() * 0.5f) < pObj->GetPos().x + pObj->GetWidth() * 0.5f &&
					pPosNew->y + (GetHeight() * 0.5f) > pObj->GetPos().y - pObj->GetHeight() * 0.5f &&
					pPosNew->y - (GetHeight() * 0.5f) < pObj->GetPos().y + pObj->GetHeight() * 0.5f)
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