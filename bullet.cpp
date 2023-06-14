//======================================================
//
//弾処理[bullet.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "object.h"
//#include "player.h"
#include "enemy.h"
#include "block.h"
#include "bullet.h"
#include "input.h"
#include "explosion.h"
#include "particle.h"
#include "effect.h"
#include "Culc.h"

//=================================
//コンストラクタ（デフォルト）
//=================================
CBullet::CBullet(int nPriority) : CObject2D(nPriority)
{
	m_nIdxTexture = -1;
	m_move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	CObject::SetType(TYPE_BULLET);
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CBullet::CBullet(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, int nPriority) 
	: CObject2D(pos, rot, fWidth, fHeight, nPriority)
{
	m_nIdxTexture = -1;
	m_move.x = sinf(FIX_ROT(rot.z * D3DX_PI + D3DX_PI)) * fSpeed;
	m_move.y = cosf(FIX_ROT(rot.z * D3DX_PI + D3DX_PI)) * fSpeed;
	CObject::SetType(TYPE_BULLET);
}

//=================================
//デストラクタ
//=================================
CBullet::~CBullet()
{
}

//=================================
//初期化
//=================================
HRESULT CBullet::Init(void)
{
	CObject2D::Init();

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\EnergyBullet_01.png");

	//種類設定
	m_Type = TYPE_PLAYER;

	return S_OK;
}

//=================================
//終了
//=================================
void CBullet::Uninit(void)
{
	CObject2D::Uninit();
}

//=================================
//更新
//=================================
void CBullet::Update(void)
{
	//エフェクト生成
	CEffect2D::Create(GetPos(), VEC3_ZERO, 30.0f, 30.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30);

	//ローカル
	D3DXVECTOR3 pos = GetPos();

	//移動
	pos += m_move;

	//範囲外判定
	if (pos.x > SCREEN_WIDTH || pos.x < 0.0f || pos.y > SCREEN_HEIGHT || pos.y < 0.0f)
	{
		Uninit();
		return;
	}

	//位置設定
	SetPos(pos);

	//敵との衝突判定
	if (m_Type == TYPE_PLAYER && CollisionEnemy() == true)
	{//弾が敵に当たった
		return;
	}
#if 0
	if (m_Type == TYPE_ENEMY && CollisionPlayer() == true)
	{//弾がプレイヤーに当たった
		return;
	}
#endif
	if (m_Type == TYPE_PLAYER && CollisionBlock() == true)
	{//弾がブロックに当たった
		return;
	}

	//親
	CObject2D::Update();
}

//=================================
//描画
//=================================
void CBullet::Draw(void)
{
	CObject2D::Draw();
}

//=================================
//生成処理
//=================================
CBullet* CBullet::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, const TYPE type)
{
	CBullet* pBullet = NULL;

	if (pBullet == NULL)
	{
		//オブジェクトアニメーション2Dの生成
		pBullet = new CBullet(pos, rot, fWidth, fHeight, fSpeed);

		//初期化
		pBullet->Init();

		//タイプ設定
		pBullet->SetType(type);

		//仮置き
		pBullet->SetCol(D3DXCOLOR(1.0f, 0.5f, 0.16f,1.0f));

		//テクスチャ割り当て
		pBullet->BindTexture(pBullet->m_nIdxTexture);

		return pBullet;
	}
	else
	{
		return NULL;
	}
}

//=================================
//敵との衝突判定
//=================================
bool CBullet::CollisionEnemy(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(ENEMY_PRIORITY,cnt);	//オブジェクト取得

		if (pObj != NULL)	//ヌルチェ
		{//なんかある
			TYPE type = pObj->GetType();	//種類取得

			if (type == TYPE_ENEMY)
			{//敵
				if (GetPos().x > pObj->GetPos().x - pObj->GetWidth() * 0.5f &&
					GetPos().x < pObj->GetPos().x + pObj->GetWidth() * 0.5f &&
					GetPos().y > pObj->GetPos().y - pObj->GetHeight() * 0.5f &&
					GetPos().y < pObj->GetPos().y + pObj->GetHeight() * 0.5f)
				{
					//爆発生成
					//CParticle2D::Create(GetPos(), 48, 16, 2, 3, D3DXCOLOR(1.0f, 0.5f, 0.14f, 1.0f), 20.0f, 20.0f);
					CParticleBillboard::Create(D3DXVECTOR3(0.0f,20.0f,0.0f), 48, 48, 2, 3, D3DXCOLOR(1.0f, 0.5f, 0.14f, 1.0f), 8.0f, 8.0f);
					//CExplosion::Create(GetPos(), GetRot(), 80.0f, 80.0f, 8, 2, 3);

					//敵にダメージ
					pObj->Uninit();

					//自分終了
					Uninit();

					//弾は敵に当たった
					return true;
				}
			}
		}
	}
	//弾は敵に当たっていなかった
	return false;
}

#if 0
//=================================
//敵との衝突判定
//=================================
bool CBullet::CollisionPlayer(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(PLAYER_PRIORITY,cnt);	//オブジェクト取得

		if (pObj != NULL)	//ヌルチェ
		{//なんかある
			TYPE type = pObj->GetType();	//種類取得

			if (type == TYPE_PLAYER)
			{//敵
				if (GetPos().x > pObj->GetPos().x - pObj->GetWidth() * 0.5f &&
					GetPos().x < pObj->GetPos().x + pObj->GetWidth() * 0.5f &&
					GetPos().y > pObj->GetPos().y - pObj->GetHeight() * 0.5f &&
					GetPos().y < pObj->GetPos().y + pObj->GetHeight() * 0.5f)
				{
					//爆発生成
					CParticle2D::Create(GetPos(), 48, 16, 2, 3, D3DXCOLOR(1.0f, 0.5f, 0.14f, 1.0f), 20.0f, 20.0f);

					//敵にダメージ
					pObj->Uninit();

					//自分終了
					Uninit();

					//弾は敵に当たった
					return true;
				}
			}
		}
	}
	//弾は敵に当たっていなかった
	return false;
}
#endif

//=================================
//ブロックとの衝突判定
//=================================
bool CBullet::CollisionBlock(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//オブジェクト取得

		if (pObj != NULL)	//ヌルチェ
		{//なんかある
			TYPE type = pObj->GetType();	//種類取得

			if (type == TYPE_BLOCK)
			{//敵
				if (GetPos().x > pObj->GetPos().x - pObj->GetWidth() * 0.5f &&
					GetPos().x < pObj->GetPos().x + pObj->GetWidth() * 0.5f &&
					GetPos().y > pObj->GetPos().y - pObj->GetHeight() * 0.5f &&
					GetPos().y < pObj->GetPos().y + pObj->GetHeight() * 0.5f)
				{
					//爆発生成
					CParticle2D::Create(GetPos(), 48, 16, 2, 3, D3DXCOLOR(1.0f, 0.5f, 0.14f, 1.0f), 20.0f, 20.0f);

					//自分終了
					Uninit();

					//弾は敵に当たった
					return true;
				}
			}
		}
	}
	//弾は敵に当たっていなかった
	return false;
}