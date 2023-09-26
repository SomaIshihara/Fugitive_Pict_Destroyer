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
#include "tutorial.h"
#include "renderer.h"
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
#include "havenum.h"
#include "level.h"
#include "koban.h"
#include "sound.h"
#include <vector>

//マクロ
#define PICTO_WALK_SPEED			(6.0f)		//ピクトさんの歩行速度
#define PICTO_POINT_RESEARCH_LENGTH	(30.0f)		//ピクトさんがポイントに到着したことにする距離
#define PICTO_AGIT_STOP_LENGTH		(20.0f)		//ピクトさんがアジトから離れる距離
#define PICTO_BUIDING_STOP_LENGTH	(120.0f)	//ピクトさんが建物から離れる距離
#define PICTO_POLICE_STOP_LENGTH	(55.0f)		//ピクトさんが警察から離れる距離
#define PICTO_POLICE_SEARCH_LENGTH	(100.0f)	//ピクト警察のサーチ範囲
#define PICTO_ATTACK_TIME			(60)		//攻撃を行う間隔
#define PICTO_DAMAGE_TIME			(120)		//赤くする時間
#define PICTO_LIFE					(1000)		//体力
#define PICTO_SOCIAL_DISTANCE		(10.0f)		//ソーシャルディスタンス範囲

#define PICTO_FORCEDRETURN_NUM		(2)			//強制帰宅するまでの人数

//計算
#define PICTO_POWER(lv,hpct)	((int)ceil((50 + (50 * ((float)hpct / PICTO_HAVENPICTO(lv)) * 1.2f) + hpct) * PICTO_ATK(lv)))

//静的メンバ変数
CPicto* CPicto::m_apPicto[MAX_OBJ];
int CPicto::m_nNumAll = 0;
CObjectX* CPicto::m_pAgitObj = nullptr;
const float CPicto::LOOSE_LENGTH = 150.0f;
const int CPicto::BASE_MODEL_NUM = 10;

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
		if (m_apPicto[cnt] == nullptr)
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
	m_pMotion = nullptr;
	m_fWidth = CManager::FLOAT_ZERO;
	m_fHeight = CManager::FLOAT_ZERO;
	m_fDepth = CManager::FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_fRedAlpha = CManager::FLOAT_ZERO;
	m_state = STATE_MAX;
	m_type = TYPE_MAX;
	m_bNear = false;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CPicto::CPicto(const D3DXVECTOR3 pos, const TYPE type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == nullptr)
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
	m_pMotion = nullptr;
	m_fWidth = CManager::FLOAT_ZERO;
	m_fHeight = CManager::FLOAT_ZERO;
	m_fDepth = CManager::FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_fRedAlpha = CManager::FLOAT_ZERO;
	m_state = STATE_MAX;
	m_type = type;
	m_bNear = false;
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

	//ピクトさんである
	SetType(CObject::TYPE_PICTO);

	//できた
	return S_OK;
}

