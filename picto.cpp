//======================================================
//
//ピクトさん処理[picto.cpp]
//Author:石原颯馬
//
//======================================================
#include "picto.h"
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
#define PICTO_WALK_SPEED				(6.0f)		//ピクトさんの歩行速度
#define PICTO_POINT_RESEARCH_LENGTH	(7.0f)		//ピクトさんがポイントに到着したことにする距離
#define PICTO_AGIT_STOP_LENGTH		(20.0f)		//ピクトさんがアジトから離れる距離
#define PICTO_BUIDING_STOP_LENGTH	(120.0f)	//ピクトさんが建物から離れる距離
#define PICTO_POLICE_STOP_LENGTH		(30.0f)		//ピクトさんが警察から離れる距離
#define PICTO_POLICE_SEARCH_LENGTH	(60.0f)		//ピクト警察のサーチ範囲
#define PICTO_ATTACK_TIME			(60)		//攻撃を行う間隔
#define PICTO_DAMAGE_TIME			(120)		//赤くする時間
#define PICTO_LIFE					(1000)		//体力
#define PICTO_SOCIAL_DISTANCE		(15.0f)		//ソーシャルディスタンス範囲

#define PICTO_FORCEDRETURN_NUM		(2)			//強制帰宅するまでの人数

//計算
#define PICTO_POWER(lv,hpct)	((int)ceil((50 + (50 * ((float)hpct / PICTO_HAVENPICTO(lv)) * 1.2f) + hpct) * PICTO_ATK(lv)))

//静的メンバ変数
CPicto* CPicto::m_apPicto[MAX_OBJ];
int CPicto::m_nNumAll = 0;
CObjectX* CPicto::m_pAgitObj = nullptr;
const float CPicto::LOOSE_LENGTH = 100.0f;

CPictoDestroyer* CPictoDestroyer::m_apPicto[MAX_OBJ];
int CPictoDestroyer::m_nNumAll = 0;
int CPictoDestroyer::m_nLv = 1;
int CPictoDestroyer::m_nExp = 0;

CPictoBlocker* CPictoBlocker::m_apPicto[MAX_OBJ];
int CPictoBlocker::m_nNumAll = 0;
int CPictoBlocker::m_nLv = 1;
int CPictoBlocker::m_nExp = 0;

CPictoTaxi* CPictoTaxi::m_apPicto[MAX_OBJ];
int CPictoTaxi::m_nNumAll = 0;
const float CPictoTaxi::RESCUE_LIFE = 0.5f;

CPictoNormal* CPictoNormal::m_apPicto[MAX_OBJ];
int CPictoNormal::m_nNumAll = 0;
const int CPictoNormal::NORMAL_D_PERCENT = 15;
const int CPictoNormal::NORMAL_B_PERCENT = 20;
const int CPictoNormal::NORMAL_N_PERCENT = 65;
const int CPictoNormal::NORMAL_NUM_MIN = 500;
const int CPictoNormal::NORMAL_NUM_DEGREE = 2500;

CPictoPolice* CPictoPolice::m_apPicto[MAX_OBJ];
int CPictoPolice::m_nNumAll = 0;

//******************************************************
//ただのピクトクラス
//******************************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CPicto::CPicto()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	//値クリア
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_pMotion = NULL;
	m_fWidth = CManager::FLOAT_ZERO;
	m_fHeight = CManager::FLOAT_ZERO;
	m_fDepth = CManager::FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_bControll = false;
	m_fRedAlpha = CManager::FLOAT_ZERO;
	m_state = STATE_MAX;
	m_type = TYPE_MAX;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPicto::CPicto(const D3DXVECTOR3 pos, const TYPE type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	//値クリア
	m_pos = pos;
	m_rot = CManager::VEC3_ZERO;
	m_pMotion = NULL;
	m_fWidth = CManager::FLOAT_ZERO;
	m_fHeight = CManager::FLOAT_ZERO;
	m_fDepth = CManager::FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_bControll = false;
	m_fRedAlpha = CManager::FLOAT_ZERO;
	m_state = STATE_MAX;
	m_type = type;
}

//=================================
//デストラクタ
//=================================
CPicto::~CPicto()
{
}

//========================
//初期化処理
//========================
HRESULT CPicto::Init(void)
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

	//できた
	return S_OK;
}

