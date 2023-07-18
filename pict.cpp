//======================================================
//
//ピクトさん処理[pict.cpp]
//Author:石原颯馬
//
//======================================================
#include "pict.h"
#include "debugproc.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"	//仮
#include "camera.h"	//仮
#include "block.h"	//仮
#include "player.h"
#include "bullet.h"
#include "motion.h"
#include "building.h"
#include "shadow.h"
#include "meshField.h"
#include "slider.h"
#include "file.h"
#include "Culc.h"

//マクロ
#define PICT_WALK_SPEED				(6.0f)		//ピクトさんの歩行速度
#define PICT_AGIT_STOP_LENGTH		(20.0f)		//ピクトさんがアジトから離れる距離
#define PICT_BUIDING_STOP_LENGTH	(120.0f)	//ピクトさんが建物から離れる距離
#define PICT_POLICE_STOP_LENGTH		(30.0f)		//ピクトさんが警察から離れる距離
#define PICT_POLICE_SEARCH_LENGTH	(60.0f)		//ピクト警察のサーチ範囲
#define PICT_ATTACK_TIME			(60)		//攻撃を行う間隔
#define PICT_DAMAGE_ALPHA			(0.9f)		//赤くする割合
#define PICT_DAMAGE_TIME			(120)		//赤くする時間
#define PICT_LIFE					(1000)		//体力
#define PICT_RESCUE_LIFE			(0.5f)		//救助する体力割合

#define PICT_FORCEDRETURN_NUM		(2)			//強制帰宅するまでの人数
#define PICT_NORMAL_D_PERCENT		(15)		//一般人ピクトがデストロイヤーになる確率
#define PICT_NORMAL_B_PERCENT		(20)		//一般人ピクトがブロッカーになる確率
#define PICT_NORMAL_N_PERCENT		(65)		//一般人ピクトでした
#define PICT_NORMAL_NUM_MIN			(500)		//一般人ピクトの最低人数
#define PICT_NORMAL_NUM_DEGREE		(2500)		//一般人ピクトの人数振れ幅

//静的メンバ変数
CPict* CPict::m_apPict[MAX_OBJ];
int CPict::m_nNumAll = 0;
CObjectX* CPict::m_pAgitObj = NULL;
CPictDestroyer* CPictDestroyer::m_apPict[MAX_OBJ];
int CPictDestroyer::m_nNumAll = 0;
CPictBlocker* CPictBlocker::m_apPict[MAX_OBJ];
int CPictBlocker::m_nNumAll = 0;
CPictTaxi* CPictTaxi::m_apPict[MAX_OBJ];
int CPictTaxi::m_nNumAll = 0;
CPictNormal* CPictNormal::m_apPict[MAX_OBJ];
int CPictNormal::m_nNumAll = 0;
CPictPolice* CPictPolice::m_apPict[MAX_OBJ];
int CPictPolice::m_nNumAll = 0;

//******************************************************
//ただのピクトクラス
//******************************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CPict::CPict()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	//値クリア
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_pMotion = NULL;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_bControll = false;
	m_nLife = INT_ZERO;
	m_fRedAlpha = FLOAT_ZERO;
	m_state = STATE_MAX;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPict::CPict(const D3DXVECTOR3 pos)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	//値クリア
	m_pos = pos;
	m_rot = VEC3_ZERO;
	m_pMotion = NULL;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_bControll = false;
	m_nLife = INT_ZERO;
	m_fRedAlpha = FLOAT_ZERO;
	m_state = STATE_MAX;
}

//=================================
//デストラクタ
//=================================
CPict::~CPict()
{
}

//========================
//初期化処理
//========================
HRESULT CPict::Init(void)
{
	//モーション生成・初期化
	m_pMotion = new CMotion;
	m_pMotion->Init();

	//モーションビューアのファイルを読み込み
	LoadMotionViewerFile("data\\motion_exithuman.txt", &m_apModel[0], m_pMotion, &m_nNumModel);

	//モーション設定
	m_pMotion->Set(0);

	//コリジョン設定
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(30.0f, 30.0f, 30.0f);
	m_collision.SetVtx(vtxMax, -vtxMax);

	//影設定
	m_pShadow = CShadow::Create();

	//サイズ設定
	m_fWidth = 30.0f;
	m_fHeight = 120.0f;
	m_fDepth = 30.0f;

	//ジャンプリセット
	m_nCounterJumpTime = 0;
	m_bJump = false;

	//操縦しない設定
	m_bControll = false;

	//仮：体力設定
	m_nLife = PICT_LIFE;

	//できた
	return S_OK;
}

