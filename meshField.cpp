//======================================================
//
//メッシュフィールド処理[meshField.cpp]
//Author:石原颯馬
//
//======================================================
#include "meshField.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include <vector>

#include <string>
using namespace std;

//=================================
//コンストラクタ
//=================================
CMeshField::CMeshField()
{
	//値クリア
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_fWidth = CManager::FLOAT_ZERO;
	m_fDepth = CManager::FLOAT_ZERO;
	m_nBlockWidth = CManager::INT_ZERO;
	m_nBlockDepth = CManager::INT_ZERO;
	SetType(TYPE_FIELD);
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CMeshField::CMeshField(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth,
	const int nBlockWidth, const int nBlockDepth)
{
	//値設定
	m_pos = pos;
	m_rot = rot;
	m_fWidth = fWidth;
	m_fDepth = fDepth;
	m_nBlockWidth = nBlockWidth;
	m_nBlockDepth = nBlockDepth;
	SetType(TYPE_FIELD);
}

//=================================
//デストラクタ
//=================================
CMeshField::~CMeshField()
{
}

//=================================
//初期化
//=================================
HRESULT CMeshField::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = CTexture::PRELOAD_ASPHALT;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * (m_nBlockWidth + 1) * (m_nBlockDepth + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxbuff,
		nullptr);

	//インデックスバッファの生成
	int nIdxNum = (((m_nBlockWidth + 1) * m_nBlockDepth * 2) + (2 * (m_nBlockDepth - 1)));
	pDevice->CreateIndexBuffer(sizeof(WORD) * nIdxNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	VERTEX_3D *pVtx;

	//バッファロック
	//頂点バッファのロックと頂点情報へのポインタを取得
	m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標+テクスチャ座標
	for (int nCount = 0; nCount < (m_nBlockWidth + 1) * (m_nBlockDepth + 1); nCount++)
	{
		//頂点座標（相対座標）
		pVtx[nCount].pos = D3DXVECTOR3(m_fWidth * (nCount % (m_nBlockWidth + 1)), 0.0f, -m_fDepth * (nCount / (m_nBlockWidth + 1)));

		//カラー
		pVtx[nCount].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標
		pVtx[nCount].tex = D3DXVECTOR2(((float)(nCount % (m_nBlockWidth + 1))), ((float)(nCount / (m_nBlockWidth + 1))));
	}

	vector<D3DXVECTOR3>* pNor = new vector<D3DXVECTOR3>[(m_nBlockWidth + 1) * (m_nBlockDepth + 1)];

	for (int cntZ = 0; cntZ < m_nBlockDepth; cntZ++)
	{
		for (int cntX = 0; cntX < m_nBlockWidth; cntX++)
		{
			D3DXVECTOR3 nor0, nor1, nor2, nor3;
			D3DXVECTOR3 vec0, vec1;
			int nVtx0 = cntZ * (m_nBlockDepth + 1) + cntX;
			int nVtx1 = cntZ * (m_nBlockDepth + 1) + cntX + 1;
			int nVtx2 = (cntZ + 1) * (m_nBlockDepth + 1) + cntX;
			int nVtx3 = (cntZ + 1) * (m_nBlockDepth + 1) + cntX + 1;

			//1
			vec0 = pVtx[nVtx3].pos - pVtx[nVtx1].pos;
			vec1 = pVtx[nVtx0].pos - pVtx[nVtx1].pos;
			D3DXVec3Cross(&nor1, &vec0, &vec1);
			D3DXVec3Normalize(&nor1, &nor1);

			//2
			vec0 = pVtx[nVtx0].pos - pVtx[nVtx2].pos;
			vec1 = pVtx[nVtx3].pos - pVtx[nVtx2].pos;
			D3DXVec3Cross(&nor2, &vec0, &vec1);
			D3DXVec3Normalize(&nor2, &nor2);

			//0
			nor0 = (nor1 + nor2);
			D3DXVec3Normalize(&nor0, &nor0);
			//3
			nor3 = (nor1 + nor2);
			D3DXVec3Normalize(&nor3, &nor3);

			pNor[nVtx0].push_back(nor0);
			pNor[nVtx1].push_back(nor1);
			pNor[nVtx2].push_back(nor2);
			pNor[nVtx3].push_back(nor3);
		}
	}

	for (int nCount = 0; nCount < (m_nBlockWidth + 1) * (m_nBlockDepth + 1); nCount++, pVtx++)
	{
		D3DXVECTOR3 nor = CManager::VEC3_ZERO;
		//全法線を足す
		for (int cntNor = 0; cntNor < pNor[nCount].size(); cntNor++)
		{
			nor += pNor[nCount].at(cntNor);
		}
		D3DXVec3Normalize(&nor, &nor);

		//法線ベクトル
		pVtx->nor = nor;
	}

	delete[] pNor;

	//頂点バッファをアンロック
	m_pVtxbuff->Unlock();

	//インデックスバッファ設定
	WORD *pIdx;	//インデックス情報へのポインタ
	int nCntWidth;	//インデックスカウンタ

	//バッファロック
	m_pIdxBuff->Lock(0, 0, (void **)&pIdx, 0);

	for (nCntWidth = 0; nCntWidth < m_nBlockDepth - 1; nCntWidth++)
	{
		//グネグネパート
		pIdx = SetIdxSwaingField(nCntWidth, pIdx);

		//チョン打ちするパート
		//最後のインデックス情報を次のにも入れる
		*pIdx = m_nBlockWidth + (m_nBlockWidth + 1) * nCntWidth;
		pIdx++;

		//その次のに次のループで最初に入る数字を入れる
		*pIdx = (m_nBlockWidth + 1) + (m_nBlockWidth + 1) * (nCntWidth + 1);
		pIdx++;
	}

	//最後のグネグネパート
	SetIdxSwaingField(nCntWidth, pIdx);

	//バッファアンロック
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=================================
//終了
//=================================
void CMeshField::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxbuff != nullptr)
	{
		m_pVtxbuff->Release();
		m_pVtxbuff = nullptr;
	}

	//インデックスバッファ破棄
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	//自分自身破棄
	Release();
}

