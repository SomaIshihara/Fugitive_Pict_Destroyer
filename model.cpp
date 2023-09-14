//======================================================
//
//モデル処理[model.cpp]
//Author:石原颯馬
//
//======================================================
#include "main.h"
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include <assert.h>

//=================================
//コンストラクタ
//=================================
CModel::CModel()
{
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_posMotioned = CManager::VEC3_ZERO;
	m_rotMotioned = CManager::VEC3_ZERO;
	m_pParent = nullptr;
}

//=================================
//デストラクタ
//=================================
CModel::~CModel()
{
}

//========================
//初期化処理
//========================
HRESULT CModel::Init(const char * pPath, const D3DXVECTOR3 posOffset, const D3DXVECTOR3 rotOffset)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetTexture();						//テクスチャオブジェクト取得
	m_pIdxtexture = nullptr;											//テクスチャ番号ポインタをnullptrにする
	m_bChangeColor = false;												//色変更フラグをオフ

	if (SUCCEEDED(D3DXLoadMeshFromX(
		pPath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		nullptr,
		&m_pBuffMat,
		nullptr,
		&m_dwNumMatModel,
		&m_pMesh)))
	{
		//テクスチャポインタ確保
		if (m_pIdxtexture == nullptr)
		{//nullptr
		 //テクスチャ番号配列確保
			m_pIdxtexture = new int[(int)m_dwNumMatModel];

			//テクスチャ読み込み
			D3DXMATERIAL* pMat;	//マテリアルポインタ

			//マテリアル情報に対するポインタ取得
			pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

			//テクスチャ読み込み
			for (int nCntTex = 0; nCntTex < (int)m_dwNumMatModel; nCntTex++)
			{
				m_pIdxtexture[nCntTex] = -1;
				if (pMat[nCntTex].pTextureFilename != nullptr)
				{//テクスチャあるよ
				 //テクスチャ読み込み
					m_pIdxtexture[nCntTex] = pTexture->Regist(pMat[nCntTex].pTextureFilename);
				}
				else
				{//ないよ
					m_pIdxtexture[nCntTex] = -1;	//テクスチャ取得時にnullptrになるようにする
				}
			}
		}
		else
		{//おかしい
			assert(false);
		}
	}

	//位置向き設定
	m_pos = posOffset;
	m_rot = rotOffset;

	return S_OK;
}

//========================
//終了処理
//========================
void CModel::Uninit(void)
{
	//メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	//マテリアルの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	//テクスチャ番号破棄
	if (m_pIdxtexture != nullptr)
	{
		delete[] m_pIdxtexture;
		m_pIdxtexture = nullptr;
	}
}

//========================
//更新処理
//========================
void CModel::Update(void)
{
	//いったん仮の値を入れる
	m_posMotioned = m_pos;
	m_rotMotioned = m_rot;
}

//========================
//描画処理
//========================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetTexture();						//テクスチャオブジェクト取得
	D3DXMATRIX mtxRot, mtxTrans, mtxTexture;							//計算用
	D3DXMATRIX mtxParent;												//親のマトリ
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotMotioned.y, m_rotMotioned.x, m_rotMotioned.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, m_posMotioned.x, m_posMotioned.y, m_posMotioned.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//パーツの親マトリ設定
	if (m_pParent != nullptr)
	{
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//パーツのマトリと親マトリをかけ合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//マテリアルデータへのポインタ取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMatModel; nCntMat++)
	{
		//マテリアル変更
		D3DMATERIAL9 changeMat = pMat[nCntMat].MatD3D;

		if (m_bChangeColor == true)
		{//色を変更する場合
			//メイン色変更
			changeMat.Diffuse = m_changeMainColor;

			//サブ色変更
			changeMat.Diffuse.r = 1.0f * m_changeSubColor.r + changeMat.Diffuse.r * (1.0f - m_changeSubColor.r);
			changeMat.Diffuse.g = 1.0f * m_changeSubColor.g + changeMat.Diffuse.g * (1.0f - m_changeSubColor.g);
			changeMat.Diffuse.b = 1.0f * m_changeSubColor.b + changeMat.Diffuse.b * (1.0f - m_changeSubColor.b);
		}

		//マテリアル設定
		pDevice->SetMaterial(&changeMat);

		//テクスチャ設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_pIdxtexture[nCntMat]));

		//モデル描画
		m_pMesh->DrawSubset(nCntMat);
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//生成処理
//========================
CModel* CModel::Create(const char * pPath, const D3DXVECTOR3 posOffset, const D3DXVECTOR3 rotOffset)
{
	CModel* pModel = nullptr;

	if (pModel == nullptr)
	{
		//オブジェクト2Dの生成
		pModel = new CModel();

		//初期化
		pModel->Init(pPath, posOffset, rotOffset);

		return pModel;
	}
	else
	{
		return nullptr;
	}
}