//========================
//終了処理
//========================
void CPict::Uninit(void)
{
	//警察ターゲット解除
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPictPolice* pPict = CPictPolice::GetPict(cnt);	//オブジェクト取得

		if (pPict != NULL)	//ヌルチェ
		{//なんかある
			pPict->UnsetTarget();	//ターゲット解除
		}
	}

	//タクシーターゲット解除
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPictTaxi* pPict = CPictTaxi::GetPict(cnt);	//オブジェクト取得

		if (pPict != NULL)	//ヌルチェ
		{//なんかある
			pPict->UnsetTarget();	//ターゲット解除
		}
	}

	//モーション破棄
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	m_apPict[m_nID] = NULL;
	for (int cnt = 0; cnt < PICT_MODEL_NUM; cnt++)
	{//一つずつ消す
		m_apModel[cnt]->Uninit();
		delete m_apModel[cnt];
		m_apModel[cnt] = NULL;
	}

	//影消す
	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
	}

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CPict::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//キーボード取得
	D3DXVECTOR3 targetPos = VEC3_ZERO;
	float targetWidthHalf = FLOAT_ZERO;
	float targetDepthHalf = FLOAT_ZERO;
	D3DXVECTOR3 pos = m_pos;
	CMotion* pMotion = GetMotion();

	//ピクトさん全員知ってるアジトへの帰還
	if (m_state == STATE_LEAVE)
	{
		if (CPict::IsControll() == false)
		{
			m_move.x = FLOAT_ZERO;
			m_move.z = FLOAT_ZERO;
			targetPos = m_pAgitObj->GetPos();
			targetWidthHalf = m_pAgitObj->GetWidth() * 0.5f;
			targetDepthHalf = m_pAgitObj->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf - PICT_AGIT_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICT_AGIT_STOP_LENGTH < pos.x ||
				targetPos.z - targetDepthHalf - PICT_AGIT_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICT_AGIT_STOP_LENGTH < pos.z)
			{//移動中
				float fTargetLenWidth, fTargetLenDepth;
				float fTargetRot;

				fTargetLenWidth = targetPos.x - pos.x;
				fTargetLenDepth = targetPos.z - pos.z;

				fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

				m_move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
				m_move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

				m_rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				if (pMotion->GetType() != MOTIONTYPE_MOVE)
				{
					pMotion->Set(MOTIONTYPE_MOVE);
				}
			}
			else
			{//ついた
				Uninit();
				return;
			}
		}
	}

	//ジャンプカウンタ増やす
	m_nCounterJumpTime++;

	//モデル設定
	for (int cnt = 0; cnt < PICT_MODEL_NUM; cnt++)
	{
		m_apModel[cnt]->Update();
	}
	//モーションがある
	if (m_pMotion != NULL)
	{
		//モーション更新
		m_pMotion->Update();
	}

	//当たり判定
	pos.x += m_move.x;
	//CollisionBlockX(&pos);

	pos.y += m_move.y - (ACCELERATION_GRAVITY * m_nCounterJumpTime / MAX_FPS);

	if (CollisionBlockY(&pos) == true)
	{
		m_bJump = false;

		//ジャンプ
		if (pKeyboard->GetRepeate(DIK_J))
		{//ジャンプ処理
			m_bJump = true;
			m_nCounterJumpTime = 0;
			m_move.y = 5.0f;
		}
	}

	pos.z += m_move.z;
	//CollisionBlockZ(&pos);

	m_pos = pos;

#if 0 
	if (pKeyboard->GetRepeate(DIK_SPACE) == true)
	{
		CBulletBillboard::Create(m_pos, m_rot, 16.0f, 16.0f, 10.0f, CObject::TYPE_PLAYER);
	}

	//追従設定
	if (pKeyboard->GetTrigger(DIK_C) == true)
	{
		CManager::GetPlayer()->Chace(m_nID);
	}
	else if (pKeyboard->GetTrigger(DIK_U) == true)
	{
		CManager::GetPlayer()->Unchace();
	}
#endif

	if (m_fRedAlpha >= FLOAT_ZERO)
	{//まだ赤い
	 //赤色具合を減らす
		m_fRedAlpha -= PICT_DAMAGE_ALPHA / PICT_DAMAGE_TIME;

		if (m_fRedAlpha < FLOAT_ZERO)
		{//赤くなくなった
			m_fRedAlpha = FLOAT_ZERO;
		}
	}

	//影設定
	m_pShadow->Set(m_pos, m_rot);
	CManager::GetDebProc()->Print("pos = (x = %f, y = %f, z = %f)\n", m_pos.x, m_pos.y, m_pos.z);
}

//========================
//描画処理
//========================
void CPict::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetTexture();						//テクスチャオブジェクト取得
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;							//計算用
	D3DMATERIAL9 matDef;												//現在のマテリアル保存用

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	//モデル取得
	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//拡縮を反映
	//D3DXMatrixScaling(&mtxScall, FENCE_SCALE, FENCE_SCALE, FENCE_SCALE);
	//D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxScall);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデル描画
	for (int cnt = 0; cnt < PICT_MODEL_NUM; cnt++)
	{
		m_apModel[cnt]->SetMainColor(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f));		//後々可変式に変更
		m_apModel[cnt]->SetSubColor(D3DXCOLOR(m_fRedAlpha, 0.0f, 0.0f, 0.0f));
		m_apModel[cnt]->Draw();
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//ダメージ付与処理
//========================
void CPict::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//付与

	//0になったら消す
	if (m_nLife <= INT_ZERO)
	{
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//全オブジェクト見る
			CPictBlocker* pPict = CPictBlocker::GetPict(cnt);	//オブジェクト取得

			if (pPict != NULL)	//ヌルチェ
			{//なんかある
				if (pPict->GetTarget() == this)
				{//自分がターゲット
					pPict->UnsetTarget();	//ターゲット外す
				}
			}
		}

		//爆散
		Uninit();
	}

	m_fRedAlpha = PICT_DAMAGE_ALPHA;
}