//========================
//終了処理
//========================
void CPicto::Uninit(void)
{
	//モーション破棄
	if (m_pMotion != nullptr)
	{
		m_pMotion->Uninit();
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	m_apPicto[m_nID] = nullptr;
	if (m_ppModel != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumModel; cnt++)
		{//一つずつ消す
			if (m_ppModel[cnt] != nullptr)
			{
				m_ppModel[cnt]->Uninit();
				delete m_ppModel[cnt];
				m_ppModel[cnt] = nullptr;
			}
		}
		delete[] m_ppModel;	//配列そのものを破棄
	}

	//影消す
	if (m_pShadow != nullptr)
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
	if (m_state != STATE_ATTACK)
	{
		if (m_pTargetObj != nullptr)
		{//目的地がある
			D3DXVECTOR3 targetPos = m_pTargetObj->GetPos();
			float targetWidthHalf = m_pTargetObj->GetWidth() * 0.5f;
			float targetDepthHalf = m_pTargetObj->GetDepth() * 0.5f;

			if (targetPos.x - (targetWidthHalf + 50.0f) < pos.x && targetPos.x + (targetWidthHalf + 50.0f) > pos.x &&
				targetPos.z - (targetDepthHalf + 50.0f) < pos.z && targetPos.z + (targetDepthHalf + 50.0f) > pos.z)
			{//ついた
				m_move.x = CManager::FLOAT_ZERO;
				m_move.z = CManager::FLOAT_ZERO;
				switch (m_state)
				{
				case STATE_FACE:
					m_state = STATE_ATTACK;
					break;
				case STATE_LEAVE:
					Return();
					return;
					break;
				}
			}
			else
			{//目的地ではない
				D3DXVECTOR3 pointPos;
				if (m_bNear == false)
				{
					//ポイント移動
					pointPos = m_pPoint->GetPos();
					if (pointPos.x - PICTO_POINT_RESEARCH_LENGTH < pos.x && pointPos.x + PICTO_POINT_RESEARCH_LENGTH > pos.x &&
						pointPos.z - PICTO_POINT_RESEARCH_LENGTH < pos.z && pointPos.z + PICTO_POINT_RESEARCH_LENGTH > pos.z)
					{//ついた
						if (targetPos.x - (targetWidthHalf + 150.0f) < pos.x && targetPos.x + (targetWidthHalf + 150.0f) > pos.x &&
							targetPos.z - (targetDepthHalf + 150.0f) < pos.z && targetPos.z + (targetDepthHalf + 150.0f) > pos.z)
						{//近い
							pointPos = targetPos;
							m_bNear = true;
						}
						else
						{//まだ遠い
							if (m_stack.empty() == true)
							{//スタックが空
								Search(m_pTargetObj);

								if (m_stack.empty() == true)
								{//探索してなおスタックが空（とりあえず建物に近いこととする）
									pointPos = targetPos;
									m_bNear = true;
								}
							}
							else
							{//ポイントを取り出す
								m_pPoint = m_stack.top();
								m_stack.pop();
								pointPos = m_pPoint->GetPos();	//新しいポイントの位置取得
							}
						}
					}
				}
				else
				{
					pointPos = targetPos;
				}

				float fTargetLenWidth, fTargetLenDepth;
				float fTargetRot;
				m_move.x = CManager::FLOAT_ZERO;
				m_move.z = CManager::FLOAT_ZERO;

				fTargetLenWidth = pointPos.x - pos.x;
				fTargetLenDepth = pointPos.z - pos.z;

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

		//動いていない場合ソーシャルディスタンス処理
		if (D3DXVec3Length(&m_move) == 0.0f)
		{
			//ソーシャルディスタンス処理
			float fLengthNear = CManager::FLOAT_ZERO;
			D3DXVECTOR3 posAfter = pos;
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
	}

	//ジャンプカウンタ増やす
	m_nCounterJumpTime++;

	//モデル設定
	if (m_ppModel != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumModel; cnt++)
		{
			if (m_ppModel[cnt] != nullptr)
			{
				m_ppModel[cnt]->Update();
			}
		}
	}

	//モーションがある
	if (m_pMotion != nullptr)
	{
		//モーション更新
		m_pMotion->Update();
	}

	//当たり判定
	pos.x += m_move.x;
	pos.y += m_move.y - (ACCELERATION_GRAVITY * m_nCounterJumpTime / MAX_FPS);

	CollisionField(&pos);

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

	m_move.x = CManager::FLOAT_ZERO;
	m_move.z = CManager::FLOAT_ZERO;
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
	if (m_ppModel != nullptr)
	{
		for (int cnt = 0; cnt < m_nNumModel; cnt++)
		{
			if (m_ppModel[cnt] != nullptr)
			{
				m_ppModel[cnt]->SetMainColor(D3DXCOLOR(0.0f, 0.6f, 0.0f, 1.0f));		//後々可変式に変更
				m_ppModel[cnt]->SetSubColor(D3DXCOLOR(m_fRedAlpha, 0.0f, 0.0f, 0.0f));
				m_ppModel[cnt]->Draw();
			}
		}
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=================================
//ターゲット設定
//=================================
void CPicto::SetTargetObj(CObject * pObj)
{
	m_pTargetObj = pObj;		//目的地設定
	Search(m_pTargetObj);	//経路探索
}

//=================================
//ターゲット解除
//=================================
void CPicto::UnsetTarget(void)
{
	m_pTargetObj = GetAgit();	//目的地をアジトにする
	m_state = STATE_LEAVE;		//帰る状態
	Search(m_pTargetObj);		//経路探索
}

//=================================
//ターゲット解除（すべて）
//=================================
void CPicto::UnsetTargetAll(void)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPicto* pPicto = CPicto::GetPicto(cnt);	//オブジェクト取得

		if (pPicto != nullptr && pPicto->GetTargetObj() == this)
		{//自分がターゲット
			pPicto->UnsetTarget();	//ターゲット外す
		}
	}
}

//=================================
//モデル設定
//=================================
void CPicto::SetModel(const char * pPath)
{
	//モーション生成・初期化
	m_pMotion = new CMotion;
	m_pMotion->Init();

	//モーションビューアのファイルを読み込み
	LoadMotionViewerFile(pPath, &m_ppModel, m_pMotion, &m_nNumModel);

	if (m_ppModel != nullptr)
	{
		for (int cnt = 0; cnt < BASE_MODEL_NUM; cnt++)
		{//体の部分のみ行う
			m_ppModel[cnt]->SetChangeColor(true);
		}
	}

	//モーション設定
	m_pMotion->Set(0);
}

//=================================
//フィールドの当たり判定
//=================================
bool CPicto::CollisionField(D3DXVECTOR3* pPosNew)
{
	bool bLand = false;

	//高さ取得
	float fLandHeight = CManager::FLOAT_ZERO;
	CScene::MODE mode = CManager::GetMode();
	if (mode == CScene::MODE_GAME)
	{//ゲーム
		fLandHeight = CGame::GetMeshField()->GetHeight(*pPosNew);
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//チュートリアル
		fLandHeight = CTutorial::GetMeshField()->GetHeight(*pPosNew);
	}

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
//帰宅
//=================================
void CPicto::Return(void)
{
	UnsetTargetAll();

	Uninit();
}

//=================================
//ポイント検索処理
//=================================
void CPicto::Search(CObject* pTarget)
{
	int nPointNum = CPoint::GetNumAll();	//ポイント数
	Node* pNode = new Node[nPointNum];		//探索に使うノードをポイントの数だけ生成

	//ノード初期化
	CPoint* pPoint = CPoint::GetTop();
	for (int cnt = 0; cnt < nPointNum; cnt++)
	{
		pNode[cnt].bDesition = false;		//未確定
		pNode[cnt].fLengthMin = FLT_MAX;	//これを距離無限大とする
		pNode[cnt].pPoint = pPoint;			//ポイントを入れる
		pNode[cnt].nFromNum = -1;			//どこから来たかわからん
		pPoint = pPoint->GetNext();			//次入れる
	}

	//始点のノードを探す（プレイヤーの最寄り）
	int nNodeStart = -1;
	float fLengthNear = FLT_MAX;
	for (int cnt = 0; cnt < nPointNum; cnt++)
	{
		float fLength = D3DXVec3Length(&(m_pos - pNode[cnt].pPoint->GetPos()));
		if (fLengthNear > fLength)
		{//短い
			nNodeStart = cnt;
			fLengthNear = fLength;
		}
	}

	//終点のノードを探す（建物の最寄り）
	Node* nodeGoal = nullptr;
	float fLengthNearTarget = FLT_MAX;
	float fLengthNearThis = FLT_MAX;
	std::vector<Node*> vecNearNode;
	for (int cnt = 0; cnt < nPointNum; cnt++)
	{//全ノードを見て近いノードを候補にする
		if (pNode[cnt].pPoint != m_pPoint)
		{
			float fLength = D3DXVec3Length(&(pNode[cnt].pPoint->GetPos() - pTarget->GetPos()));
			if (fLength < fLengthNearTarget)
			{//近いの見つけた
				vecNearNode.clear();					//候補全部消す
				fLengthNearTarget = fLength;			//距離入れる
				vecNearNode.emplace_back(&pNode[cnt]);	//リストに加える
			}
			else if (fLength == fLengthNearTarget)
			{//同じの見つけた
				vecNearNode.emplace_back(&pNode[cnt]);	//リストに加える
			}
		}
	}
	//候補から一番近いのを終点にする
	for (int cnt = 0; cnt < vecNearNode.size(); cnt++)
	{
		float fLength = D3DXVec3Length(&(vecNearNode[cnt]->pPoint->GetPos() - this->GetPos()));
		if (fLength < fLengthNearThis)
		{//近いの見つけた
			nodeGoal = vecNearNode[cnt];	//ノード入れる
			fLengthNearTarget = fLength;	//距離入れる
		}
	}
	
	//そのノードの距離を0にする
	pNode[nNodeStart].fLengthMin = 0.0f;
	//確定とルート記録
	pNode[nNodeStart].bDesition = true;
	pNode[nNodeStart].nFromNum = -1;

	//つながっているノードとの距離を計算し、小さければ更新
	std::vector<CPoint*> vec = pNode[nNodeStart].pPoint->GetConnect();
	pPoint = pNode[nNodeStart].pPoint;
	for (int cntA = 0; cntA < vec.size(); cntA++)
	{
		//距離計算
		float fLength = D3DXVec3Length(&(pPoint->GetPos() - vec[cntA]->GetPos()));

		//探す
		for (int cntB = 0; cntB < nPointNum; cntB++)
		{
			if (pNode[cntB].pPoint == vec[cntA] && pNode[cntB].fLengthMin > fLength + pNode[nNodeStart].fLengthMin)
			{//探しているポイントが入っているノードを見つけた。あと距離も近い
				if (pNode[cntB].bDesition == false)
				{//そいつ確定済みじゃない
					pNode[cntB].fLengthMin = fLength + pNode[nNodeStart].fLengthMin;
					pNode[cntB].nFromNum = nNodeStart;
				}
				break;
			}
		}
	}

	//探索
	while (1)
	{
		//距離が最も小さいノードを確定
		int nNodeNear = -1;
		float fLengthNear = FLT_MAX;
		for (int cnt = 0; cnt < nPointNum; cnt++)
		{
			if (pNode[cnt].bDesition == false && pNode[cnt].fLengthMin < fLengthNear)
			{
				nNodeNear = cnt;
				fLengthNear = pNode[cnt].fLengthMin;
			}
		}
		
		//すべて確定済み
		if (nNodeNear == -1)
		{//nNodeNearが変化なし
			break;
		}

		//確定とルート記録
		pNode[nNodeNear].bDesition = true;
		vec = pNode[nNodeNear].pPoint->GetConnect();

		//つながっているノードとの距離を計算し、小さければ更新
		vec = pNode[nNodeNear].pPoint->GetConnect();
		CPoint* pPoint = pNode[nNodeNear].pPoint;
		for (int cntA = 0; cntA < vec.size(); cntA++)
		{
			//距離計算
			float fLength = D3DXVec3Length(&(pPoint->GetPos() - vec[cntA]->GetPos()));

			//探す
			for (int cntB = 0; cntB < nPointNum; cntB++)
			{
				if (pNode[cntB].pPoint == vec[cntA] && pNode[cntB].fLengthMin > fLength + pNode[nNodeNear].fLengthMin)
				{//探しているポイントが入っているノードを見つけた。あと距離も近い
					if (pNode[cntB].bDesition == false)
					{//そいつ確定済みじゃない
						pNode[cntB].fLengthMin = fLength + pNode[nNodeNear].fLengthMin;
						pNode[cntB].nFromNum = nNodeNear;
					}
					break;
				}
			}
		}
	}
	//ここで探索完了
	//経路を入れる
	while (1)
	{
		m_stack.emplace(nodeGoal->pPoint);

		if (nodeGoal->nFromNum == -1)
		{
			break;
		}

		nodeGoal = &pNode[nodeGoal->nFromNum];
	}

	//早速最初のポイントを取り出す
	m_pPoint = m_stack.top();
	m_stack.pop();
	m_bNear = false;
	
	delete[] pNode;		//ノード消す
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
		if (m_apPicto[cnt] == nullptr)
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
		if (m_apPicto[cnt] == nullptr)
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

	//モデル設定
	CPicto::SetModel("data\\motion_exithuman_dest.txt");

	//所持数減算処理
	CScene::MODE mode = CManager::GetMode();
	CHaveNum** ppHaveNumObj = nullptr;
	if (mode == CScene::MODE_GAME)
	{//ゲーム
		ppHaveNumObj = CGame::GetHaveNumObj();
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//チュートリアル
		ppHaveNumObj = CTutorial::GetHaveNumObj();
	}

	//リーダー減算
	ppHaveNumObj[TYPE_DESTROYER]->AddNum(-1);

	//一般人連れてく
	int nAgitNormal = ppHaveNumObj[TYPE_NORMAL]->GetHaveNum();
	int nMaxNormal = PICTO_HAVENPICTO(m_nLv);

	if (nAgitNormal >= nMaxNormal)
	{//現在のレベルで持てる一般人の人数以上
		ppHaveNumObj[TYPE_NORMAL]->AddNum(-nMaxNormal);
		m_nHaveNormalPicto = nMaxNormal;
	}
	else
	{//足りないけどとりあえずいるだけ連れてく
		ppHaveNumObj[TYPE_NORMAL]->AddNum(-nAgitNormal);
		m_nHaveNormalPicto = nAgitNormal;
	}

	//体力設定
	m_nLife = HAVE_LIFE(m_nLv);

	return S_OK;
}

//========================
//終了処理
//========================
void CPictoDestroyer::Uninit(void)
{
	m_apPicto[m_nID] = nullptr;
	m_nNumAll--;

	//親処理
	CPicto::Uninit();
}

//========================
//更新処理
//========================
void CPictoDestroyer::Update(void)
{
	CSound* pSound = CManager::GetSound();
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
			//建物への角度計算
			float fBuildingAngle = atan2f(GetTargetObj()->GetHeight(), D3DXVec3Length(&(targetPos - pos)));

			//弾発射
			CBulletBillboard::Create(GetPos() + D3DXVECTOR3(0.0f,30.0f,0.0f), rot + D3DXVECTOR3(-fBuildingAngle, 0.0f, 0.0f), 10.0f, 10.0f, 3.0f, PICTO_POWER(m_nLv,m_nHaveNormalPicto), TYPE_DESTROYER, this);
			pSound->Play(CSound::SOUND_LABEL_SE_ROCKET);

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
	CPictoDestroyer* pPicto = nullptr;

	if (pPicto == nullptr)
	{
		//ピクトの生成
		pPicto = new CPictoDestroyer(pos);

		//初期化
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//タクシー乗車
//=================================
void CPictoDestroyer::TakeTaxi(CPictoTaxi * taxi)
{
	taxi->SetTakeTaxi(CPicto::TYPE_DESTROYER, 1);
	taxi->SetTakeTaxi(CPicto::TYPE_NORMAL, m_nHaveNormalPicto);
	UnsetTargetAll();
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
		UnsetTargetAll();

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

		//画面に反映
		CScene::MODE mode = CManager::GetMode();
		if (mode == CScene::MODE_GAME)
		{//ゲーム
			CGame::GetLevelObj()[TYPE_DESTROYER]->SetNum(m_nLv);
		}
	}
}

//=================================
//帰宅
//=================================
void CPictoDestroyer::Return(void)
{
	CScene::MODE mode = CManager::GetMode();

	if (mode == CScene::MODE_GAME)
	{//ゲーム
		CGame::GetPlayer()->AddPicto(1, 0, m_nHaveNormalPicto);
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//チュートリアル
		CTutorial::GetPlayer()->AddPicto(1, 0, m_nHaveNormalPicto);
	}

	CPicto::Return();
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
		if (m_apPicto[cnt] == nullptr)
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
		if (m_apPicto[cnt] == nullptr)
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

	//モデル設定
	CPicto::SetModel("data\\motion_exithuman_block.txt");

	//所持数減算処理
	CScene::MODE mode = CManager::GetMode();
	CHaveNum** ppHaveNumObj = nullptr;
	if (mode == CScene::MODE_GAME)
	{//ゲーム
		ppHaveNumObj = CGame::GetHaveNumObj();
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//チュートリアル
		ppHaveNumObj = CTutorial::GetHaveNumObj();
	}

	//リーダー減算
	ppHaveNumObj[TYPE_BLOCKER]->AddNum(-1);

	//一般人連れてく
	int nAgitNormal = ppHaveNumObj[TYPE_NORMAL]->GetHaveNum();
	int nMaxNormal = PICTO_HAVENPICTO(m_nLv);

	if (nAgitNormal >= nMaxNormal)
	{//現在のレベルで持てる一般人の人数以上
		ppHaveNumObj[TYPE_NORMAL]->AddNum(-nMaxNormal);
		m_nHaveNormalPicto = nMaxNormal;
	}
	else
	{//足りないけどとりあえずいるだけ連れてく
		ppHaveNumObj[TYPE_NORMAL]->AddNum(-nAgitNormal);
		m_nHaveNormalPicto = nAgitNormal;
	}

	//体力設定
	m_nLife = HAVE_LIFE(m_nLv);

	return S_OK;
}

//========================
//終了処理
//========================
void CPictoBlocker::Uninit(void)
{
	m_apPicto[m_nID] = nullptr;
	m_nNumAll--;

	//親処理
	CPicto::Uninit();
}

//========================
//更新処理
//========================
void CPictoBlocker::Update(void)
{
	CSound* pSound = CManager::GetSound();
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
			UnsetTarget();
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
			pSound->Play(CSound::SOUND_LABEL_SE_GUN);

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
	CPictoBlocker* pPicto = nullptr;

	if (pPicto == nullptr)
	{
		//ピクトの生成
		pPicto = new CPictoBlocker(pos);

		//初期化
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return nullptr;
	}
}

//=================================
//タクシー乗車
//=================================
void CPictoBlocker::TakeTaxi(CPictoTaxi * taxi)
{
	taxi->SetTakeTaxi(CPicto::TYPE_BLOCKER, 1);
	taxi->SetTakeTaxi(CPicto::TYPE_NORMAL, m_nHaveNormalPicto);
	UnsetTargetAll();
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
		UnsetTargetAll();

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

		//画面に反映
		CScene::MODE mode = CManager::GetMode();
		if (mode == CScene::MODE_GAME)
		{//ゲーム
			CGame::GetLevelObj()[TYPE_BLOCKER]->SetNum(m_nLv);
		}
	}
}

//=================================
//帰宅
//=================================
void CPictoBlocker::Return(void)
{
	CScene::MODE mode = CManager::GetMode();

	if (mode == CScene::MODE_GAME)
	{//ゲーム
		CGame::GetPlayer()->AddPicto(0, 1, m_nHaveNormalPicto);
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//チュートリアル
		CTutorial::GetPlayer()->AddPicto(0, 1, m_nHaveNormalPicto);
	}

	CPicto::Return();
}

//******************************************************
//ピクタクシークラス
//******************************************************
//=================================
//コンストラクタ
//=================================
CPictoTaxi::CPictoTaxi(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == nullptr)
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
	//親処理
	CPicto::Init();

	//モデル設定
	CPicto::SetModel("data\\motion_exithuman_taxi.txt");

	//設定されていたモードを取得
	m_mode = (MODE)CManager::GetScene()->GetSlider()->GetSelectIdx();

	//モードによって変わるかも
	switch (m_mode)
	{
	case MODE_PICK:
		//アイテム類探す
		m_pTargetPicto = SearchNormal();
		break;
	case MODE_RESCUE:
		m_pTargetPicto = SearchBattler();
		break;
	}

	if (m_pTargetPicto == nullptr)
	{//なんもない
		if (GetMotion()->GetType() != MOTIONTYPE_NEUTRAL)
		{
			GetMotion()->Set(MOTIONTYPE_NEUTRAL);
		}
	}
	else
	{//見つけた
		SetState(STATE_FACE);
		SetTargetObj(m_pTargetPicto);
	}

	m_nLife = PICTO_TAXI_MAXLIFE;

	return S_OK;
}

//========================
//終了処理
//========================
void CPictoTaxi::Uninit(void)
{
	m_apPicto[m_nID] = nullptr;
	m_nNumAll--;

	//親処理
	CPicto::Uninit();
}

//========================
//更新処理
//========================
void CPictoTaxi::Update(void)
{
	if (m_mode == MODE_SABO && GetState() != STATE_LEAVE)
	{//サボり
		SetState(STATE_LEAVE);
		UnsetTarget();
	}
	
	//目的地到着時の処理
	if (GetState() == STATE_ATTACK && m_pTargetPicto != nullptr)
	{
		//タクシーに乗せる
		m_pTargetPicto->TakeTaxi(this);
		m_pTargetPicto->Uninit();
		m_pTargetPicto = nullptr;

		//ターゲット解除
		UnsetTarget();
	}

	//探索処理
	if (m_pTargetPicto == nullptr)
	{
		switch (m_mode)
		{
		case MODE_PICK:
			//探索
			m_pTargetPicto = SearchNormal();
			break;
		case MODE_RESCUE:
			//探索
			m_pTargetPicto = SearchBattler();
			break;
		}

		if (m_pTargetPicto == nullptr)
		{//なんもない
			if (GetMotion()->GetType() != MOTIONTYPE_NEUTRAL)
			{
				GetMotion()->Set(MOTIONTYPE_NEUTRAL);
			}
		}
		else
		{//見つけた
			SetState(STATE_FACE);
			SetTargetObj(m_pTargetPicto);
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
	CPictoTaxi* pPicto = nullptr;

	if (pPicto == nullptr)
	{
		//ピクトの生成
		pPicto = new CPictoTaxi(pos);

		//初期化
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return nullptr;
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
		UnsetTarget();
	}
}

//========================
//一般人ピクト探索
//========================
CPictoNormal* CPictoTaxi::SearchNormal(void)
{
	CPictoNormal* pPictoNear = nullptr;
	float fNearLength;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPictoNormal* pPicto = CPictoNormal::GetPicto(cnt);	//オブジェクト取得

		if (pPicto != nullptr)	//ヌルチェ
		{//なんかある
			float fLength = D3DXVec3Length(&(pPicto->GetPos() - this->GetPos()));

			if (pPictoNear == nullptr || fLength < fNearLength)
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
	CPicto* pPictoDamaged = nullptr;
	int nLifeDamaged = INT_MAX;

	//デストロイヤー・ブロッカー探索
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CPicto* pPicto = CPicto::GetPicto(cnt);	//オブジェクト取得

		if (pPicto != nullptr)	//ヌルチェ
		{//なんかある
			TYPE type = pPicto->GetType();
			if (type == TYPE_DESTROYER || type == TYPE_BLOCKER)
			{
				//レベル取得
				int nLv;
				if (type == TYPE_DESTROYER)
				{//デストロイヤー
					nLv = CPictoDestroyer::GetLv();
				}
				else
				{//ブロッカー
					nLv = CPictoBlocker::GetLv();
				}
				int nLife = pPicto->GetLife();

				if ((((float)nLife / HAVE_LIFE(nLv)) <= RESCUE_LIFE) && nLifeDamaged > nLife)
				{//救助対象でありなおかつ体力が一番少ない
					pPictoDamaged = pPicto;
					nLifeDamaged = nLife;
				}
			}
		}
	}

	//居るかいないかわからんけどあっちで何とかしてくれるから返す
	return pPictoDamaged;
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
		UnsetTargetAll();

		//爆散
		Uninit();
	}

	//赤くする
	SetRedAlpha();
}

//=================================
//帰宅
//=================================
void CPictoTaxi::Return(void)
{
	//戦闘要員追加処理
	CScene::MODE mode = CManager::GetMode();
	CPlayer* pPlayer = nullptr;
	if (mode == CScene::MODE_GAME)
	{//ゲーム
		pPlayer = CGame::GetPlayer();
	}
	else if (mode == CScene::MODE_TUTORIAL)
	{//チュートリアル
		pPlayer = CTutorial::GetPlayer();
	}
	pPlayer->AddPicto(m_nTakeDestroyer, m_nTakeBlocker, m_nTakeNormal);

	CPicto::Return();
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
		if (m_apPicto[cnt] == nullptr)
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
		if (m_apPicto[cnt] == nullptr)
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

	//モデル設定
	CPicto::SetModel("data\\motion_exithuman_nor.txt");

	return S_OK;
}

//========================
//終了処理
//========================
void CPictoNormal::Uninit(void)
{
	m_apPicto[m_nID] = nullptr;
	m_nNumAll--;

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
	CPictoNormal* pPicto = nullptr;

	if (pPicto == nullptr)
	{
		//ピクトの生成
		pPicto = new CPictoNormal(pos);

		//初期化
		pPicto->Init();

		return pPicto;
	}
	else
	{
		return nullptr;
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

//=================================
//帰宅
//=================================
void CPictoNormal::Return(void)
{
	CPicto::Return();
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
		if (m_apPicto[cnt] == nullptr)
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
CPictoPolice::CPictoPolice(const D3DXVECTOR3 pos, const TYPE type) : CPicto(pos,type)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apPicto[cnt] == nullptr)
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

	//モデル設定
	CPicto::SetModel("data\\motion_exithuman_police.txt");

	//体力設定
	m_nLife = HAVE_LIFE(m_nLv);

	return S_OK;
}

//========================
//終了処理
//========================
void CPictoPolice::Uninit(void)
{
	m_apPicto[m_nID] = nullptr;
	m_nNumAll--;

	//親処理
	CPicto::Uninit();
}

//========================
//更新処理
//========================
void CPictoPolice::Update(void)
{
	CSound* pSound = CManager::GetSound();
	D3DXVECTOR3 targetPos = CManager::VEC3_ZERO;
	float targetWidthHalf = CManager::FLOAT_ZERO;
	float targetDepthHalf = CManager::FLOAT_ZERO;
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	D3DXVECTOR3 move = GetMove();
	CMotion* pMotion = GetMotion();

	move.x = CManager::FLOAT_ZERO;
	move.z = CManager::FLOAT_ZERO;

	CObject* pObject = GetTargetObj();
	if (pObject != nullptr)
	{
		if (pObject->GetType() == TYPE_PICTO)
		{
			if (GetState() == STATE_ATTACK)
			{
				//向かせる
				float fTargetLenWidth, fTargetLenDepth;
				targetPos = GetTargetObj()->GetPos();
				fTargetLenWidth = targetPos.x - pos.x;
				fTargetLenDepth = targetPos.z - pos.z;

				if (D3DXVec3Length(&(targetPos - pos)) > LOOSE_LENGTH)
				{//逃がす
					UnsetTarget();
					if (pMotion->GetType() != MOTIONTYPE_NEUTRAL)
					{
						pMotion->Set(MOTIONTYPE_NEUTRAL);
					}
				}

				float fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);
				rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				m_nCounterAttack++;
				if (m_nCounterAttack > PICTO_ATTACK_TIME)
				{
					//弾発射
					CBulletBillboard::Create(GetPos() + D3DXVECTOR3(0.0f, 30.0f, 0.0f), GetRot(), 10.0f, 10.0f, 10.0f, PICTO_POWER(m_nLv, m_nHaveNormalPicto), TYPE_POLICE, this);
					pSound->Play(CSound::SOUND_LABEL_SE_GUN);

					//攻撃カウンターリセット
					m_nCounterAttack = CManager::INT_ZERO;
				}

				if (pMotion->GetType() != MOTIONTYPE_ATTACK)
				{
					pMotion->Set(MOTIONTYPE_ATTACK);
				}
			}
		}
		else if (pObject->GetType() == TYPE_OBJECT)
		{//ちょっと交番の前で突っ立っているので帰ってもらいます
			Return();
			return;
		}
		else
		{
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
							m_pTargetBui = GetTargetObj();
							SetTargetObj(pPicto);
							SetState(STATE_FACE);
						}
					}
				}
			}

			if (GetState() == STATE_ATTACK && pMotion->GetType() != MOTIONTYPE_NEUTRAL)
			{
				pMotion->Set(MOTIONTYPE_NEUTRAL);
			}
		}
	}
	
	//位置移動量設定
	SetRot(rot);
	SetMove(move);

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
CPictoPolice* CPictoPolice::Create(const D3DXVECTOR3 pos, const int nLv)
{
	CPictoPolice* pPicto = nullptr;

	if (pPicto == nullptr)
	{
		//ピクトの生成
		pPicto = new CPictoPolice(pos);

		//初期化
		pPicto->Init();
		pPicto->m_nLv = nLv;

		return pPicto;
	}
	else
	{
		return nullptr;
	}
}