//========================
//終了処理
//========================
void CPicto::Uninit(void)
{
	//警察ターゲット解除
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPictoPolice* pPicto = CPictoPolice::GetPicto(cnt);	//オブジェクト取得

		if (pPicto != NULL)	//ヌルチェ
		{//なんかある
			pPicto->UnsetTarget();	//ターゲット解除
		}
	}

	//タクシーターゲット解除
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPictoTaxi* pPicto = CPictoTaxi::GetPicto(cnt);	//オブジェクト取得

		if (pPicto != NULL)	//ヌルチェ
		{//なんかある
			pPicto->UnsetTargetPicto();	//ターゲット解除
		}
	}

	//モーション破棄
	if (m_pMotion != NULL)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = NULL;
	}

	m_apPicto[m_nID] = NULL;
	for (int cnt = 0; cnt < PICTO_MODEL_NUM; cnt++)
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

	//人数減らす
	m_nNumAll--;

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CPicto::Update(void)
{
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();	//キーボード取得
	D3DXVECTOR3 pos = m_pos;
	CMotion* pMotion = GetMotion();

	//ピクト共通:ポイント移動処理
	if (CPicto::IsControll() == false && m_state != STATE_ATTACK)
	{
		if (m_targetObj != NULL)
		{//目的地がある
			D3DXVECTOR3 targetPos = m_targetObj->GetPos();
			float targetWidthHalf = m_targetObj->GetWidth() * 0.5f;
			float targetDepthHalf = m_targetObj->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf * 1.5f < pos.x && targetPos.x + targetWidthHalf * 1.5f > pos.x &&
				targetPos.z - targetDepthHalf * 1.5f < pos.z && targetPos.z + targetDepthHalf * 1.5f > pos.z)
			{//ついた
				m_move.x = CManager::FLOAT_ZERO;
				m_move.z = CManager::FLOAT_ZERO;
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
			else
			{//目的地ではない
				//ポイント移動
				if (m_PointPos.x - PICTO_POINT_RESEARCH_LENGTH < pos.x && m_PointPos.x + PICTO_POINT_RESEARCH_LENGTH > pos.x &&
					m_PointPos.z - PICTO_POINT_RESEARCH_LENGTH < pos.z && m_PointPos.z + PICTO_POINT_RESEARCH_LENGTH > pos.z)
				{//ついた
					if (targetPos.x - targetWidthHalf * 2.5f < pos.x && targetPos.x + targetWidthHalf * 2.5f > pos.x &&
						targetPos.z - targetDepthHalf * 2.5f < pos.z && targetPos.z + targetDepthHalf * 2.5f > pos.z)
					{//建物が近い
						m_PointPos = targetPos;
					}
					else
					{//まだ遠い
						Search();	//ポイント検索
					}
				}
				else
				{//移動中
					float fTargetLenWidth, fTargetLenDepth;
					float fTargetRot;
					m_move.x = CManager::FLOAT_ZERO;
					m_move.z = CManager::FLOAT_ZERO;

					fTargetLenWidth = m_PointPos.x - pos.x;
					fTargetLenDepth = m_PointPos.z - pos.z;

					fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

					m_move.x = sinf(fTargetRot) * PICTO_WALK_SPEED;
					m_move.z = cosf(fTargetRot) * PICTO_WALK_SPEED;

					m_rot.y = FIX_ROT(fTargetRot + D3DX_PI);

					if (pMotion->GetType() != MOTIONTYPE_MOVE)
					{
						pMotion->Set(MOTIONTYPE_MOVE);
					}
				}
			}
		}

		//ソーシャルディスタンス処理
		float fLengthNear = CManager::FLOAT_ZERO;
		D3DXVECTOR3 posAfter = pos + m_move;
		CPicto* pPictoNear = nullptr;

		//近いの調べる
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{
			if (m_apPicto[cnt] != nullptr && m_apPicto[cnt] != this)
			{//リストにある
				float fLength = D3DXVec3Length(&(m_apPicto[cnt]->GetPos() - posAfter));
				if (pPictoNear == nullptr || fLengthNear > fLength)
				{//なんも見てない
					pPictoNear = m_apPicto[cnt];
					fLengthNear = fLength;
				}
			}
		}

		//近すぎない？
		if (pPictoNear != nullptr && fLengthNear <= PICTO_SOCIAL_DISTANCE)
		{//密です
			float fTargetLenWidth, fTargetLenDepth;
			float fTargetRot;
			D3DXVECTOR3 posNearPicto = pPictoNear->GetPos();

			//距離測って離れる
			fTargetLenWidth = posNearPicto.x - posAfter.x;
			fTargetLenDepth = posNearPicto.z - posAfter.z;

			fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
			fTargetRot = FIX_ROT(fTargetRot + (1.1f * D3DX_PI));

			posAfter.x = posNearPicto.x + (sinf(fTargetRot) * PICTO_SOCIAL_DISTANCE * 2.0f);
			posAfter.z = posNearPicto.z + (cosf(fTargetRot) * PICTO_SOCIAL_DISTANCE * 2.0f);

			//差分を移動量にする
			m_move = posAfter - m_pos;
		}
	}

	//ジャンプカウンタ増やす
	m_nCounterJumpTime++;

	//モデル設定
	for (int cnt = 0; cnt < PICTO_MODEL_NUM; cnt++)
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

	if (m_fRedAlpha >= CManager::FLOAT_ZERO)
	{//まだ赤い
	 //赤色具合を減らす
		m_fRedAlpha -= PICTO_DAMAGE_ALPHA / PICTO_DAMAGE_TIME;

		if (m_fRedAlpha < CManager::FLOAT_ZERO)
		{//赤くなくなった
			m_fRedAlpha = CManager::FLOAT_ZERO;
		}
	}

	//影設定
	m_pShadow->Set(m_pos, m_rot);
}