#if 0
//=================================
//ブロックとの衝突判定(X)
//=================================
void CPict::CollisionBlockX(D3DXVECTOR3* pPosNew)
{
	float fPlayerWidth = GetWidth() * 0.5f, fPlayerHeight = GetHeight() * 0.5f, fPlayerDepth = GetDepth() * 0.5f;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//オブジェクト取得

		if (pObj != NULL)	//ヌルチェ
		{//なんかある
			CObject::TYPE type = pObj->GetType();	//種類取得

			if (type == TYPE_BLOCK)
			{//ブロック
				float fOtherWidth = pObj->GetWidth() * 0.5f, fOtherHeight = pObj->GetHeight() * 0.5f, fOtherDepth = pObj->GetDepth() * 0.5f;
				D3DXVECTOR3 otherPos = pObj->GetPos();
				if (pPosNew->y - fPlayerHeight < otherPos.y + fOtherHeight &&
					pPosNew->y + fPlayerHeight > otherPos.y - fOtherHeight && 
					pPosNew->z - fPlayerDepth < otherPos.z + fOtherDepth &&
					pPosNew->z + fPlayerDepth > otherPos.z - fOtherDepth)
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
#endif

//=================================
//ブロックとの衝突判定(Y)
//=================================
bool CPict::CollisionBlockY(D3DXVECTOR3* pPosNew)
{
	float fPlayerWidth = GetWidth() * 0.5f, fPlayerHeight = GetHeight() * 0.5f, fPlayerDepth = GetDepth() * 0.5f;
	bool bLand = false;

#if 0
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//オブジェクト取得

		if (pObj != NULL)	//ヌルチェ
		{//なんかある
			CObject::TYPE type = pObj->GetType();	//種類取得

			if (type == TYPE_BLOCK)
			{//ブロック
				float fOtherWidth = pObj->GetWidth() * 0.5f, fOtherHeight = pObj->GetHeight() * 0.5f, fOtherDepth = pObj->GetDepth() * 0.5f;
				D3DXVECTOR3 otherPos = pObj->GetPos();
				if (pPosNew->x - fPlayerWidth < otherPos.x + fOtherWidth &&
					pPosNew->x + fPlayerWidth > otherPos.x - fOtherWidth &&
					pPosNew->z - fPlayerDepth < otherPos.z + fOtherDepth &&
					pPosNew->z + fPlayerDepth > otherPos.z - fOtherDepth)
				{
					if (GetPos().y + fPlayerHeight <= otherPos.y - fOtherHeight &&
						pPosNew->y + fPlayerHeight > otherPos.y - fOtherHeight)
					{
						pPosNew->y = otherPos.y - fOtherHeight - fPlayerHeight;
						m_move.y = 0.0f;
						m_nCounterJumpTime = 0;
						bLand = true;
					}
					else if (GetPos().y - fPlayerHeight >= otherPos.y + fOtherHeight &&
						pPosNew->y - fPlayerHeight < otherPos.y + fOtherHeight)
					{
						pPosNew->y = otherPos.y + fOtherHeight + fPlayerHeight;
						m_move.x = 0.0f;
						m_nCounterJumpTime = 0;
					}
				}
			}
		}
	}
#endif

	//高さ取得
	float fLandHeight = CManager::GetMeshField()->GetHeight(*pPosNew);
	if (pPosNew->y < fLandHeight)
	{
		pPosNew->y = fLandHeight;
		bLand = true;
		m_move.y = 0.0f;
		m_nCounterJumpTime = 0;
	}

	return bLand;
}

#if 0
//=================================
//ブロックとの衝突判定(Z)
//=================================
void CPict::CollisionBlockZ(D3DXVECTOR3* pPosNew)
{
	float fPlayerWidth = GetWidth() * 0.5f, fPlayerHeight = GetHeight() * 0.5f, fPlayerDepth = GetDepth() * 0.5f;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//オブジェクト取得

		if (pObj != NULL)	//ヌルチェ
		{//なんかある
			CObject::TYPE type = pObj->GetType();	//種類取得

			if (type == TYPE_BLOCK)
			{//ブロック
				float fOtherWidth = pObj->GetWidth() * 0.5f, fOtherHeight = pObj->GetHeight() * 0.5f, fOtherDepth = pObj->GetDepth() * 0.5f;
				D3DXVECTOR3 otherPos = pObj->GetPos();
				if (pPosNew->x - fPlayerWidth < otherPos.x + fOtherWidth &&
					pPosNew->x + fPlayerWidth > otherPos.x - fOtherWidth &&
					pPosNew->y - fPlayerHeight < otherPos.y + fOtherHeight &&
					pPosNew->y + fPlayerHeight > otherPos.y - fOtherHeight
					)
				{
					if (GetPos().z + fPlayerDepth <= otherPos.z - fOtherDepth &&
						pPosNew->z + fPlayerDepth > otherPos.z - fOtherDepth)
					{
						pPosNew->z = otherPos.z - fOtherDepth - fPlayerDepth;
						m_move.z = 0.0f;
					}
					else if (GetPos().z - fPlayerDepth >= otherPos.z + fOtherDepth &&
						pPosNew->z - fPlayerDepth < otherPos.z + fOtherDepth)
					{
						pPosNew->z = otherPos.z + fOtherDepth + fPlayerDepth;
						m_move.z = 0.0f;
					}
				}
			}
		}
	}
}
#endif

//=================================
//ピクトさんの操縦
//=================================
void CPict::Controll(D3DXVECTOR3 move)
{
	if (m_bControll == true)
	{//操縦可能
		m_move = move;
	}
}

//=================================
//パラメータ読み込み
//=================================
void CPict::LoadPictParam(const char * pPath)
{
}

//******************************************************
//デストロイヤーピクトクラス
//******************************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CPictDestroyer::CPictDestroyer()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_pTargetBuilding = NULL;
	m_nCounterDestruction = INT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictDestroyer::CPictDestroyer(const D3DXVECTOR3 pos) : CPict(pos)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_pTargetBuilding = NULL;
	m_nCounterDestruction = INT_ZERO;
}

