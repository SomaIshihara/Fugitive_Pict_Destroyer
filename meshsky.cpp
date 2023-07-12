//======================================================
//
//メッシュ空処理[meshsky.cpp]
//Author:石原颯馬
//
//======================================================
#include "meshsky.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include <vector>

using namespace std;

//=================================
//コンストラクタ
//=================================
CMeshSky::CMeshSky()
{
	//値クリア
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_fRadius = FLOAT_ZERO;
	m_nBlockVertical = INT_ZERO;
	m_nBlockHorizontal = INT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CMeshSky::CMeshSky(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius,
	const int nBlockWidth, const int nBlockDepth)
{
	//値設定
	m_pos = pos;
	m_rot = rot;
	m_fRadius = fRadius;
	m_nBlockVertical = nBlockWidth;
	m_nBlockHorizontal = nBlockDepth;
}

//=================================
//デストラクタ
//=================================
CMeshSky::~CMeshSky()
{
}

//=================================
//初期化
//=================================
HRESULT CMeshSky::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得

	//テクスチャ読み込み
	CTexture* pTexture = CManager::GetTexture();
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Block_R_01.png");

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nBlockVertical + 1) * (m_nBlockHorizontal + 1) + 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxbuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nBlockHorizontal + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffTop,
		NULL);

	int nIdxNum = (((m_nBlockVertical + 1) * m_nBlockHorizontal * 2) + (2 * (m_nBlockHorizontal - 1)));
	pDevice->CreateIndexBuffer(sizeof(WORD) * nIdxNum,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffMiddle,
		NULL);

	VERTEX_3D *pVtx;

	//バッファロック
	//頂点バッファのロックと頂点情報へのポインタを取得
	m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0);

	//上部
	pVtx[0].pos = D3DXVECTOR3(0.0f, m_fRadius, 0.0f);
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	//頂点座標+テクスチャ座標
	for (int cntV = 0; cntV < m_nBlockVertical; cntV++)
	{
		float fAngleV = (1.0f * D3DX_PI) * ((float)(cntV + 1) / (m_nBlockVertical + 1));

		for (int cntH = 0; cntH < m_nBlockHorizontal + 1; cntH++)
		{
			float fAngleH = (2.0f * D3DX_PI) * ((float)cntH / (m_nBlockHorizontal));
			int cnt = cntV * (m_nBlockHorizontal + 1) + cntH + 1;

			pVtx[cnt].pos = D3DXVECTOR3(sinf(fAngleH) * sinf(fAngleV) * m_fRadius, cosf(fAngleV) * m_fRadius, cosf(fAngleH) * sinf(fAngleV) * m_fRadius);
			pVtx[cnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[cnt].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}

	//下部
	int nLastVtx = (m_nBlockVertical - 1) * (m_nBlockHorizontal + 1) + m_nBlockHorizontal + 2;
	pVtx[nLastVtx].pos = D3DXVECTOR3(0.0f, -m_fRadius, 0.0f);
	pVtx[nLastVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[nLastVtx].tex = D3DXVECTOR2(0.0f, 0.0f);

	vector<D3DXVECTOR3>* pNor = new vector<D3DXVECTOR3>[(m_nBlockVertical + 1) * (m_nBlockHorizontal + 1)];

	for (int cntZ = 0; cntZ < m_nBlockHorizontal; cntZ++)
	{
		for (int cntX = 0; cntX < m_nBlockVertical; cntX++)
		{
			D3DXVECTOR3 nor0, nor1, nor2, nor3;
			D3DXVECTOR3 vec0, vec1;
			int nVtx0 = cntZ * (m_nBlockHorizontal + 1) + cntX;
			int nVtx1 = cntZ * (m_nBlockHorizontal + 1) + cntX + 1;
			int nVtx2 = (cntZ + 1) * (m_nBlockHorizontal + 1) + cntX;
			int nVtx3 = (cntZ + 1) * (m_nBlockHorizontal + 1) + cntX + 1;

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

	for (int nCount = 0; nCount < (m_nBlockVertical + 1) * (m_nBlockHorizontal + 1); nCount++, pVtx++)
	{
		D3DXVECTOR3 nor = VEC3_ZERO;
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

	//上部バッファロック
	m_pIdxBuffTop->Lock(0, 0, (void **)&pIdx, 0);

	for (int cnt = 0; cnt < m_nBlockHorizontal + 1; cnt++)
	{
		pIdx[cnt] = cnt;
	}
	pIdx[m_nBlockHorizontal + 1] = 1;

	//上部バッファアンロック
	m_pIdxBuffTop->Unlock();

	//バッファロック
	m_pIdxBuffMiddle->Lock(0, 0, (void **)&pIdx, 0);

	for (nCntWidth = 0; nCntWidth < m_nBlockHorizontal - 1; nCntWidth++)
	{
		//グネグネパート
		pIdx = SetIdxSwaingField(nCntWidth, pIdx);

		//チョン打ちするパート
		//最後のインデックス情報を次のにも入れる
		*pIdx = *(pIdx - 1);
		pIdx++;

		//その次のに次のループで最初に入る数字を入れる
		*pIdx = (m_nBlockVertical + 1) + (m_nBlockVertical + 1) * (nCntWidth + 1) + 1;
		pIdx++;
	}

	//最後のグネグネパート
	SetIdxSwaingField(nCntWidth, pIdx);

	//バッファアンロック
	m_pIdxBuffMiddle->Unlock();

	return S_OK;
}

//=================================
//終了
//=================================
void CMeshSky::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxbuff != NULL)
	{
		m_pVtxbuff->Release();
		m_pVtxbuff = NULL;
	}

	//インデックスバッファ破棄
	if (m_pIdxBuffMiddle != NULL)
	{
		m_pIdxBuffMiddle->Release();
		m_pIdxBuffMiddle = NULL;
	}
}

//=================================
//更新
//=================================
void CMeshSky::Update(void)
{
}

//=================================
//描画
//=================================
void CMeshSky::Draw(void)
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

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャ設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuffTop);

	//ポリゴン描画（インデックスされたやつ）
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (m_nBlockHorizontal + 1), 0, (m_nBlockHorizontal));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuffMiddle);

	//ポリゴン描画（インデックスされたやつ）
	int nPolyNum = (((m_nBlockVertical * m_nBlockHorizontal * 2) + (2 * (m_nBlockHorizontal - 1)))) - 2;
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (m_nBlockVertical - 1) * (m_nBlockHorizontal + 1) + m_nBlockHorizontal, 0, nPolyNum);
}