//========================
//描画処理
//========================
void CPicto::Draw(void)
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
	for (int cnt = 0; cnt < PICTO_MODEL_NUM; cnt++)
	{
		m_apModel[cnt]->SetMainColor(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f));		//後々可変式に変更
		m_apModel[cnt]->SetSubColor(D3DXCOLOR(m_fRedAlpha, 0.0f, 0.0f, 0.0f));
		m_apModel[cnt]->Draw();
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=================================
//ターゲット設定
//=================================
void CPicto::SetTargetObj(CObject * pObj)
{
	m_targetObj = pObj;		//目的地設定
	Search();				//経路探索
}

//=================================
//ターゲット解除
//=================================
void CPicto::UnsetTargetObj(void)
{
	m_targetObj = GetAgit();	//目的地をアジトにする
	m_state = STATE_LEAVE;		//帰る状態
	Search();					//経路探索
}

//=================================
//フィールドの当たり判定
//=================================
bool CPicto::CollisionField(D3DXVECTOR3* pPosNew)
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
void CPicto::Controll(D3DXVECTOR3 move)
{
	if (m_bControll == true)
	{//操縦可能
		m_move = move;
	}
}

//=================================
//パラメータ読み込み
//=================================
void CPicto::LoadPictoParam(const char * pPath)
{
}