//=================================
//デストラクタ
//=================================
CPictDestroyer::~CPictDestroyer()
{
}

//========================
//初期化処理
//========================
HRESULT CPictDestroyer::Init(void)
{
	//親処理
	CPict::Init();

	return S_OK;
}

//========================
//終了処理
//========================
void CPictDestroyer::Uninit(void)
{
	m_apPict[m_nID] = NULL;

	//親処理
	CPict::Uninit();
}

//========================
//更新処理
//========================
void CPictDestroyer::Update(void)
{
	D3DXVECTOR3 targetPos = VEC3_ZERO;
	float targetWidthHalf = FLOAT_ZERO;
	float targetDepthHalf = FLOAT_ZERO;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	CMotion* pMotion = GetMotion();

	//XZのみ消す
	move.x = FLOAT_ZERO;
	move.z = FLOAT_ZERO;

	if (m_pTargetBuilding != NULL)
	{
		if (CPict::IsControll() == false)
		{
			move.x = FLOAT_ZERO;
			move.z = FLOAT_ZERO;
			targetPos = m_pTargetBuilding->GetPos();
			targetWidthHalf = m_pTargetBuilding->GetWidth() * 0.5f;
			targetDepthHalf = m_pTargetBuilding->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf - PICT_BUIDING_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICT_BUIDING_STOP_LENGTH < pos.x ||
				targetPos.z - targetDepthHalf - PICT_BUIDING_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICT_BUIDING_STOP_LENGTH < pos.z)
			{//移動中
				float fTargetLenWidth, fTargetLenDepth;
				float fTargetRot;

				fTargetLenWidth = targetPos.x - pos.x;
				fTargetLenDepth = targetPos.z - pos.z;

				fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

				move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
				move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

				rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				if (pMotion->GetType() != MOTIONTYPE_MOVE)
				{
					pMotion->Set(MOTIONTYPE_MOVE);
				}

				//破壊カウンターリセット
				m_nCounterDestruction = INT_ZERO;
			}
			else
			{//ついた
				//攻撃状態にする
				SetState(STATE_ATTACK);

				m_nCounterDestruction++;

				if (m_nCounterDestruction > PICT_ATTACK_TIME)
				{
					//弾発射
					CBulletBillboard::Create(GetPos(), D3DXVECTOR3(-0.3f * D3DX_PI,0.0f,0.0f), 10.0f, 10.0f, 3.0f, 1000, CObject::TYPE_PICT, this);

					//破壊カウンターリセット
					m_nCounterDestruction = INT_ZERO;
				}

				if (pMotion->GetType() != MOTIONTYPE_DESTROY)
				{
					pMotion->Set(MOTIONTYPE_DESTROY);
				}
			}
		}
	}

	//値設定
	SetRot(rot);
	SetMove(move);

	//親処理
	CPict::Update();
}

//========================
//描画処理
//========================
void CPictDestroyer::Draw(void)
{
	//親処理
	CPict::Draw();
}

//========================
//生成処理
//========================
CPictDestroyer* CPictDestroyer::Create(const D3DXVECTOR3 pos)
{
	CPictDestroyer* pPict = NULL;

	if (pPict == NULL)
	{
		//ピクトの生成
		pPict = new CPictDestroyer(pos);

		//初期化
		pPict->Init();

		return pPict;
	}
	else
	{
		return NULL;
	}
}

//=================================
//ターゲット解除（と帰宅）
//=================================
void CPictDestroyer::UnsetTarget(void)
{
	m_pTargetBuilding = NULL;
	SetState(STATE_LEAVE);
}

//=================================
//タクシー乗車
//=================================
void CPictDestroyer::TakeTaxi(CPictTaxi * taxi)
{
	taxi->SetTakeTaxi(CPict::TYPE_DESTROYER, 1);
	//ここに連れている一般人も乗せる処理
}

