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
#include "game.h"
#include "renderer.h"
#include "input.h"	//仮
#include "camera.h"	//仮
#include "player.h"
#include "bullet.h"
#include "motion.h"
#include "building.h"
#include "shadow.h"
#include "meshField.h"
#include "slider.h"
#include "file.h"
#include "Culc.h"
#include "point.h"
#include "item.h"

//マクロ
#define PICT_WALK_SPEED				(6.0f)		//ピクトさんの歩行速度
#define PICT_POINT_RESEARCH_LENGTH	(2.0f)		//ピクトさんがポイントに到着したことにする距離
#define PICT_AGIT_STOP_LENGTH		(20.0f)		//ピクトさんがアジトから離れる距離
#define PICT_BUIDING_STOP_LENGTH	(120.0f)	//ピクトさんが建物から離れる距離
#define PICT_POLICE_STOP_LENGTH		(30.0f)		//ピクトさんが警察から離れる距離
#define PICT_POLICE_SEARCH_LENGTH	(60.0f)		//ピクト警察のサーチ範囲
#define PICT_ATTACK_TIME			(60)		//攻撃を行う間隔
#define PICT_DAMAGE_ALPHA			(0.9f)		//赤くする割合
#define PICT_DAMAGE_TIME			(120)		//赤くする時間
#define PICT_LIFE					(1000)		//体力
#define PICT_RESCUE_LIFE			(0.5f)		//救助する体力割合
#define PICT_SOCIAL_DISTANCE		(15.0f)		//ソーシャルディスタンス範囲

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
	m_type = TYPE_MAX;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPict::CPict(const D3DXVECTOR3 pos, const TYPE type)
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
	m_type = type;
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

	//状態設定
	m_state = STATE_FACE;

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
			pPict->UnsetTargetPict();	//ターゲット解除
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
	D3DXVECTOR3 pos = m_pos;
	CMotion* pMotion = GetMotion();
	m_move.x = FLOAT_ZERO;
	m_move.z = FLOAT_ZERO;

	//ピクト共通:ポイント移動処理
	if (CPict::IsControll() == false && m_state != STATE_ATTACK)
	{
		if (m_targetObj != NULL)
		{//目的地がある
			D3DXVECTOR3 targetPos = m_targetObj->GetPos();
			float targetWidthHalf = m_targetObj->GetWidth() * 0.5f;
			float targetDepthHalf = m_targetObj->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf * 1.5f > pos.x || targetPos.x + targetWidthHalf * 1.5f < pos.x ||
				targetPos.z - targetDepthHalf * 1.5f > pos.z || targetPos.z + targetDepthHalf * 1.5f < pos.z)
			{//目的地ではない
				//ポイント移動
				if (m_PointPos.x - PICT_POINT_RESEARCH_LENGTH > pos.x || m_PointPos.x + PICT_POINT_RESEARCH_LENGTH < pos.x ||
					m_PointPos.z - PICT_POINT_RESEARCH_LENGTH > pos.z || m_PointPos.z + PICT_POINT_RESEARCH_LENGTH < pos.z)
				{//移動中
					float fTargetLenWidth, fTargetLenDepth;
					float fTargetRot;

					fTargetLenWidth = m_PointPos.x - pos.x;
					fTargetLenDepth = m_PointPos.z - pos.z;

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
					Search();	//ポイント検索
				}
			}
			else
			{//ついた
				switch (m_state)
				{
				case STATE_FACE:
					m_state = STATE_ATTACK;
					break;
				case STATE_LEAVE:
					Uninit();
					return;
					break;
				}
			}
		}

		//ソーシャルディスタンス処理
		float fLengthNear = FLOAT_ZERO;
		D3DXVECTOR3 posAfter = pos + m_move;
		CPict* pPictNear = nullptr;

		//近いの調べる
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{
			if (m_apPict[cnt] != nullptr && m_apPict[cnt] != this)
			{//リストにある
				float fLength = D3DXVec3Length(&(m_apPict[cnt]->GetPos() - posAfter));
				if (pPictNear == nullptr || fLengthNear > fLength)
				{//なんも見てない
					pPictNear = m_apPict[cnt];
					fLengthNear = fLength;
				}
			}
		}

		//近すぎない？
		if (pPictNear != nullptr && fLengthNear <= PICT_SOCIAL_DISTANCE)
		{//密です
			float fTargetLenWidth, fTargetLenDepth;
			float fTargetRot;
			D3DXVECTOR3 posNearPict = pPictNear->GetPos();

			//距離測って離れる
			fTargetLenWidth = posNearPict.x - posAfter.x;
			fTargetLenDepth = posNearPict.z - posAfter.z;

			fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
			fTargetRot = FIX_ROT(fTargetRot + (1.1f * D3DX_PI));

			posAfter.x = posNearPict.x + (sinf(fTargetRot) * PICT_SOCIAL_DISTANCE * 2.0f);
			posAfter.z = posNearPict.z + (cosf(fTargetRot) * PICT_SOCIAL_DISTANCE * 2.0f);

			//差分を移動量にする
			m_move = posAfter - m_pos;
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
	pos.y += m_move.y - (ACCELERATION_GRAVITY * m_nCounterJumpTime / MAX_FPS);

	if (CollisionField(&pos) == true)
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
	m_pos = pos;

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
				if (pPict->GetTargetObj() == this)
				{//自分がターゲット
					pPict->UnsetTargetObj();	//ターゲット外す
				}
			}
		}

		//爆散
		Uninit();
	}

	m_fRedAlpha = PICT_DAMAGE_ALPHA;
}