//=================================
//生成
//=================================
CMeshSky* CMeshSky::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fRadius, const int nBlockWidth, const int nBlockDepth)
{
	CMeshSky* pMeshField = NULL;

	if (pMeshField == NULL)
	{
		//オブジェクト2Dの生成
		pMeshField = new CMeshSky(pos, rot, fRadius, nBlockWidth, nBlockDepth);

		//初期化
		pMeshField->Init();

		return pMeshField;
	}
	else
	{
		return NULL;
	}
}

//========================
//グネグネインデックス設定処理
//========================
WORD* CMeshSky::SetIdxSwaingField(int nCntWidth, WORD *pIdx)
{
	//グネグネパート
	for (int nCountHeight = 0; nCountHeight < (m_nBlockVertical + 1) * 2; nCountHeight++, pIdx++)
	{
		*pIdx = (m_nBlockVertical + 1) * ((nCountHeight % 2) ^ 0x1) + nCountHeight / 2 + (m_nBlockVertical + 1) * nCntWidth + 1;
	}
	return pIdx;
}

float CMeshSky::GetHeight(D3DXVECTOR3 posNew)
{
	//頂点バッファのロックと頂点情報へのポインタを取得
	VERTEX_3D *pVtx;	//設定用ポインタ
	m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0);

	D3DXVECTOR3 dust;

	for (int cntZ = 0; cntZ < m_nBlockHorizontal; cntZ++)
	{
		for (int cntX = 0; cntX < m_nBlockVertical; cntX++)
		{
			int nVtx0 = cntZ * (m_nBlockHorizontal + 1) + cntX;
			int nVtx1 = cntZ * (m_nBlockHorizontal + 1) + cntX + 1;
			int nVtx2 = (cntZ + 1) * (m_nBlockHorizontal + 1) + cntX;
			int nVtx3 = (cntZ + 1) * (m_nBlockHorizontal + 1) + cntX + 1;

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