//******************************************************
//ブロッカーピクトクラス
//******************************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CPictBlocker::CPictBlocker()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nCounterAttack = INT_ZERO;
	m_pTargetPolice = NULL;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictBlocker::CPictBlocker(const D3DXVECTOR3 pos) : CPict(pos)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nCounterAttack = INT_ZERO;
	m_pTargetPolice = NULL;
}

//=================================
//デストラクタ
//=================================
CPictBlocker::~CPictBlocker()
{
}

//========================
//初期化処理
//========================
HRESULT CPictBlocker::Init(void)
{
	//親処理
	CPict::Init();

	return S_OK;
}

//========================
//終了処理
//========================
void CPictBlocker::Uninit(void)
{
	m_apPict[m_nID] = NULL;

	//親処理
	CPict::Uninit();
}

//========================
//更新処理
//========================
void CPictBlocker::Update(void)
{
	D3DXVECTOR3 targetPos = VEC3_ZERO;
	float targetWidthHalf = FLOAT_ZERO;
	float targetDepthHalf = FLOAT_ZERO;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	CMotion* pMotion = GetMotion();

	if (m_pTargetPolice != NULL)
	{
		if (CPict::IsControll() == false)
		{
			move.x = FLOAT_ZERO;
			move.z = FLOAT_ZERO;
			targetPos = m_pTargetPolice->GetPos();
			targetWidthHalf = m_pTargetPolice->GetWidth() * 0.5f;
			targetDepthHalf = m_pTargetPolice->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf - PICT_POLICE_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICT_POLICE_STOP_LENGTH < pos.x ||
				targetPos.z - targetDepthHalf - PICT_POLICE_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICT_POLICE_STOP_LENGTH < pos.z)
			{
				float fTargetLenWidth, fTargetLenDepth;
				float fTargetRot;

				fTargetLenWidth = targetPos.x - pos.x;
				fTargetLenDepth = targetPos.z - pos.z;

				fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

				move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
				move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

				rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				if (pMotion->GetType() != MOTIONTYPE_MOVE)
				{
					pMotion->Set(MOTIONTYPE_MOVE);
				}

				//攻撃カウンターリセット
				m_nCounterAttack = INT_ZERO;
			}
			else
			{
				m_nCounterAttack++;
				if (m_nCounterAttack > PICT_ATTACK_TIME)
				{
					//弾発射
					CBulletBillboard::Create(GetPos() + D3DXVECTOR3(0.0f,10.0f,0.0f), GetRot() + D3DXVECTOR3(0.0f,D3DX_PI,0.0f), 10.0f, 10.0f, 10.0f, 100, TYPE_PICT, this);

					//攻撃カウンターリセット
					m_nCounterAttack = INT_ZERO;
				}

				if (pMotion->GetType() != MOTIONTYPE_ATTACK)
				{
					pMotion->Set(MOTIONTYPE_ATTACK);
				}
			}
		}
	}

	//値設定
	SetRot(rot);
	SetMove(move);

	//親処理
	CPict::Update();
}

//========================
//描画処理
//========================
void CPictBlocker::Draw(void)
{
	//親処理
	CPict::Draw();
}

//========================
//生成処理
//========================
CPictBlocker* CPictBlocker::Create(const D3DXVECTOR3 pos)
{
	CPictBlocker* pPict = NULL;

	if (pPict == NULL)
	{
		//ピクトの生成
		pPict = new CPictBlocker(pos);

		//初期化
		pPict->Init();

		return pPict;
	}
	else
	{
		return NULL;
	}
}

//=================================
//ターゲット解除（と帰宅）
//=================================
void CPictBlocker::UnsetTarget(void)
{
	m_pTargetPolice = NULL;
	SetState(STATE_LEAVE);
}

//=================================
//タクシー乗車
//=================================
void CPictBlocker::TakeTaxi(CPictTaxi * taxi)
{
	taxi->SetTakeTaxi(CPict::TYPE_BLOCKER, 1);
	//ここに連れている一般人も乗せる処理
}

//******************************************************
//ピクタクシークラス
//******************************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CPictTaxi::CPictTaxi()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nTakeDestroyer = INT_ZERO;
	m_nTakeBlocker = INT_ZERO;
	m_nTakeNormal = INT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictTaxi::CPictTaxi(const D3DXVECTOR3 pos)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nTakeDestroyer = INT_ZERO;
	m_nTakeBlocker = INT_ZERO;
	m_nTakeNormal = INT_ZERO;
}

//=================================
//デストラクタ
//=================================
CPictTaxi::~CPictTaxi()
{
}

//========================
//初期化処理
//========================
HRESULT CPictTaxi::Init(void)
{
	//設定されていたモードを取得
	m_mode = (MODE)CManager::GetSlider()->GetSelectIdx();
	
	//親処理
	CPict::Init();

	return S_OK;
}

//========================
//終了処理
//========================
void CPictTaxi::Uninit(void)
{
	m_apPict[m_nID] = NULL;

	//親処理
	CPict::Uninit();
}

