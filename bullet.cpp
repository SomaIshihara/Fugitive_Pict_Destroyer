//======================================================
//
//弾処理[bullet.cpp]
//Author:石原颯馬
//
//======================================================
#include "manager.h"
#include "game.h"
#include "tutorial.h"
#include "renderer.h"
#include "texture.h"
#include "object.h"
#include "meshsky.h"
#include "bullet.h"
#include "building.h"
#include "picto.h"
#include "input.h"
#include "particle.h"
#include "effect.h"
#include "Culc.h"
#include "sound.h"

//****************************************
//2D弾
//****************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CBullet2D::CBullet2D(int nPriority) : CObject2D(nPriority)
{
	m_nIdxTexture = -1;
	m_move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	CObject::SetType(TYPE_BULLET);
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CBullet2D::CBullet2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, int nPriority) 
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
CBullet2D::~CBullet2D()
{
}

//=================================
//初期化
//=================================
HRESULT CBullet2D::Init(void)
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
void CBullet2D::Uninit(void)
{
	CObject2D::Uninit();
}

//=================================
//更新
//=================================
void CBullet2D::Update(void)
{
	//エフェクト生成
	CEffect2D::Create(GetPos(), CManager::VEC3_ZERO, 30.0f, 30.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30);

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

#if 0
	//敵との衝突判定
	if (m_Type == TYPE_PLAYER && CollisionEnemy() == true)
	{//弾が敵に当たった
		return;
	}
	if (m_Type == TYPE_ENEMY && CollisionPlayer() == true)
	{//弾がプレイヤーに当たった
		return;
	}
	if (m_Type == TYPE_PLAYER && CollisionBlock() == true)
	{//弾がブロックに当たった
		return;
	}
#endif

	//親
	CObject2D::Update();
}

//=================================
//描画
//=================================
void CBullet2D::Draw(void)
{
	CObject2D::Draw();
}

//=================================
//生成処理
//=================================
CBullet2D* CBullet2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, const TYPE type)
{
	CBullet2D* pBullet = nullptr;

	if (pBullet == nullptr)
	{
		//オブジェクトアニメーション2Dの生成
		pBullet = new CBullet2D(pos, rot, fWidth, fHeight, fSpeed);

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
		return nullptr;
	}
}

#if 0
//=================================
//ブロックとの衝突判定
//=================================
bool CBullet2D::CollisionBlock(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//オブジェクト取得

		if (pObj != nullptr)	//ヌルチェ
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
#endif

//****************************************
//ビルボード弾
//****************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CBulletBillboard::CBulletBillboard(int nPriority) : CObjectBillboard(nPriority)
{
	m_nIdxTexture = -1;
	m_move = D3DXVECTOR3(3.0f, 0.0f, 0.0f);
	CObject::SetType(TYPE_BULLET);
}

//=================================
//コンストラクタ（オーバーロード 位置向きandパターン幅高さ）
//=================================
CBulletBillboard::CBulletBillboard(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, const float fSpeed, 
	const int nPower, CPicto* firePicto, int nPriority) : CObjectBillboard(pos, rot, fWidth, fHeight, nPriority)
{
	m_nIdxTexture = -1;
	m_move.x = sinf(FIX_ROT(rot.y + D3DX_PI)) * fSpeed;
	m_move.y = sinf(FIX_ROT(rot.x + D3DX_PI)) * fSpeed;
	m_move.z = cosf(FIX_ROT(rot.y + D3DX_PI)) * fSpeed;
	m_nPower = nPower;
	m_pfirePicto = firePicto;
	CObject::SetType(TYPE_BULLET);
}

//=================================
//デストラクタ
//=================================
CBulletBillboard::~CBulletBillboard()
{
}

//=================================
//初期化
//=================================
HRESULT CBulletBillboard::Init(void)
{
	CObjectBillboard::Init();

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\EnergyBullet_01.png");

	//種類設定
	m_Type = CPicto::TYPE_NORMAL;

	return S_OK;
}

//=================================
//終了
//=================================
void CBulletBillboard::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//=================================
//更新
//=================================
void CBulletBillboard::Update(void)
{
	//エフェクト生成
	CEffectBillboard::Create(GetPos(), CManager::VEC3_ZERO, 30.0f, 30.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 30);

	//ローカル
	D3DXVECTOR3 pos = GetPos();

	//移動
	pos += m_move;

	//範囲外判定
	CScene::MODE mode = CManager::GetMode();
	if (mode == CScene::MODE_GAME)
	{//ゲーム
		if (D3DXVec3Length(&pos) >= CGame::GetSkyObj()->GetHeight())
		{
			Uninit();
			return;
		}
	}
	else if (mode == CScene::MODE_GAME)
	{//チュートリアル
		if (D3DXVec3Length(&pos) >= CTutorial::GetSkyObj()->GetHeight())
		{
			Uninit();
			return;
		}
	}

	//位置設定
	SetPos(pos);

	//建物との衝突判定
	if (m_Type == CPicto::TYPE_DESTROYER && CollisionBuilding() == true)
	{
		return;
	}

	//ピクトとの判定
	if ((m_Type == CPicto::TYPE_BLOCKER || m_Type == CPicto::TYPE_POLICE) && CollisionPicto() == true)
	{
		return;
	}

	//親
	CObjectBillboard::Update();
}

//=================================
//描画
//=================================
void CBulletBillboard::Draw(void)
{
	CObjectBillboard::Draw();
}

//=================================
//生成処理
//=================================
CBulletBillboard* CBulletBillboard::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, 
	const float fSpeed, const int nPower, const CPicto::TYPE type, CPicto* firePicto)
{
	CBulletBillboard* pBullet = nullptr;

	if (pBullet == nullptr)
	{
		//オブジェクトアニメーション2Dの生成
		pBullet = new CBulletBillboard(pos, rot, fWidth, fHeight, fSpeed, nPower, firePicto);

		//初期化
		pBullet->Init();

		//タイプ設定
		pBullet->m_Type = type;

		//仮置き
		pBullet->SetCol(D3DXCOLOR(1.0f, 0.5f, 0.16f, 1.0f));

		//テクスチャ割り当て
		pBullet->BindTexture(pBullet->m_nIdxTexture);

		return pBullet;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//建物との衝突判定
//=================================
bool CBulletBillboard::CollisionBuilding(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CBuilding* pBuilding = CBuilding::GetBuilding(cnt);

		if (pBuilding != nullptr && pBuilding->GetEndurance() > 0)	//ヌルチェ
		{//なんかある
			D3DXVECTOR3 buildPos = pBuilding->GetPos();
			float buildWidth = pBuilding->GetWidth();
			float buildDepth = pBuilding->GetDepth();

			if (GetPos().x > buildPos.x - buildWidth * 0.5f &&
				GetPos().x < buildPos.x + buildWidth * 0.5f &&
				GetPos().z > buildPos.z - buildDepth * 0.5f &&
				GetPos().z < buildPos.z + buildDepth * 0.5f)
			{
				//爆発生成
				CParticleBillboard::Create(GetPos(), 30, 32, 0.2f, 3, D3DXCOLOR(1.0f, 0.5f, 0.14f, 1.0f), 20.0f, 20.0f);
				CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_EXPLOSION);

				//攻撃
				pBuilding->AddDamage(m_nPower);

				//自分終了
				Uninit();

				//弾は敵に当たった
				return true;
			}
		}
	}
	//弾は敵に当たっていなかった
	return false;
}