//=================================
//ポイント検索処理
//=================================
void CPicto::Search(void)
{
	D3DXVECTOR3 posTarget = m_targetObj->GetPos();
	CPoint* pPointNear = NULL;
	float fLenNear = 0.0f;
	float fRadNear = 0.0f;
	CPoint* pPoint = CPoint::GetTop();

	//現在地と建物までの角度を計算
	float fTargetLenWidth = posTarget.x - m_pos.x;
	float fTargetLenDepth = posTarget.z - m_pos.z;
	float fRadiusBuilding = atan2f(fTargetLenWidth, fTargetLenDepth);

	while (pPoint != NULL)
	{//リスト終了までやる
		D3DXVECTOR3 vecPoint = pPoint->GetPos() - m_pos;

		bool bCollision = false;
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{
			CBuilding* pBuilding = CBuilding::GetBuilding(cnt);
			if (pBuilding != NULL)
			{
				D3DXVECTOR3 posBuilding = pBuilding->GetPos();
				float fWidthHalf = pBuilding->GetWidth() * 0.5f;
				float fDepthHalf = pBuilding->GetDepth() * 0.5f;

				//4頂点作る
				D3DXVECTOR3 posBuild[4];	
				posBuild[0] = posBuilding + D3DXVECTOR3(-fWidthHalf, 0.0f, -fDepthHalf);
				posBuild[1] = posBuilding + D3DXVECTOR3(fWidthHalf, 0.0f, -fDepthHalf);
				posBuild[2] = posBuilding + D3DXVECTOR3(fWidthHalf, 0.0f, fDepthHalf);
				posBuild[3] = posBuilding + D3DXVECTOR3(-fWidthHalf, 0.0f, fDepthHalf);

				//プラスマイナスがあったかのフラッグ
				bool bPlus = false;
				bool bMinus = false;	
				for (int cntPos = 0; cntPos < 4; cntPos++)
				{
					D3DXVECTOR3 vecLine = (posBuild[(cntPos + 1) % 4] - posBuild[cntPos]);
					D3DXVECTOR3 vecToPosOld = m_pos - posBuild[cntPos];
					D3DXVECTOR3 vecToPos = pPoint->GetPos() - posBuild[cntPos];
					if (TASUKIGAKE(vecLine.x, vecLine.z, vecToPosOld.x, vecToPosOld.z) >= 0.0f && TASUKIGAKE(vecLine.x, vecLine.z, vecToPos.x, vecToPos.z) < 0.0f)
					{//当たった
						float fAreaA = (vecToPos.z * vecPoint.x) - (vecToPos.x * vecPoint.z);
						float fAreaB = (vecLine.z * vecPoint.x) - (vecLine.x * vecPoint.z);
						if (fAreaA / fAreaB >= 0.0f && fAreaA / fAreaB <= 1.0f)
						{//ごっつん
							bPlus = true;
							bMinus = true;
							break;	//もう当たったので終了
						}
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
			float fLength = D3DXVec3Length(&vecPoint);	//ポイントと現在地の距離
			//ポイントと現在地の角度
			float fRadius = fabsf(acosf(D3DXVec3Dot(&vecPoint, &(posTarget - m_pos)) / (fLength * D3DXVec3Length(&(posTarget - m_pos)))));

			if(fLength > PICTO_POINT_RESEARCH_LENGTH + 2.0f)
			{//何も入っていない・角度が小さい
				if (pPointNear == nullptr)
				{
					pPointNear = pPoint;
					fLenNear = fLength;
					fRadNear = fRadius;
				}
				else if (fRadius < 0.5f * D3DX_PI && fRadius < fRadNear)
				{
					pPointNear = pPoint;
					fLenNear = fLength;
					fRadNear = fRadius;
				}
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
CPictoDestroyer::CPictoDestroyer()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nCounterDestruction = CManager::INT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictoDestroyer::CPictoDestroyer(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nCounterDestruction = CManager::INT_ZERO;
}

//=================================
//デストラクタ
//=================================
CPictoDestroyer::~CPictoDestroyer()
{
}

//========================
//初期化処理
//========================
HRESULT CPictoDestroyer::Init(void)
{
	//親処理
	CPicto::Init();

	//体力設定
	m_nLife = HAVE_LIFE(m_nLv);

	return S_OK;
}

//========================
//終了処理
//========================
void CPictoDestroyer::Uninit(void)
{
	m_apPicto[m_nID] = NULL;

	//親処理
	CPicto::Uninit();
}

//========================
//更新処理
//========================
void CPictoDestroyer::Update(void)
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

		if (m_nCounterDestruction > PICTO_ATTACK_TIME)
		{
			//弾発射
			CBulletBillboard::Create(GetPos() + D3DXVECTOR3(0.0f,30.0f,0.0f), rot + D3DXVECTOR3(-0.0f * D3DX_PI, 0.0f, 0.0f), 10.0f, 10.0f, 3.0f, PICTO_POWER(m_nLv,m_nHaveNormalPicto), TYPE_DESTROYER, this);

			//破壊カウンターリセット
			m_nCounterDestruction = CManager::INT_ZERO;
		}

		if (pMotion->GetType() != MOTIONTYPE_DESTROY)
		{
			pMotion->Set(MOTIONTYPE_DESTROY);
		}
	}
	else
	{
		//破壊カウンターリセット
		m_nCounterDestruction = CManager::INT_ZERO;
	}
	SetMove(CManager::VEC3_ZERO);
	SetRot(rot);

	//親処理
	CPicto::Update();
}

//========================
//描画処理
//========================
void CPictoDestroyer::Draw(void)
{
	//親処理
	CPicto::Draw();
}

//========================
//生成処理
//========================
CPictoDestroyer* CPictoDestroyer::Create(const D3DXVECTOR3 pos)
{
	CPictoDestroyer* pPicto = NULL;

	if (pPicto == NULL)
	{
		//ピクトの生成
		pPicto = new CPictoDestroyer(pos);

		//初期化
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return NULL;
	}
}

//=================================
//タクシー乗車
//=================================
void CPictoDestroyer::TakeTaxi(CPictoTaxi * taxi)
{
	taxi->SetTakeTaxi(CPicto::TYPE_DESTROYER, 1);
	//ここに連れている一般人も乗せる処理
}

//========================
//ダメージ付与処理
//========================
void CPictoDestroyer::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//付与

						//0になったら消す
	if (m_nLife <= CManager::INT_ZERO)
	{
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//全オブジェクト見る
			CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);	//オブジェクト取得

			if (pPicto != NULL)	//ヌルチェ
			{//なんかある
				if (pPicto->GetTargetObj() == this)
				{//自分がターゲット
					pPicto->UnsetTargetObj();	//ターゲット外す
				}
			}
		}

		//爆散
		Uninit();
	}

	//赤くする
	SetRedAlpha();
}


//=================================
//経験値取得（レベルアップ処理含む）
//=================================
void CPictoDestroyer::AddExp(const int nExp)
{
	//経験値加算
	m_nExp += nExp;

	//一定量超えたらレベルアップ
	while (REQUIRE_EXP(m_nLv + 1) <= m_nExp)
	{//上げきる
		m_nExp -= REQUIRE_EXP(m_nLv + 1);	//所持経験値減算
		m_nLv++;							//レベルアップ
	}
}

//******************************************************
//ブロッカーピクトクラス
//******************************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CPictoBlocker::CPictoBlocker()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nCounterAttack = CManager::INT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictoBlocker::CPictoBlocker(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nCounterAttack = CManager::INT_ZERO;
}

//=================================
//デストラクタ
//=================================
CPictoBlocker::~CPictoBlocker()
{
}

//========================
//初期化処理
//========================
HRESULT CPictoBlocker::Init(void)
{
	//親処理
	CPicto::Init();

	//体力設定
	m_nLife = HAVE_LIFE(m_nLv);

	return S_OK;
}

//========================
//終了処理
//========================
void CPictoBlocker::Uninit(void)
{
	m_apPicto[m_nID] = NULL;

	//親処理
	CPicto::Uninit();
}

//========================
//更新処理
//========================
void CPictoBlocker::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	CMotion* pMotion = GetMotion();

	if (GetState() == STATE_ATTACK)
	{
		D3DXVECTOR3 targetPos = GetTargetObj()->GetPos();
		float fTargetLenWidth, fTargetLenDepth;
		float fTargetRot;

		if (D3DXVec3Length(&(targetPos - pos)) > LOOSE_LENGTH)
		{//逃がす
			UnsetTargetObj();
			if (pMotion->GetType() != MOTIONTYPE_NEUTRAL)
			{
				pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
			SetState(STATE_LEAVE);
		}

		fTargetLenWidth = targetPos.x - pos.x;
		fTargetLenDepth = targetPos.z - pos.z;

		fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
		rot.y = FIX_ROT(fTargetRot + D3DX_PI);

		m_nCounterAttack++;
		if (m_nCounterAttack > PICTO_ATTACK_TIME)
		{
			//弾発射
			CBulletBillboard::Create(GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f), GetRot(), 10.0f, 10.0f, 10.0f, PICTO_POWER(m_nLv, m_nHaveNormalPicto), TYPE_BLOCKER, this);

			//攻撃カウンターリセット
			m_nCounterAttack = CManager::INT_ZERO;
		}

		if (pMotion->GetType() != MOTIONTYPE_ATTACK)
		{
			pMotion->Set(MOTIONTYPE_ATTACK);
		}
	}
	else
	{
		//攻撃カウンターリセット
		m_nCounterAttack = CManager::INT_ZERO;
	}
	SetMove(CManager::VEC3_ZERO);
	SetRot(rot);

	//親処理
	CPicto::Update();
}