//========================
//更新処理
//========================
void CPictTaxi::Update(void)
{
	D3DXVECTOR3 targetPos = VEC3_ZERO;
	float targetWidthHalf = FLOAT_ZERO;
	float targetDepthHalf = FLOAT_ZERO;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	CMotion* pMotion = GetMotion();
	move.x = FLOAT_ZERO;
	move.z = FLOAT_ZERO;

	if (m_mode == MODE_SABO)
	{//サボり
		SetState(STATE_LEAVE);
	}
	else
	{//働く
		SetState(STATE_FACE);
	}

	//アジト帰還以外の処理
	if (CPict::IsControll() == false)
	{
		if (GetState() != STATE_LEAVE)
		{//帰還しない
			if (m_mode == MODE_PICK)
			{//収集
				//ターゲット外れていたら探索
				if (m_ptargetPict == NULL /* && pBulletObj == NULL*/)
				{
					SearchBullet();
					m_ptargetPict = SearchNormal();

					//ここに弾とピクトの距離測って比較する処理
				}

				if (m_ptargetPict != NULL)
				{
					targetPos = m_ptargetPict->GetPos();
					targetWidthHalf = m_ptargetPict->GetWidth() * 0.5f;
					targetDepthHalf = m_ptargetPict->GetDepth() * 0.5f;

					if (targetPos.x - targetWidthHalf - PICT_POLICE_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICT_POLICE_STOP_LENGTH < pos.x ||
						targetPos.z - targetDepthHalf - PICT_POLICE_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICT_POLICE_STOP_LENGTH < pos.z)
					{
						float fTargetLenWidth, fTargetLenDepth;
						float fTargetRot;

						fTargetLenWidth = targetPos.x - pos.x;
						fTargetLenDepth = targetPos.z - pos.z;

						fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

						move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
						move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

						rot.y = FIX_ROT(fTargetRot + D3DX_PI);

						if (pMotion->GetType() != MOTIONTYPE_MOVE)
						{
							pMotion->Set(MOTIONTYPE_MOVE);
						}
					}
					else
					{//到着
						m_ptargetPict->TakeTaxi(this);
						m_ptargetPict->Uninit();
						m_ptargetPict = NULL;
					}
				}
			}
			else if (m_mode == MODE_RESCUE)
			{//救助
			 //ターゲット外れていたら探索
				if (m_ptargetPict == NULL)
				{
					m_ptargetPict = SearchBattler();
				}

				if (m_ptargetPict != NULL)
				{
					targetPos = m_ptargetPict->GetPos();
					targetWidthHalf = m_ptargetPict->GetWidth() * 0.5f;
					targetDepthHalf = m_ptargetPict->GetDepth() * 0.5f;

					if (targetPos.x - targetWidthHalf - PICT_POLICE_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICT_POLICE_STOP_LENGTH < pos.x ||
						targetPos.z - targetDepthHalf - PICT_POLICE_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICT_POLICE_STOP_LENGTH < pos.z)
					{
						float fTargetLenWidth, fTargetLenDepth;
						float fTargetRot;

						fTargetLenWidth = targetPos.x - pos.x;
						fTargetLenDepth = targetPos.z - pos.z;

						fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

						move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
						move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

						rot.y = FIX_ROT(fTargetRot + D3DX_PI);

						if (pMotion->GetType() != MOTIONTYPE_MOVE)
						{
							pMotion->Set(MOTIONTYPE_MOVE);
						}
					}
					else
					{//到着
						m_ptargetPict->TakeTaxi(this);
						m_ptargetPict->Uninit();
						m_ptargetPict = NULL;
					}
				}
			}
		}
	}

	//値設定
	SetRot(rot);
	SetMove(move);

	//親処理
	CPict::Update();
}

//========================
//描画処理
//========================
void CPictTaxi::Draw(void)
{
	//親処理
	CPict::Draw();
}

//========================
//生成処理
//========================
CPictTaxi* CPictTaxi::Create(const D3DXVECTOR3 pos)
{
	CPictTaxi* pPict = NULL;

	if (pPict == NULL)
	{
		//ピクトの生成
		pPict = new CPictTaxi(pos);

		//初期化
		pPict->Init();

		return pPict;
	}
	else
	{
		return NULL;
	}
}

//========================
//タクシー乗車処理
//========================
void CPictTaxi::SetTakeTaxi(const CPict::TYPE type, const int nTakeNum)
{
	switch (type)
	{
	case TYPE_DESTROYER:	//デストロイヤー
		m_nTakeDestroyer += nTakeNum;
		break;
	case TYPE_BLOCKER:		//ブロッカー
		m_nTakeBlocker += nTakeNum;
		break;
	case TYPE_NORMAL:		//一般人
		m_nTakeNormal += nTakeNum;
		break;
	}

	if (m_nTakeDestroyer + m_nTakeBlocker >= PICT_FORCEDRETURN_NUM)
	{//強制帰宅する
		SetState(STATE_LEAVE);
	}
}

//========================
//弾探索
//========================
void CPictTaxi::SearchBullet(void)
{
}