//=================================
//ターゲット設定
//=================================
void CPict::SetTargetObj(CObject * pObj)
{
	m_targetObj = pObj;		//目的地設定
	Search();				//経路探索
}

//=================================
//ターゲット解除
//=================================
void CPict::UnsetTargetObj(void)
{
	m_targetObj = GetAgit();	//目的地をアジトにする
	m_state = STATE_LEAVE;		//帰る状態
	Search();					//経路探索
}

//=================================
//フィールドの当たり判定
//=================================
bool CPict::CollisionField(D3DXVECTOR3* pPosNew)
{
	bool bLand = false;

	//高さ取得
	float fLandHeight = CGame::GetMeshField()->GetHeight(*pPosNew);
	if (pPosNew->y < fLandHeight)
	{
		pPosNew->y = fLandHeight;
		bLand = true;
		m_move.y = 0.0f;
		m_nCounterJumpTime = 0;
	}

	return bLand;
}

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

//=================================
//ポイント検索処理
//=================================
void CPict::Search(void)
{
	D3DXVECTOR3 vecTarget = m_targetObj->GetPos() - m_pos;
	CPoint* pPointNear = NULL;
	float fLenNear = 0.0f;
	float fRadNear = 0.0f;
	CPoint* pPoint = CPoint::GetTop();

	while (pPoint != NULL)
	{//リスト終了までやる
		D3DXVECTOR3 vecPoint = pPoint->GetPos() - m_pos;
		float fLength = D3DXVec3Length(&vecPoint);	//ポイントと現在地の距離
		//ポイントと現在地の角度
		float fRadius = fabsf(acosf(D3DXVec3Dot(&vecTarget, &(pPoint->GetPos() - m_pos)) / (D3DXVec3Length(&vecTarget) * D3DXVec3Length(&(pPoint->GetPos() - m_pos)))));
		if (pPointNear == NULL && fLength > PICT_POINT_RESEARCH_LENGTH)
		{//何も入っていない
			pPointNear = pPoint;
			fLenNear = fLength;
			fRadNear = fRadius;
		}
		else if (fLength > PICT_POINT_RESEARCH_LENGTH && fRadNear > fRadius)
		{//角度が小さい
			bool bCollision = false;
			for (int cnt = 0; cnt < MAX_OBJ; cnt++)
			{
				CBuilding* pBuilding = CBuilding::GetBuilding(cnt);
				if (pBuilding != NULL)
				{
					D3DXVECTOR3 pos = pBuilding->GetPos();
					float fWidthHalf = pBuilding->GetWidth() * 0.5f;
					float fDepthHalf = pBuilding->GetDepth() * 0.5f;

					D3DXVECTOR3 posBuild[4];	//4頂点作る
					posBuild[0] = pos + D3DXVECTOR3(-fWidthHalf, 0.0f, -fDepthHalf);
					posBuild[1] = pos + D3DXVECTOR3(fWidthHalf, 0.0f, -fDepthHalf);
					posBuild[2] = pos + D3DXVECTOR3(-fWidthHalf, 0.0f, fDepthHalf);
					posBuild[3] = pos + D3DXVECTOR3(fWidthHalf, 0.0f, fDepthHalf);

					bool bPlus = false;
					bool bMinus = false;	//プラスマイナスがあったかのフラッグ
					for (int cntPos = 0; cntPos < 4; cntPos++)
					{
						if (TASUKIGAKE(vecPoint.x, vecPoint.z, posBuild[cntPos].x, posBuild[cntPos].z) > 0.0f)
						{//プラス
							bPlus = true;
						}
						else if (TASUKIGAKE(vecPoint.x, vecPoint.z, posBuild[cntPos].x, posBuild[cntPos].z) < 0.0f)
						{//マイナス
							bMinus = true;
						}
						else
						{//ゼロなので当たっている（まぁ両方trueにすれば当たったことになるからいいよね）
							bPlus = true;
							bMinus = true;
							break;	//もう当たったので終了
						}
					}

					if (bPlus == true && bMinus == true)
					{
						bCollision = true;	//衝突した
						break;
					}
				}
			}
			
			if (bCollision == false)
			{//当たってない
				pPointNear = pPoint;
				fLenNear = fLength;
				fRadNear = fRadius;
			}
		}
		
		//次のポイントへ
		pPoint = pPoint->GetNext();
	}

	m_PointPos = pPointNear->GetPos();	//新しい位置を入れる
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
	m_nCounterDestruction = INT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictDestroyer::CPictDestroyer(const D3DXVECTOR3 pos, const TYPE type) : CPict(pos,type)
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
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	CMotion* pMotion = GetMotion();

	if (GetState() == STATE_ATTACK)
	{
		D3DXVECTOR3 targetPos = GetTargetObj()->GetPos();
		float fTargetLenWidth, fTargetLenDepth;
		float fTargetRot;

		fTargetLenWidth = targetPos.x - pos.x;
		fTargetLenDepth = targetPos.z - pos.z;

		fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
		rot.y = FIX_ROT(fTargetRot + D3DX_PI);

		m_nCounterDestruction++;

		if (m_nCounterDestruction > PICT_ATTACK_TIME)
		{
			//弾発射
			CBulletBillboard::Create(GetPos(), rot + D3DXVECTOR3(-0.3f * D3DX_PI, 0.0f, 0.0f), 10.0f, 10.0f, 3.0f, 1000, TYPE_DESTROYER, this);

			//破壊カウンターリセット
			m_nCounterDestruction = INT_ZERO;
		}

		if (pMotion->GetType() != MOTIONTYPE_DESTROY)
		{
			pMotion->Set(MOTIONTYPE_DESTROY);
		}
	}
	else
	{
		//破壊カウンターリセット
		m_nCounterDestruction = INT_ZERO;
	}
	SetRot(rot);

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
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictBlocker::CPictBlocker(const D3DXVECTOR3 pos, const TYPE type) : CPict(pos,type)
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
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	CMotion* pMotion = GetMotion();

	if (GetState() == STATE_ATTACK)
	{
		D3DXVECTOR3 targetPos = GetTargetObj()->GetPos();
		float fTargetLenWidth, fTargetLenDepth;
		float fTargetRot;

		fTargetLenWidth = targetPos.x - pos.x;
		fTargetLenDepth = targetPos.z - pos.z;

		fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
		rot.y = FIX_ROT(fTargetRot + D3DX_PI);

		m_nCounterAttack++;
		if (m_nCounterAttack > PICT_ATTACK_TIME)
		{
			//弾発射
			CBulletBillboard::Create(GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f), GetRot(), 10.0f, 10.0f, 10.0f, 100, TYPE_BLOCKER, this);

			//攻撃カウンターリセット
			m_nCounterAttack = INT_ZERO;
		}

		if (pMotion->GetType() != MOTIONTYPE_ATTACK)
		{
			pMotion->Set(MOTIONTYPE_ATTACK);
		}
	}
	else
	{
		//攻撃カウンターリセット
		m_nCounterAttack = INT_ZERO;
	}
	SetRot(rot);

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
	m_nHaveBullet = INT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictTaxi::CPictTaxi(const D3DXVECTOR3 pos, const TYPE type) : CPict(pos,type)
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
	m_nHaveBullet = INT_ZERO;
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
	m_mode = (MODE)CGame::GetSlider()->GetSelectIdx();

	//モードによって変わるかも
	switch (m_mode)
	{
	case MODE_PICK:
		//アイテム類探す
		SearchPick();
		break;
	case MODE_RESCUE:
		break;
	}
	
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
	if (m_mode == MODE_SABO)
	{//サボり
		SetState(STATE_LEAVE);
		UnsetTargetObj();
	}

	if (GetState() == STATE_ATTACK)
	{//目的地到着
		switch (m_mode)
		{
		case MODE_PICK:
			if (m_ptargetPict != NULL)
			{
				//タクシーに乗せる
				m_ptargetPict->TakeTaxi(this);
				m_ptargetPict->Uninit();
				m_ptargetPict = NULL;

				//ターゲット解除
				UnsetTargetObj();
			}
			else if (m_pItemBullet != NULL)
			{
				//タクシーに乗せる
				m_pItemBullet->PickBullet(this);
				m_pItemBullet->Uninit();
				m_pItemBullet = NULL;

				//ターゲット解除
				UnsetTargetObj();
			}

			//次のアイテム類探す
			if (SearchPick() == false)
			{//なんもない
				if (GetMotion()->GetType() != MOTIONTYPE_NEUTRAL)
				{
					GetMotion()->Set(MOTIONTYPE_NEUTRAL);
				}
			}
			SetState(STATE_FACE);
			break;
		case MODE_RESCUE:
			//ターゲット外れていたら探索
			if (m_ptargetPict == NULL)
			{
				m_ptargetPict = SearchBattler();
			}

			if (m_ptargetPict != NULL)
			{
				//タクシーに乗せる
				m_ptargetPict->TakeTaxi(this);
				m_ptargetPict->Uninit();
				m_ptargetPict = NULL;

				//ターゲット解除
				UnsetTargetObj();
			}
			break;
		}
	}

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
//アイテム類探索
//========================
bool CPictTaxi::SearchPick(void)
{
	//ターゲット外れていたら探索
	if (m_ptargetPict == NULL && m_pItemBullet == NULL)
	{
		//取得
		m_ptargetPict = SearchNormal();
		m_pItemBullet = SearchBullet();

		//ここに弾とピクトの距離測って比較する処理
		float fLengthPict = -1.0f;
		float fLengthItem = -1.0f;

		if (m_ptargetPict != NULL && m_pItemBullet != NULL)
		{//両方取得した
			fLengthPict = D3DXVec3Length(&(m_ptargetPict->GetPos() - this->GetPos()));
			fLengthItem = D3DXVec3Length(&(m_pItemBullet->GetPos() - this->GetPos()));

			//遠いほうをNULLにする
			if (fLengthPict <= fLengthItem)
			{//ピクトのほうが近い（距離が同じの場合も含む）
				SetTargetObj(m_ptargetPict);
				m_pItemBullet = NULL;
			}
			else
			{//弾のほうが近い
				SetTargetObj(m_pItemBullet);
				m_ptargetPict = NULL;
			}
		}
		else if(m_ptargetPict != NULL)
		{//ピクトだけ
			SetTargetObj(m_ptargetPict);
		}
		else if (m_pItemBullet != NULL)
		{//弾だけ
			SetTargetObj(m_pItemBullet);
		}
		else
		{//無
			return false;
		}

		return true;
	}

	return false;
}