//========================
//ターゲット解除
//========================
void CPictoPolice::UnsetTarget(void)
{
	if (m_pTargetBui != nullptr)
	{//パトロール中
		SetTargetObj(m_pTargetBui);
		m_pTargetBui = nullptr;
		SetState(STATE_FACE);
	}
	else
	{//緊急出動なので建物知らんから帰る
		CKoban* pKobanNear = nullptr;
		float fLengthNear = FLT_MAX;
		for (int cnt = 0; cnt < CKoban::GetNumAll(); cnt++)
		{
			CKoban* pKoban = CKoban::GetKoban(cnt);
			if (pKoban != nullptr)
			{
				float fLength = D3DXVec3Length(&(pKoban->GetPos() - this->GetPos()));
				if (fLengthNear > fLength)
				{
					pKobanNear = pKoban;
					fLengthNear = fLength;
				}
			}
		}
		SetTargetObj(pKobanNear);
		SetState(STATE_LEAVE);
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
		//経験値付与
		CPictoBlocker::AddExp(DROP_EXP(m_nLv));	//いったんレベル9として扱う

		UnsetTargetAll();

		//爆散
		Uninit();
	}

	//赤くする
	SetRedAlpha();
}

//=================================
//帰宅
//=================================
void CPictoPolice::Return(void)
{
	CPicto::Return();
}