//========================
//描画処理
//========================
void CPictoBlocker::Draw(void)
{
	//親処理
	CPicto::Draw();
}

//========================
//生成処理
//========================
CPictoBlocker* CPictoBlocker::Create(const D3DXVECTOR3 pos)
{
	CPictoBlocker* pPicto = NULL;

	if (pPicto == NULL)
	{
		//ピクトの生成
		pPicto = new CPictoBlocker(pos);

		//初期化
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return NULL;
	}
}

//=================================
//タクシー乗車
//=================================
void CPictoBlocker::TakeTaxi(CPictoTaxi * taxi)
{
	taxi->SetTakeTaxi(CPicto::TYPE_BLOCKER, 1);
	//ここに連れている一般人も乗せる処理
}

//========================
//ダメージ付与処理
//========================
void CPictoBlocker::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//付与

						//0になったら消す
	if (m_nLife <= CManager::INT_ZERO)
	{
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//全オブジェクト見る
			CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);	//オブジェクト取得

			if (pPicto != NULL)	//ヌルチェ
			{//なんかある
				if (pPicto->GetTargetObj() == this)
				{//自分がターゲット
					pPicto->UnsetTargetObj();	//ターゲット外す
				}
			}
		}

		//爆散
		Uninit();
	}

	//赤くする
	SetRedAlpha();
}

//=================================
//経験値取得（レベルアップ処理含む）
//=================================
void CPictoBlocker::AddExp(const int nExp)
{
	//経験値加算
	m_nExp += nExp;

	//一定量超えたらレベルアップ
	while (REQUIRE_EXP(m_nLv + 1) <= m_nExp)
	{//上げきる
		m_nExp -= REQUIRE_EXP(m_nLv + 1);	//所持経験値減算
		m_nLv++;							//レベルアップ
	}
}

//******************************************************
//ピクタクシークラス
//******************************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CPictoTaxi::CPictoTaxi()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nTakeDestroyer = CManager::INT_ZERO;
	m_nTakeBlocker = CManager::INT_ZERO;
	m_nTakeNormal = CManager::INT_ZERO;
	m_nHaveBullet = CManager::INT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictoTaxi::CPictoTaxi(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nTakeDestroyer = CManager::INT_ZERO;
	m_nTakeBlocker = CManager::INT_ZERO;
	m_nTakeNormal = CManager::INT_ZERO;
	m_nHaveBullet = CManager::INT_ZERO;
}

//=================================
//デストラクタ
//=================================
CPictoTaxi::~CPictoTaxi()
{
}

//========================
//初期化処理
//========================
HRESULT CPictoTaxi::Init(void)
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
	CPicto::Init();

	return S_OK;
}

//========================
//終了処理
//========================
void CPictoTaxi::Uninit(void)
{
	m_apPicto[m_nID] = NULL;

	//親処理
	CPicto::Uninit();
}