//=================================
//更新
//=================================
void CMeshField::Update(void)
{
}

//=================================
//描画
//=================================
void CMeshField::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得
	CTexture* pTexture = CManager::GetTexture();						//テクスチャオブジェクト取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxbuff, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャ設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//ポリゴン描画（インデックスされたやつ）
	int nPolyNum = ((((m_nBlockWidth + 1) * m_nBlockDepth * 2) + (2 * (m_nBlockDepth - 1))) - 2);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (m_nBlockWidth + 1) * (m_nBlockDepth + 1), 0, nPolyNum);
}

//=================================
//生成
//=================================
CMeshField* CMeshField::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth,
	const int nBlockWidth, const int nBlockDepth)
{
	CMeshField* pMeshField = nullptr;

	if (pMeshField == nullptr)
	{
		//オブジェクト2Dの生成
		pMeshField = new CMeshField(pos, rot, fWidth, fDepth, nBlockWidth, nBlockDepth);

		//初期化
		pMeshField->Init();

		return pMeshField;
	}
	else
	{
		return nullptr;
	}
}

//========================
//グネグネインデックス設定処理
//========================
WORD* CMeshField::SetIdxSwaingField(int nCntWidth, WORD *pIdx)
{
	//グネグネパート
	for (int nCountHeight = 0; nCountHeight < (m_nBlockWidth + 1) * 2; nCountHeight++, pIdx++)
	{
		*pIdx = (m_nBlockWidth + 1) * ((nCountHeight % 2) ^ 0x1) + nCountHeight / 2 + (m_nBlockWidth + 1) * nCntWidth;
	}
	return pIdx;
}

float CMeshField::GetHeight(D3DXVECTOR3 posNew)
{
	//頂点バッファのロックと頂点情報へのポインタを取得
	VERTEX_3D *pVtx;	//設定用ポインタ
	m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0);

	D3DXVECTOR3 dust;

	for (int cntZ = 0; cntZ < m_nBlockDepth; cntZ++)
	{
		for (int cntX = 0; cntX < m_nBlockWidth; cntX++)
		{
			int nVtx0 = cntZ * (m_nBlockDepth + 1) + cntX;
			int nVtx1 = cntZ * (m_nBlockDepth + 1) + cntX + 1;
			int nVtx2 = (cntZ + 1) * (m_nBlockDepth + 1) + cntX;
			int nVtx3 = (cntZ + 1) * (m_nBlockDepth + 1) + cntX + 1;

			if (D3DXVec3Cross(&dust, &(pVtx[nVtx3].pos - pVtx[nVtx2].pos), &(posNew - (pVtx[nVtx2].pos + m_pos)))->y <= 0.0f &&
				D3DXVec3Cross(&dust, &(pVtx[nVtx0].pos - pVtx[nVtx3].pos), &(posNew - (pVtx[nVtx3].pos + m_pos)))->y <= 0.0f &&
				D3DXVec3Cross(&dust, &(pVtx[nVtx2].pos - pVtx[nVtx0].pos), &(posNew - (pVtx[nVtx0].pos + m_pos)))->y <= 0.0f)
			{
				D3DXVECTOR3 vec1, vec2, nor;
				vec1 = pVtx[nVtx0].pos - pVtx[nVtx2].pos;
				vec2 = pVtx[nVtx3].pos - pVtx[nVtx2].pos;
				D3DXVec3Cross(&nor, &vec1, &vec2);
				D3DXVec3Normalize(&nor, &nor);
				if (nor.y != 0.0f)
				{
					return (-((posNew.x - (pVtx[nVtx2].pos.x + m_pos.x))*nor.x + (posNew.z - (pVtx[nVtx2].pos.z + m_pos.z))*nor.z) / nor.y) + pVtx[nVtx2].pos.y + m_pos.y;
				}
			}

			if (D3DXVec3Cross(&dust, &(pVtx[nVtx0].pos - pVtx[nVtx1].pos), &(posNew - (pVtx[nVtx1].pos + m_pos)))->y <= 0.0f &&
				D3DXVec3Cross(&dust, &(pVtx[nVtx3].pos - pVtx[nVtx0].pos), &(posNew - (pVtx[nVtx0].pos + m_pos)))->y <= 0.0f &&
				D3DXVec3Cross(&dust, &(pVtx[nVtx1].pos - pVtx[nVtx3].pos), &(posNew - (pVtx[nVtx3].pos + m_pos)))->y <= 0.0f)
			{
				D3DXVECTOR3 vec1, vec2, nor;
				vec1 = pVtx[nVtx3].pos - pVtx[nVtx1].pos;
				vec2 = pVtx[nVtx0].pos - pVtx[nVtx1].pos;
				D3DXVec3Cross(&nor, &vec1, &vec2);
				D3DXVec3Normalize(&nor, &nor);
				if (nor.y != 0.0f)
				{
					return (-((posNew.x - (pVtx[nVtx1].pos.x + m_pos.x))*nor.x + (posNew.z - (pVtx[nVtx1].pos.z + m_pos.z))*nor.z) / nor.y) + pVtx[nVtx1].pos.y + m_pos.y;
				}
			}
		}
	}

	//頂点バッファをアンロック
	m_pVtxbuff->Unlock();

	return 0.0f;
}