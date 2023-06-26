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
#include "file.h"
#include "Culc.h"

//マクロ
#define PICT_WALK_SPEED		(6.0f)	//ピクトさんの歩行速度
#define PICT_STOP_LENGTH	(60.0f)	//ピクトさんが建物から離れる距離
#define PICT_DESTRUCTION_TIME	(60)//破壊工作を行う間隔

//静的メンバ変数
CPict* CPict::m_apPict[MAX_OBJ];
int CPict::m_nNumAll = 0;

//仮：モデルパス
const char* c_apModelPath[PICT_MODEL_NUM] =
{
	"data/MODEL/exithuman_x/01_Body.x",
	"data/MODEL/exithuman_x/02_Head.x",
	"data/MODEL/exithuman_x/03_Left_Arm.x",
	"data/MODEL/exithuman_x/04_Left_Hand.x",
	"data/MODEL/exithuman_x/05_Right_Arm.x",
	"data/MODEL/exithuman_x/06_Right_Hand.x",
	"data/MODEL/exithuman_x/07_Left_Leg.x",
	"data/MODEL/exithuman_x/08_Left_Foot.x",
	"data/MODEL/exithuman_x/09_Right_Leg.x",
	"data/MODEL/exithuman_x/10_Right_Foot.x"
};

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
	m_pTargetBuilding= NULL;
	m_pMotion = NULL;
	m_nCounterDestruction = INT_ZERO;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_bControll = false;
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
	m_pTargetBuilding = NULL;
	m_nCounterDestruction = INT_ZERO;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nCounterJumpTime = 0;
	m_bJump = false;
	m_bControll = false;
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
	m_pTargetBuilding = NULL;
	m_nCounterDestruction = INT_ZERO;

	//モーション生成・初期化
	m_pMotion = new CMotion;
	m_pMotion->Init();

	//モーションビューアのファイルを読み込み
	LoadMotionViewerFile("data\\motion_exithuman.txt", &m_apModel[0], m_pMotion, &m_nNumModel);

	//モーション設定
	m_pMotion->Set(0);

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

	//できた
	return S_OK;
}

//========================
//終了処理
//========================
void CPict::Uninit(void)
{
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

	//ジャンプカウンタ増やす
	m_nCounterJumpTime++;

	if (m_pTargetBuilding != NULL)
	{
		if (m_bControll == false)
		{
			m_move.x = FLOAT_ZERO;
			m_move.z = FLOAT_ZERO;
			targetPos = m_pTargetBuilding->GetPos();
			targetWidthHalf = m_pTargetBuilding->GetWidth() * 0.5f;
			targetDepthHalf = m_pTargetBuilding->GetDepth() * 0.5f;

			if (targetPos.x - targetWidthHalf - PICT_STOP_LENGTH > m_pos.x || targetPos.x + targetWidthHalf + PICT_STOP_LENGTH < m_pos.x ||
				targetPos.z - targetDepthHalf - PICT_STOP_LENGTH > m_pos.z || targetPos.z + targetDepthHalf + PICT_STOP_LENGTH < m_pos.z)
			{
				float fTargetLenWidth, fTargetLenDepth;
				float fTargetRot;

				fTargetLenWidth = targetPos.x - m_pos.x;
				fTargetLenDepth = targetPos.z - m_pos.z;

				fTargetRot = atan2f(fTargetLenWidth, fTargetLenDepth);

				m_move.x = sinf(fTargetRot) * PICT_WALK_SPEED;
				m_move.z = cosf(fTargetRot) * PICT_WALK_SPEED;

				m_rot.y = FIX_ROT(fTargetRot + D3DX_PI);

				if (m_pMotion->GetType() != 1)
				{
					m_pMotion->Set(1);
				}

				//破壊カウンターリセット
				m_nCounterDestruction = INT_ZERO;
			}
			else
			{
				m_nCounterDestruction++;
				if (m_nCounterDestruction > PICT_DESTRUCTION_TIME)
				{
					//破壊工作
					m_pTargetBuilding->AddDamage(1000);

					//破壊カウンターリセット
					m_nCounterDestruction = INT_ZERO;
				}

				if (m_pMotion->GetType() != 0)
				{
					m_pMotion->Set(0);
				}
			}
		}
	}

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
	CollisionBlockX(&pos);

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
	CollisionBlockZ(&pos);

	m_pos = pos;

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
	D3DXMATERIAL *pMat;													//マテリアルデータへのポインタ

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
		m_apModel[cnt]->Draw();
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//生成処理
//========================
CPict* CPict::Create(D3DXVECTOR3 pos)
{
	CPict* pPict = NULL;

	if (pPict == NULL)
	{
		//オブジェクト2Dの生成
		pPict = new CPict(pos);

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
			TYPE type = pObj->GetType();	//種類取得

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

//=================================
//ブロックとの衝突判定(Y)
//=================================
bool CPict::CollisionBlockY(D3DXVECTOR3* pPosNew)
{
	float fPlayerWidth = GetWidth() * 0.5f, fPlayerHeight = GetHeight() * 0.5f, fPlayerDepth = GetDepth() * 0.5f;
	bool bLand = false;

	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//全オブジェクト見る
		CObject* pObj = GetObject(BLOCK_PRIORITY, cnt);	//オブジェクト取得

		if (pObj != NULL)	//ヌルチェ
		{//なんかある
			TYPE type = pObj->GetType();	//種類取得

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
			TYPE type = pObj->GetType();	//種類取得

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