//=================================
//ピクトとの衝突判定
//=================================
bool CBulletBillboard::CollisionPicto(void)
{
	if (m_Type == CPicto::TYPE_BLOCKER)
	{//ブロッカー（標的：警察）
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//全オブジェクト見る
			CPictoPolice* pPicto = CPictoPolice::GetPicto(cnt);

			if (pPicto != nullptr && pPicto != m_pfirePicto)	//ヌルチェ
			{//なんかある
				D3DXVECTOR3 pictoPos = pPicto->GetPos();
				float pictoWidth = pPicto->GetWidth();
				float pictoDepth = pPicto->GetDepth();

				if (GetPos().x > pictoPos.x - pictoWidth * 0.5f &&
					GetPos().x < pictoPos.x + pictoWidth * 0.5f &&
					GetPos().z > pictoPos.z - pictoDepth * 0.5f &&
					GetPos().z < pictoPos.z + pictoDepth * 0.5f)
				{
					//爆発生成
					CParticleBillboard::Create(GetPos(), 10, 4, 1, 2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 20.0f);
					CManager::GetSound()->Play(CSound::SOUND_LABEL_SE_DAMAGE);

					//ダメージ
					pPicto->AddDamage(m_nPower);

					//自分終了
					Uninit();

					//弾は敵に当たった
					return true;
				}
			}
		}
	}
	else if (m_Type == CPicto::TYPE_POLICE)
	{//警察（標的：デストロイヤー・ブロッカー）
		//デストロイヤー
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//全オブジェクト見る
			CPictoDestroyer* pPicto = CPictoDestroyer::GetPicto(cnt);

			if (pPicto != nullptr && pPicto != m_pfirePicto)	//ヌルチェ
			{//なんかある
				D3DXVECTOR3 pictoPos = pPicto->GetPos();
				float pictoWidth = pPicto->GetWidth();
				float pictoDepth = pPicto->GetDepth();

				if (GetPos().x > pictoPos.x - pictoWidth * 0.5f &&
					GetPos().x < pictoPos.x + pictoWidth * 0.5f &&
					GetPos().z > pictoPos.z - pictoDepth * 0.5f &&
					GetPos().z < pictoPos.z + pictoDepth * 0.5f)
				{
					//爆発生成
					CParticleBillboard::Create(GetPos(), 10, 4, 1, 2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 20.0f);

					//ダメージ
					pPicto->AddDamage(m_nPower);

					//自分終了
					Uninit();

					//弾は敵に当たった
					return true;
				}
			}
		}

		//ブロッカー
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//全オブジェクト見る
			CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);

			if (pPicto != nullptr && pPicto != m_pfirePicto)	//ヌルチェ
			{//なんかある
				D3DXVECTOR3 pictoPos = pPicto->GetPos();
				float pictoWidth = pPicto->GetWidth();
				float pictoDepth = pPicto->GetDepth();

				if (GetPos().x > pictoPos.x - pictoWidth * 0.5f &&
					GetPos().x < pictoPos.x + pictoWidth * 0.5f &&
					GetPos().z > pictoPos.z - pictoDepth * 0.5f &&
					GetPos().z < pictoPos.z + pictoDepth * 0.5f)
				{
					//爆発生成
					CParticleBillboard::Create(GetPos(), 10, 4, 1, 2, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 20.0f);

					//ダメージ
					pPicto->AddDamage(m_nPower);

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