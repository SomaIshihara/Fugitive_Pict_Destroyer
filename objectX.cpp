//======================================================
//
//オブジェクト（3D）処理[object3D.cpp]
//Author:石原颯馬
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "objectX.h"

//静的メンバ変数
CObjectX::Model CObjectX::m_aModel[];

//=================================
//コンストラクタ（デフォルト）
//=================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	//クリア
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nIdx = 0;
}

//=================================
//コンストラクタ（オーバーロード 位置向き）
//=================================
CObjectX::CObjectX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx, int nPriority) : CObject(nPriority)
{
	//クリア
	m_pos = pos;
	m_rot = rot;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nIdx = nIdx;
}

//=================================
//デストラクタ
//=================================
CObjectX::~CObjectX()
{
}

//========================
//初期化処理
//========================
HRESULT CObjectX::Init(void)
{
	return S_OK;
}

//========================
//終了処理
//========================
void CObjectX::Uninit(void)
{
	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CObjectX::Update(void)
{
	
}

//========================
//描画処理
//========================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;							//計算用
	D3DMATERIAL9 matDef;												//現在のマテリアル保存用
	D3DXMATERIAL *pMat;													//マテリアルデータへのポインタ

	//現在のマテリアル取得
	pDevice->GetMaterial(&matDef);

	//モデル取得
	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&mtxWorld);

	//拡縮を反映
	//D3DXMatrixScaling(&mtxScall, FENCE_SCALE, FENCE_SCALE, FENCE_SCALE);
	//D3DXMatrixMultiply(&g_aFence[nCount].mtxWorld, &g_aFence[nCount].mtxWorld, &mtxScall);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)m_aModel[m_nIdx].m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aModel[m_nIdx].m_dwNumMatModel; nCntMat++)
	{
		//マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャ設定
		pDevice->SetTexture(0, m_aModel[m_nIdx].m_apTexture[nCntMat]);

		//モデル描画
		m_aModel[m_nIdx].m_pMesh->DrawSubset(nCntMat);
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//生成処理
//========================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx)
{
	CObjectX* pObj2D = NULL;

	if (pObj2D == NULL)
	{
		//オブジェクト2Dの生成
		pObj2D = new CObjectX(pos, rot, nIdx);

		//初期化
		pObj2D->Init();

		return pObj2D;
	}
	else
	{
		return NULL;
	}
}

//========================
//ファイル読み込み処理
//========================
void CObjectX::Load(const char * pPath, const int nIdx)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得

	if (SUCCEEDED(D3DXLoadMeshFromX(
		pPath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_aModel[nIdx].m_pBuffMat,
		NULL,
		&m_aModel[nIdx].m_dwNumMatModel,
		&m_aModel[nIdx].m_pMesh)))
	{
		//テクスチャ読み込み
		D3DXMATERIAL* pMat;	//マテリアルポインタ

		//マテリアル情報に対するポインタ取得
		pMat = (D3DXMATERIAL*)m_aModel[nIdx].m_pBuffMat->GetBufferPointer();

		//テクスチャ読み込み
		for (int nCntTex = 0; nCntTex < (int)m_aModel[nIdx].m_dwNumMatModel; nCntTex++)
		{
			if (pMat[nCntTex].pTextureFilename != NULL)
			{
				//テクスチャ読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntTex].pTextureFilename,
					&m_aModel[nIdx].m_apTexture[nCntTex]);
			}
		}
	}
}

//========================
//ファイル破棄処理
//========================
void CObjectX::Unload(void)
{
	for (int cntModel = 0; cntModel < X_MODEL_NUM; cntModel++)
	{
		//メッシュの破棄
		if (m_aModel[cntModel].m_pMesh != NULL)
		{
			m_aModel[cntModel].m_pMesh->Release();
			m_aModel[cntModel].m_pMesh = NULL;
		}

		//マテリアルの破棄
		if (m_aModel[cntModel].m_pBuffMat != NULL)
		{
			m_aModel[cntModel].m_pBuffMat->Release();
			m_aModel[cntModel].m_pBuffMat = NULL;
		}

		//仮置き：テクスチャ破棄
		for (int cntTexture = 0; cntTexture < X_TEXTURE_NUM; cntTexture++)
		{
			if (m_aModel[cntModel].m_apTexture[cntTexture] != NULL)
			{
				m_aModel[cntModel].m_apTexture[cntTexture]->Release();
				m_aModel[cntModel].m_apTexture[cntTexture] = NULL;
			}
		}
	}
	
}