//========================
//一般人ピクト探索
//========================
CPictNormal* CPictTaxi::SearchNormal(void)
{
	CPictNormal* pPictNear = NULL;
	float fNearLength;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPictNormal* pPict = CPictNormal::GetPict(cnt);	//オブジェクト取得

		if (pPict != NULL)	//ヌルチェ
		{//なんかある
			float fLength = D3DXVec3Length(&(pPict->GetPos() - this->GetPos()));

			if (pPictNear == NULL || fLength < fNearLength)
			{//近いかそもそも1体しか知らん
				fNearLength = fLength;
				pPictNear = pPict;
			}
		}
	}

	//近いピクトのポインタ返す
	return pPictNear;
}

//========================
//戦闘要員ピクト探索
//========================
CPict* CPictTaxi::SearchBattler(void)
{
	CPictDestroyer* pPictD = NULL;
	CPictBlocker* pPictB = NULL;
	int nLifeD,nLifeB;

	//デストロイヤー探索
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPictDestroyer* pPict = CPictDestroyer::GetPict(cnt);	//オブジェクト取得

		if (pPict != NULL)	//ヌルチェ
		{//なんかある
			int nLife = pPict->GetLife();

			if ((((float)nLife / PICT_LIFE) <= PICT_RESCUE_LIFE) && (pPictD == NULL || nLifeD > nLife))
			{//救助対象でありなおかつ体力が一番少ない
				pPictD = pPict;
				nLifeD = nLife;
			}
		}
	}

	//ブロッカー探索
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPictBlocker* pPict = CPictBlocker::GetPict(cnt);	//オブジェクト取得

		if (pPict != NULL)	//ヌルチェ
		{//なんかある
			int nLife = pPict->GetLife();

			if ((((float)nLife / PICT_LIFE) <= PICT_RESCUE_LIFE) && (pPictB == NULL || nLifeB > nLife))
			{//救助対象でありなおかつ体力が一番少ない
				pPictB = pPict;
				nLifeB = nLife;
			}
		}
	}

	//そもそもいるかどうか
	if (pPictD != NULL && pPictB != NULL)
	{//両方いる
		//体力少ないほうを返す
		if (nLifeD <= nLifeB)
		{//どっちも体力同じならデストロイヤー優先
			return pPictD;
		}
		else
		{//ブロッカー
			return pPictB;
		}
	}
	else if(pPictD == NULL && pPictB != NULL)
	{//ブロッカーしかいない
		return pPictB;
	}
	else if (pPictD != NULL && pPictB == NULL)
	{//デストロイヤーしかいない
		return pPictD;
	}
	else
	{//いない
		return NULL;
	}
}


//******************************************************
//一般人ピクトクラス
//******************************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CPictNormal::CPictNormal()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictNormal::CPictNormal(const D3DXVECTOR3 pos) : CPict(pos)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
}

//=================================
//デストラクタ
//=================================
CPictNormal::~CPictNormal()
{
}

//========================
//初期化処理
//========================
HRESULT CPictNormal::Init(void)
{
	//親処理
	CPict::Init();

	return S_OK;
}

//========================
//終了処理
//========================
void CPictNormal::Uninit(void)
{
	m_apPict[m_nID] = NULL;

	//親処理
	CPict::Uninit();
}

//========================
//更新処理
//========================
void CPictNormal::Update(void)
{
	//親処理
	CPict::Update();
}

//========================
//描画処理
//========================
void CPictNormal::Draw(void)
{
	//親処理
	CPict::Draw();
}

//========================
//生成処理
//========================
CPictNormal* CPictNormal::Create(const D3DXVECTOR3 pos)
{
	CPictNormal* pPict = NULL;

	if (pPict == NULL)
	{
		//ピクトの生成
		pPict = new CPictNormal(pos);

		//初期化
		pPict->Init();

		return pPict;
	}
	else
	{
		return NULL;
	}
}

//========================
//タクシーに乗る処理
//========================
void CPictNormal::TakeTaxi(CPictTaxi* taxi)
{
	//抽選
	int nRand = rand() % (PICT_NORMAL_D_PERCENT + PICT_NORMAL_B_PERCENT + PICT_NORMAL_N_PERCENT);

	//分岐
	if (nRand < PICT_NORMAL_D_PERCENT)
	{//デストロイヤー
		taxi->SetTakeTaxi(CPict::TYPE_DESTROYER, 1);
	}
	else if (nRand < PICT_NORMAL_D_PERCENT + PICT_NORMAL_B_PERCENT)
	{//ブロッカー
		taxi->SetTakeTaxi(CPict::TYPE_BLOCKER, 1);
	}
	else
	{//一般人ピクト
		int nPictNum = rand() % (PICT_NORMAL_NUM_DEGREE + 1) + PICT_NORMAL_NUM_MIN;	//乱数で人数決まる
		taxi->SetTakeTaxi(CPict::TYPE_NORMAL, nPictNum);
	}
}

