//======================================================
//
//ピクトさん処理[pict.cpp]
//Author:石原颯馬
//
//======================================================
#include "pict.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"

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
	m_targetPos = VEC3_ZERO;
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
	m_targetPos = VEC3_ZERO;
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
	//モデル生成
	m_apModel[0] = CModel::Create(c_apModelPath[0], D3DXVECTOR3(0.0f, 35.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[1] = CModel::Create(c_apModelPath[1], D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[2] = CModel::Create(c_apModelPath[2], D3DXVECTOR3(-5.0f, 7.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[3] = CModel::Create(c_apModelPath[3], D3DXVECTOR3(-10.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[4] = CModel::Create(c_apModelPath[4], D3DXVECTOR3(5.0f, 7.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[5] = CModel::Create(c_apModelPath[5], D3DXVECTOR3(10.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[6] = CModel::Create(c_apModelPath[6], D3DXVECTOR3(-3.0f, -8.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[7] = CModel::Create(c_apModelPath[7], D3DXVECTOR3(0.0f, -12.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[8] = CModel::Create(c_apModelPath[8], D3DXVECTOR3(3.0f, -8.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apModel[9] = CModel::Create(c_apModelPath[9], D3DXVECTOR3(0.0f, -12.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//モデル親子設定
	m_apModel[0]->SetParent(NULL);
	m_apModel[1]->SetParent(m_apModel[0]);
	m_apModel[2]->SetParent(m_apModel[0]);
	m_apModel[3]->SetParent(m_apModel[2]);
	m_apModel[4]->SetParent(m_apModel[0]);
	m_apModel[5]->SetParent(m_apModel[4]);
	m_apModel[6]->SetParent(m_apModel[0]);
	m_apModel[7]->SetParent(m_apModel[6]);
	m_apModel[8]->SetParent(m_apModel[0]);
	m_apModel[9]->SetParent(m_apModel[8]);

	m_targetPos = D3DXVECTOR3(-100.0f, 0.0f, -100.0f);

	//できた
	return S_OK;
}

//========================
//終了処理
//========================
void CPict::Uninit(void)
{
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
	D3DXVECTOR3 targetLength = m_targetPos - m_pos;
	if (D3DXVec3Length(&targetLength) >= 10.0f)
	{
		float fTargetWidth, fTargetDepth;
		float fTargetRot;

		fTargetWidth = m_targetPos.x - m_pos.x;
		fTargetDepth = m_targetPos.z - m_pos.z;

		fTargetRot = atan2f(fTargetWidth, fTargetDepth);

		m_pos.x += sinf(fTargetRot) * 1.2f;
		m_pos.z += cosf(fTargetRot) * 1.2f;
	}
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