//========================
//更新処理
//========================
void CPictoTaxi::Update(void)
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
			if (m_ptargetPicto != NULL)
			{
				//タクシーに乗せる
				m_ptargetPicto->TakeTaxi(this);
				m_ptargetPicto->Uninit();
				m_ptargetPicto = NULL;

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
			if (m_ptargetPicto == NULL)
			{
				m_ptargetPicto = SearchBattler();
			}

			if (m_ptargetPicto != NULL)
			{
				//タクシーに乗せる
				m_ptargetPicto->TakeTaxi(this);
				m_ptargetPicto->Uninit();
				m_ptargetPicto = NULL;

				//ターゲット解除
				UnsetTargetObj();
			}
			break;
		}
	}

	//親処理
	CPicto::Update();
}

//========================
//描画処理
//========================
void CPictoTaxi::Draw(void)
{
	//親処理
	CPicto::Draw();
}

//========================
//生成処理
//========================
CPictoTaxi* CPictoTaxi::Create(const D3DXVECTOR3 pos)
{
	CPictoTaxi* pPicto = NULL;

	if (pPicto == NULL)
	{
		//ピクトの生成
		pPicto = new CPictoTaxi(pos);

		//初期化
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return NULL;
	}
}

//========================
//タクシー乗車処理
//========================
void CPictoTaxi::SetTakeTaxi(const CPicto::TYPE type, const int nTakeNum)
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

	if (m_nTakeDestroyer + m_nTakeBlocker >= PICTO_FORCEDRETURN_NUM)
	{//強制帰宅する
		SetState(STATE_LEAVE);
	}
}

//========================
//アイテム類探索
//========================
bool CPictoTaxi::SearchPick(void)
{
	//ターゲット外れていたら探索
	if (m_ptargetPicto == NULL && m_pItemBullet == NULL)
	{
		//取得
		m_ptargetPicto = SearchNormal();
		m_pItemBullet = SearchBullet();

		//ここに弾とピクトの距離測って比較する処理
		float fLengthPicto = -1.0f;
		float fLengthItem = -1.0f;

		if (m_ptargetPicto != NULL && m_pItemBullet != NULL)
		{//両方取得した
			fLengthPicto = D3DXVec3Length(&(m_ptargetPicto->GetPos() - this->GetPos()));
			fLengthItem = D3DXVec3Length(&(m_pItemBullet->GetPos() - this->GetPos()));

			//遠いほうをNULLにする
			if (fLengthPicto <= fLengthItem)
			{//ピクトのほうが近い（距離が同じの場合も含む）
				SetTargetObj(m_ptargetPicto);
				m_pItemBullet = NULL;
			}
			else
			{//弾のほうが近い
				SetTargetObj(m_pItemBullet);
				m_ptargetPicto = NULL;
			}
		}
		else if(m_ptargetPicto != NULL)
		{//ピクトだけ
			SetTargetObj(m_ptargetPicto);
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
CItemBullet* CPictoTaxi::SearchBullet(void)
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
CPictoNormal* CPictoTaxi::SearchNormal(void)
{
	CPictoNormal* pPictoNear = NULL;
	float fNearLength;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPictoNormal* pPicto = CPictoNormal::GetPicto(cnt);	//オブジェクト取得

		if (pPicto != NULL)	//ヌルチェ
		{//なんかある
			float fLength = D3DXVec3Length(&(pPicto->GetPos() - this->GetPos()));

			if (pPictoNear == NULL || fLength < fNearLength)
			{//近いかそもそも1体しか知らん
				fNearLength = fLength;
				pPictoNear = pPicto;
			}
		}
	}

	//近いピクトのポインタ返す
	return pPictoNear;
}

//========================
//戦闘要員ピクト探索
//========================
CPicto* CPictoTaxi::SearchBattler(void)
{
	CPictoDestroyer* pPictoD = NULL;
	CPictoBlocker* pPictoB = NULL;
	int nLifeD,nLifeB;

	//デストロイヤー探索
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPictoDestroyer* pPicto = CPictoDestroyer::GetPicto(cnt);	//オブジェクト取得

		if (pPicto != NULL)	//ヌルチェ
		{//なんかある
			int nLife = pPicto->GetLife();

			if ((((float)nLife / PICTO_LIFE) <= RESCUE_LIFE) && (pPictoD == NULL || nLifeD > nLife))
			{//救助対象でありなおかつ体力が一番少ない
				pPictoD = pPicto;
				nLifeD = nLife;
			}
		}
	}

	//ブロッカー探索
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);	//オブジェクト取得

		if (pPicto != NULL)	//ヌルチェ
		{//なんかある
			int nLife = pPicto->GetLife();

			if ((((float)nLife / PICTO_LIFE) <= RESCUE_LIFE) && (pPictoB == NULL || nLifeB > nLife))
			{//救助対象でありなおかつ体力が一番少ない
				pPictoB = pPicto;
				nLifeB = nLife;
			}
		}
	}

	//そもそもいるかどうか
	if (pPictoD != NULL && pPictoB != NULL)
	{//両方いる
		//体力少ないほうを返す
		if (nLifeD <= nLifeB)
		{//どっちも体力同じならデストロイヤー優先
			return pPictoD;
		}
		else
		{//ブロッカー
			return pPictoB;
		}
	}
	else if(pPictoD == NULL && pPictoB != NULL)
	{//ブロッカーしかいない
		return pPictoB;
	}
	else if (pPictoD != NULL && pPictoB == NULL)
	{//デストロイヤーしかいない
		return pPictoD;
	}
	else
	{//いない
		return NULL;
	}
}