//========================
//弾探索
//========================
CItemBullet* CPictTaxi::SearchBullet(void)
{
	CItemBullet* pItemNear = NULL;
	float fNearLength;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CItemBullet* pItem = CItemBullet::GetItemBullet(cnt);	//オブジェクト取得

		if (pItem != NULL)	//ヌルチェ
		{//なんかある
			float fLength = D3DXVec3Length(&(pItem->GetPos() - this->GetPos()));

			if (pItemNear == NULL || fLength < fNearLength)
			{//近いかそもそも1つしか知らん
				fNearLength = fLength;
				pItemNear = pItem;
			}
		}
	}

	//近いピクトのポインタ返す
	return pItemNear;
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
CPictNormal::CPictNormal(const D3DXVECTOR3 pos, const TYPE type) : CPict(pos,type)
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
	m_nCounterAttack = INT_ZERO;
	m_pTargetPict = NULL;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictPolice::CPictPolice(const D3DXVECTOR3 pos, const TYPE type) : CPict(pos,type)
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

	move.x = FLOAT_ZERO;
	move.z = FLOAT_ZERO;

	if (GetState() == STATE_ATTACK)
	{
		if (m_pTargetPict != NULL)
		{//狙いを定めている
			targetPos = m_pTargetPict->GetPos();
			targetWidthHalf = m_pTargetPict->GetWidth() * 0.5f;
			targetDepthHalf = m_pTargetPict->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf - PICT_POLICE_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICT_POLICE_STOP_LENGTH < pos.x ||
				targetPos.z - targetDepthHalf - PICT_POLICE_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICT_POLICE_STOP_LENGTH < pos.z)
			{//到着してない
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
			{//ついた
				//向かせる
				float fTargetLenWidth, fTargetLenDepth;
				fTargetLenWidth = targetPos.x - pos.x;
				fTargetLenDepth = targetPos.z - pos.z;

				float fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
				rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				m_nCounterAttack++;
				if (m_nCounterAttack > PICT_ATTACK_TIME)
				{
					//弾発射
					CBulletBillboard::Create(GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f), GetRot(), 10.0f, 10.0f, 10.0f, 100, TYPE_POLICE, this);

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
		{//見張りなう
			for (int cnt = 0; cnt < MAX_OBJ; cnt++)
			{//全オブジェクト見る
				CPict* pPict = CPict::GetPict(cnt);	//オブジェクト取得

				if (pPict != NULL && pPict != this && GetType() != TYPE_NORMAL)
				{
					if (D3DXVec3Length(&(pPict->GetPos() - this->GetPos())) <= PICT_POLICE_SEARCH_LENGTH)
					{
						m_pTargetPict = pPict;
					}
				}
			}
			if (pMotion->GetType() != MOTIONTYPE_NEUTRAL)
			{
				pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
		}
	}
	else
	{
		//攻撃カウンターリセット
		m_nCounterAttack = INT_ZERO;
	}
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