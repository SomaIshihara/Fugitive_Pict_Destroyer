//======================================================
//
//オブジェクト（X）処理[object3D.cpp]
//Author:石原颯馬
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "objectX.h"
#include <assert.h>

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

	//サイズ設定
	D3DXVECTOR3 vtxMin, vtxMax;
	m_aModel[m_nIdx].m_collision.GetVtx(&vtxMin, &vtxMax);
	m_fWidth = vtxMax.x - vtxMin.x;
	m_fHeight = vtxMax.y - vtxMin.y;
	m_fDepth = vtxMax.z - vtxMin.z;
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
	SetType(TYPE_BUILDING);	//一時的に建物とする（当たり判定チェックのため）
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
	CTexture* pTexture = CManager::GetTexture();						//テクスチャオブジェクト取得
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
		pDevice->SetTexture(0, pTexture->GetAddress(m_aModel[m_nIdx].m_pIdxtexture[nCntMat]));

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
	CObjectX* pObjX = NULL;

	if (pObjX == NULL)
	{
		//オブジェクト2Dの生成
		pObjX = new CObjectX(pos, rot, nIdx);

		//初期化
		pObjX->Init();

		return pObjX;
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
	CTexture* pTexture = CManager::GetTexture();						//テクスチャオブジェクト取得
	m_aModel[nIdx].m_pIdxtexture = NULL;	//テクスチャ番号ポインタをNULLにする

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
		//テクスチャポインタ確保
		if (m_aModel[nIdx].m_pIdxtexture == NULL)
		{//NULL
			//テクスチャ番号配列確保
			m_aModel[nIdx].m_pIdxtexture = new int[(int)m_aModel[nIdx].m_dwNumMatModel];

			//当たり判定生成
			int nNumVtx;		//頂点数
			DWORD dwSizeFVF;	//頂点フォーマットのサイズ
			BYTE *pVtxBuff;		//頂点バッファポインタ

			//頂点数を取得
			nNumVtx = m_aModel[nIdx].m_pMesh->GetNumVertices();

			//頂点フォーマット
			dwSizeFVF = D3DXGetFVFVertexSize(m_aModel[nIdx].m_pMesh->GetFVF());

			//頂点バッファロック
			m_aModel[nIdx].m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void **)&pVtxBuff);

			//最初だけ全部入れる
			D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;

			D3DXVECTOR3 vtxMax = vtx;
			D3DXVECTOR3 vtxMin = vtx;

			pVtxBuff += dwSizeFVF;

			for (int nCntVtx = 1; nCntVtx < nNumVtx; nCntVtx++, pVtxBuff += dwSizeFVF)
			{
				D3DXVECTOR3 vtx = *(D3DXVECTOR3 *)pVtxBuff;

				if (vtxMax.x < vtx.x)
				{
					vtxMax.x = vtx.x;
				}
				if (vtxMax.y < vtx.y)
				{
					vtxMax.y = vtx.y;
				}
				if (vtxMax.z < vtx.z)
				{
					vtxMax.z = vtx.z;
				}
				if (vtxMin.x > vtx.x)
				{
					vtxMin.x = vtx.x;
				}
				if (vtxMin.y > vtx.y)
				{
					vtxMin.y = vtx.y;
				}
				if (vtxMin.z > vtx.z)
				{
					vtxMin.z = vtx.z;
				}
			}

			//設定
			m_aModel[nIdx].m_collision.SetVtx(vtxMin, vtxMax);

			//頂点バッファアンロック
			m_aModel[nIdx].m_pMesh->UnlockVertexBuffer();

			//テクスチャ読み込み
			D3DXMATERIAL* pMat;	//マテリアルポインタ

			//マテリアル情報に対するポインタ取得
			pMat = (D3DXMATERIAL*)m_aModel[nIdx].m_pBuffMat->GetBufferPointer();

			//テクスチャ読み込み
			for (int nCntTex = 0; nCntTex < (int)m_aModel[nIdx].m_dwNumMatModel; nCntTex++)
			{
				m_aModel[nIdx].m_pIdxtexture[nCntTex] = NULL;
				if (pMat[nCntTex].pTextureFilename != NULL)
				{//テクスチャあるよ
					//テクスチャ読み込み
					m_aModel[nIdx].m_pIdxtexture[nCntTex] = pTexture->Regist(pMat[nCntTex].pTextureFilename);
				}
				else
				{//ないよ
					m_aModel[nIdx].m_pIdxtexture[nCntTex] = -1;	//テクスチャ取得時にNULLになるようにする
				}
			}
		}
		else
		{//おかしい
			assert(false);
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

		//テクスチャ番号破棄
		if (m_aModel[cntModel].m_pIdxtexture != NULL)
		{
			delete[] m_aModel[cntModel].m_pIdxtexture;
			m_aModel[cntModel].m_pIdxtexture = NULL;
		}
	}
}