//========================
//ダメージ付与処理
//========================
void CPictoTaxi::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//付与

	//0になったら消す
	if (m_nLife <= CManager::INT_ZERO)
	{
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//全オブジェクト見る
			CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);	//オブジェクト取得

			if (pPicto != NULL)	//ヌルチェ
			{//なんかある
				if (pPicto->GetTargetObj() == this)
				{//自分がターゲット
					pPicto->UnsetTargetObj();	//ターゲット外す
				}
			}
		}

		//爆散
		Uninit();
	}

	//赤くする
	SetRedAlpha();
}


//******************************************************
//一般人ピクトクラス
//******************************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CPictoNormal::CPictoNormal()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictoNormal::CPictoNormal(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
}

//=================================
//デストラクタ
//=================================
CPictoNormal::~CPictoNormal()
{
}

//========================
//初期化処理
//========================
HRESULT CPictoNormal::Init(void)
{
	//親処理
	CPicto::Init();

	return S_OK;
}

//========================
//終了処理
//========================
void CPictoNormal::Uninit(void)
{
	m_apPicto[m_nID] = NULL;

	//親処理
	CPicto::Uninit();
}

//========================
//更新処理
//========================
void CPictoNormal::Update(void)
{
	//親処理
	CPicto::Update();
}

//========================
//描画処理
//========================
void CPictoNormal::Draw(void)
{
	//親処理
	CPicto::Draw();
}

//========================
//生成処理
//========================
CPictoNormal* CPictoNormal::Create(const D3DXVECTOR3 pos)
{
	CPictoNormal* pPicto = NULL;

	if (pPicto == NULL)
	{
		//ピクトの生成
		pPicto = new CPictoNormal(pos);

		//初期化
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return NULL;
	}
}

//========================
//タクシーに乗る処理
//========================
void CPictoNormal::TakeTaxi(CPictoTaxi* taxi)
{
	//抽選
	int nRand = rand() % (NORMAL_D_PERCENT + NORMAL_B_PERCENT + NORMAL_N_PERCENT);

	//分岐
	if (nRand < NORMAL_D_PERCENT)
	{//デストロイヤー
		taxi->SetTakeTaxi(CPicto::TYPE_DESTROYER, 1);
	}
	else if (nRand < NORMAL_D_PERCENT + NORMAL_B_PERCENT)
	{//ブロッカー
		taxi->SetTakeTaxi(CPicto::TYPE_BLOCKER, 1);
	}
	else
	{//一般人ピクト
		int nPictoNum = rand() % (NORMAL_NUM_DEGREE + 1) + NORMAL_NUM_MIN;	//乱数で人数決まる
		taxi->SetTakeTaxi(CPicto::TYPE_NORMAL, nPictoNum);
	}
}