//******************************************************
//ピクト警察クラス
//******************************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CPictPolice::CPictPolice()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_pTargetBuilding = NULL;
	m_nCounterAttack = INT_ZERO;
	m_pTargetPict = NULL;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictPolice::CPictPolice(const D3DXVECTOR3 pos) : CPict(pos)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPict[cnt] == NULL)
		{//空っぽ
			m_apPict[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_pTargetBuilding = NULL;
	m_nCounterAttack = INT_ZERO;
	m_pTargetPict = NULL;
}

//=================================
//デストラクタ
//=================================
CPictPolice::~CPictPolice()
{
}

//========================
//初期化処理
//========================
HRESULT CPictPolice::Init(void)
{
	//親処理
	CPict::Init();

	return S_OK;
}

//========================
//終了処理
//========================
void CPictPolice::Uninit(void)
{
	m_apPict[m_nID] = NULL;

	//親処理
	CPict::Uninit();
}

//========================
//更新処理
//========================
void CPictPolice::Update(void)
{
	D3DXVECTOR3 targetPos = VEC3_ZERO;
	float targetWidthHalf = FLOAT_ZERO;
	float targetDepthHalf = FLOAT_ZERO;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	CMotion* pMotion = GetMotion();

	if (CPict::IsControll() == false)
	{
		if (m_pTargetPict != NULL)
		{
			move.x = FLOAT_ZERO;
			move.z = FLOAT_ZERO;
			targetPos = m_pTargetPict->GetPos();
			targetWidthHalf = m_pTargetPict->GetWidth() * 0.5f;
			targetDepthHalf = m_pTargetPict->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf - PICT_POLICE_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICT_POLICE_STOP_LENGTH < pos.x ||
				targetPos.z - targetDepthHalf - PICT_POLICE_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICT_POLICE_STOP_LENGTH < pos.z)
			{
				float fTargetLenWidth, fTargetLenDepth;
				float fTargetRot;

				fTargetLenWidth = targetPos.x - pos.x;
				fTargetLenDepth = targetPos.z - pos.z;

				fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

				move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
				move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

				rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				if (pMotion->GetType() != MOTIONTYPE_MOVE)
				{
					pMotion->Set(MOTIONTYPE_MOVE);
				}

				//攻撃カウンターリセット
				m_nCounterAttack = INT_ZERO;
			}
			else
			{
				//向かせる
				float fTargetLenWidth, fTargetLenDepth;
				fTargetLenWidth = targetPos.x - pos.x;
				fTargetLenDepth = targetPos.z - pos.z;

				float fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
				rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				m_nCounterAttack++;
				if (m_nCounterAttack > PICT_ATTACK_TIME)
				{
					//攻撃
					m_pTargetPict->AddDamage(100);

					//攻撃カウンターリセット
					m_nCounterAttack = INT_ZERO;
				}

				if (pMotion->GetType() != MOTIONTYPE_ATTACK)
				{
					pMotion->Set(MOTIONTYPE_ATTACK);
				}
			}
		}
		else
		{
			if (m_pTargetBuilding != NULL)
			{
				move.x = FLOAT_ZERO;
				move.z = FLOAT_ZERO;
				targetPos = m_pTargetBuilding->GetPos();
				targetWidthHalf = m_pTargetBuilding->GetWidth() * 0.5f;
				targetDepthHalf = m_pTargetBuilding->GetDepth() * 0.5f;

				if (targetPos.x - targetWidthHalf - PICT_POLICE_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICT_POLICE_STOP_LENGTH < pos.x ||
					targetPos.z - targetDepthHalf - PICT_POLICE_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICT_POLICE_STOP_LENGTH < pos.z)
				{
					float fTargetLenWidth, fTargetLenDepth;
					float fTargetRot;

					fTargetLenWidth = targetPos.x - pos.x;
					fTargetLenDepth = targetPos.z - pos.z;

					fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

					move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
					move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

					rot.y = FIX_ROT(fTargetRot + D3DX_PI);

					if (pMotion->GetType() != MOTIONTYPE_MOVE)
					{
						pMotion->Set(MOTIONTYPE_MOVE);
					}

					//攻撃カウンターリセット
					m_nCounterAttack = INT_ZERO;
				}
				else if (pMotion->GetType() != MOTIONTYPE_NEUTRAL)
				{
					pMotion->Set(MOTIONTYPE_NEUTRAL);
				}
			}

			for (int cnt = 0; cnt < MAX_OBJ; cnt++)
			{//全オブジェクト見る
				CPictBlocker* pPict = CPictBlocker::GetPict(cnt);	//オブジェクト取得

				if (pPict != NULL)
				{
					if (D3DXVec3Length(&(pPict->GetPos() - this->GetPos())) <= PICT_POLICE_SEARCH_LENGTH)
					{
						m_pTargetPict = pPict;
					}
				}
			}
		}
	}

	//値設定
	SetRot(rot);
	SetMove(move);

	//親処理
	CPict::Update();
}

//========================
//描画処理
//========================
void CPictPolice::Draw(void)
{
	//親処理
	CPict::Draw();
}

//========================
//生成処理
//========================
CPictPolice* CPictPolice::Create(const D3DXVECTOR3 pos)
{
	CPictPolice* pPict = NULL;

	if (pPict == NULL)
	{
		//ピクトの生成
		pPict = new CPictPolice(pos);

		//初期化
		pPict->Init();

		return pPict;
	}
	else
	{
		return NULL;
	}
}