//******************************************************
//ピクト警察クラス
//******************************************************
//=================================
//コンストラクタ（デフォルト）
//=================================
CPictoPolice::CPictoPolice()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nCounterAttack = CManager::INT_ZERO;
	m_pTargetPicto = NULL;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPictoPolice::CPictoPolice(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == NULL)
		{//空っぽ
			m_apPicto[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nCounterAttack = CManager::INT_ZERO;
	m_pTargetPicto = NULL;
}

//=================================
//デストラクタ
//=================================
CPictoPolice::~CPictoPolice()
{
}

//========================
//初期化処理
//========================
HRESULT CPictoPolice::Init(void)
{
	//親処理
	CPicto::Init();

	//体力設定
	m_nLife = HAVE_LIFE(m_nLv);

	return S_OK;
}

//========================
//終了処理
//========================
void CPictoPolice::Uninit(void)
{
	m_apPicto[m_nID] = NULL;
	m_nNumAll--;

	//親処理
	CPicto::Uninit();
}

//========================
//更新処理
//========================
void CPictoPolice::Update(void)
{
	D3DXVECTOR3 targetPos = CManager::VEC3_ZERO;
	float targetWidthHalf = CManager::FLOAT_ZERO;
	float targetDepthHalf = CManager::FLOAT_ZERO;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	CMotion* pMotion = GetMotion();

	move.x = CManager::FLOAT_ZERO;
	move.z = CManager::FLOAT_ZERO;

	if (m_pTargetPicto != nullptr)
	{//狙いを定めている
		SetState(STATE_ATTACK);	//攻撃する気マンマン

		targetPos = m_pTargetPicto->GetPos();
		targetWidthHalf = m_pTargetPicto->GetWidth() * 0.5f;
		targetDepthHalf = m_pTargetPicto->GetDepth() * 0.5f;

		if (D3DXVec3Length(&(targetPos - pos)) > LOOSE_LENGTH)
		{//逃がす
			UnsetTarget();
			if (pMotion->GetType() != MOTIONTYPE_NEUTRAL)
			{
				pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
			SetState(STATE_FACE);
		}
		else if (targetPos.x - targetWidthHalf - PICTO_POLICE_STOP_LENGTH > pos.x || targetPos.x + targetWidthHalf + PICTO_POLICE_STOP_LENGTH < pos.x ||
			targetPos.z - targetDepthHalf - PICTO_POLICE_STOP_LENGTH > pos.z || targetPos.z + targetDepthHalf + PICTO_POLICE_STOP_LENGTH < pos.z)
		{//到着してない
			float fTargetLenWidth, fTargetLenDepth;
			float fTargetRot;

			fTargetLenWidth = targetPos.x - pos.x;
			fTargetLenDepth = targetPos.z - pos.z;

			fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

			move.x = sinf(fTargetRot) * PICTO_WALK_SPEED;
			move.z = cosf(fTargetRot) * PICTO_WALK_SPEED;

			rot.y = FIX_ROT(fTargetRot + D3DX_PI);

			if (pMotion->GetType() != MOTIONTYPE_MOVE)
			{
				pMotion->Set(MOTIONTYPE_MOVE);
			}

			//攻撃カウンターリセット
			m_nCounterAttack = CManager::INT_ZERO;
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
			if (m_nCounterAttack > PICTO_ATTACK_TIME)
			{
				//弾発射
				CBulletBillboard::Create(GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f), GetRot(), 10.0f, 10.0f, 10.0f, PICTO_POWER(m_nLv, m_nHaveNormalPicto), TYPE_POLICE, this);

				//攻撃カウンターリセット
				m_nCounterAttack = CManager::INT_ZERO;
			}

			if (pMotion->GetType() != MOTIONTYPE_ATTACK)
			{
				pMotion->Set(MOTIONTYPE_ATTACK);
			}
		}

		//位置移動量設定
		SetRot(rot);
		SetMove(move);
	}
	else
	{//見張りなう
		move.x = CManager::FLOAT_ZERO;
		move.z = CManager::FLOAT_ZERO;
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//全オブジェクト見る
			CPicto* pPicto = CPicto::GetPicto(cnt);	//オブジェクト取得

			if (pPicto != nullptr)
			{
				CPicto::TYPE type = pPicto->GetType();
				if (pPicto != this && type != TYPE_NORMAL && type != TYPE_POLICE)
				{
					if (D3DXVec3Length(&(pPicto->GetPos() - this->GetPos())) <= PICTO_POLICE_SEARCH_LENGTH)
					{
						m_pTargetPicto = pPicto;
					}
				}
			}
		}
		if (GetState() == STATE_ATTACK && pMotion->GetType() != MOTIONTYPE_NEUTRAL)
		{
			pMotion->Set(MOTIONTYPE_NEUTRAL);
		}
		//位置移動量設定
		SetRot(rot);
		SetMove(move);
	}

	//親処理
	CPicto::Update();
}

//========================
//描画処理
//========================
void CPictoPolice::Draw(void)
{
	//親処理
	CPicto::Draw();
}

//========================
//生成処理
//========================
CPictoPolice* CPictoPolice::Create(const D3DXVECTOR3 pos)
{
	CPictoPolice* pPicto = NULL;

	if (pPicto == NULL)
	{
		//ピクトの生成
		pPicto = new CPictoPolice(pos);

		//初期化
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return NULL;
	}
}

//========================
//ダメージ付与処理
//========================
void CPictoPolice::AddDamage(int nDamage)
{
	m_nLife -= nDamage;	//付与

	//0になったら消す
	if (m_nLife <= CManager::INT_ZERO)
	{
		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//全オブジェクト見る
			CPictoBlocker* pPicto = CPictoBlocker::GetPicto(cnt);	//オブジェクト取得

			if (pPicto != NULL)	//ヌルチェ
			{//なんかある
				if (pPicto->GetTargetObj() == this)
				{//自分がターゲット
					pPicto->UnsetTargetObj();	//ターゲット外す
				}
			}
		}

		//経験値付与
		CPictoBlocker::AddExp(DROP_EXP(9));	//いったんレベル9として扱う

		//爆散
		Uninit();
	}

	//赤くする
	SetRedAlpha();
}
