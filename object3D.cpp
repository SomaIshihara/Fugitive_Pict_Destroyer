//======================================================
//
//オブジェクト（3D）処理[object3D.cpp]
//Author:石原颯馬
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"
#include "object3D.h"
#include <assert.h>

//=================================
//コンストラクタ（デフォルト）
//=================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	//クリア
	m_pVtxbuff = nullptr;
	m_nIdxTexture = -1;
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
}

//=================================
//コンストラクタ（オーバーロード 位置向き）
//=================================
CObject3D::CObject3D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth, int nPriority) : CObject(nPriority)
{
	//クリア
	m_pVtxbuff = nullptr;
	m_nIdxTexture = -1;
	m_pos = pos;
	m_rot = rot;
	m_fWidth = fWidth;
	m_fDepth = fDepth;
}

//=================================
//デストラクタ
//=================================
CObject3D::~CObject3D()
{
}

//========================
//初期化処理
//========================
HRESULT CObject3D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxbuff,
		nullptr)))
	{
		return E_FAIL;
	}

	////仮：テクスチャ読み込み
	//CTexture* pTexture = CManager::GetTexture();
	//m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\icon32.png");

	VERTEX_3D *pVtx;

	//バッファロック
	//頂点バッファのロックと頂点情報へのポインタを取得
	m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標（相対座標）
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth * 0.5f, 0.0f, m_fDepth * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth * 0.5f, 0.0f, m_fDepth * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth * 0.5f, 0.0f, -m_fDepth * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth * 0.5f, 0.0f, -m_fDepth * 0.5f);

	//法線ベクトル
	D3DXVECTOR3 vec1 = pVtx[1].pos - pVtx[0].pos;
	D3DXVECTOR3 vec2 = pVtx[2].pos - pVtx[0].pos;
	D3DXVec3Cross(&pVtx[0].nor, &vec1, &vec2);
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

	vec1 = pVtx[2].pos - pVtx[3].pos;
	vec2 = pVtx[1].pos - pVtx[3].pos;
	D3DXVec3Cross(&pVtx[3].nor, &vec1, &vec2);
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);
	//↓内部
	/*D3DXVECTOR3 vec1 = pVtx[2].pos - pVtx[0].pos;
	D3DXVECTOR3 vec2 = pVtx[1].pos - pVtx[0].pos;
	pVtx[0].nor = D3DXVECTOR3(
		(vec1.z * vec2.y) - (vec1.y * vec2.z),
		(vec1.z * vec2.x) - (vec1.x * vec2.z),
		(vec1.y * vec2.x) - (vec1.x * vec2.y)
	);
	D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);


	vec1 = pVtx[1].pos - pVtx[3].pos;
	vec2 = pVtx[2].pos - pVtx[3].pos;
	pVtx[3].nor = D3DXVECTOR3(
		(vec1.z * vec2.y) - (vec1.y * vec2.z),
		(vec1.z * vec2.x) - (vec1.x * vec2.z),
		(vec1.y * vec2.x) - (vec1.x * vec2.y)
	);
	D3DXVec3Normalize(&pVtx[3].nor, &pVtx[3].nor);*/

	pVtx[1].nor = (pVtx[0].nor + pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[1].nor, &pVtx[1].nor);

	pVtx[2].nor = (pVtx[0].nor + pVtx[1].nor);
	D3DXVec3Normalize(&pVtx[2].nor, &pVtx[2].nor);

	//カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxbuff->Unlock();

	return S_OK;
}

//========================
//終了処理
//========================
void CObject3D::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxbuff != nullptr)
	{
		m_pVtxbuff->Release();
		m_pVtxbuff = nullptr;
	}

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CObject3D::Update(void)
{
	
}

//========================
//描画処理
//========================
void CObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetTexture();						//テクスチャオブジェクト取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用

	CManager::GetRenderer()->SetEnableAlplaTest(true);
	CManager::GetRenderer()->SetEnableZTest(false);

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

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	CManager::GetRenderer()->SetEnableAlplaTest(false);
	CManager::GetRenderer()->SetEnableZTest(true);
}

//========================
//生成処理
//========================
CObject3D* CObject3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth, int nPriority)
{
	CObject3D* pObj3D = nullptr;

	if (pObj3D == nullptr)
	{
		//オブジェクト2Dの生成
		pObj3D = new CObject3D(pos, rot, fWidth, fDepth, nPriority);

		//初期化
		pObj3D->Init();

		return pObj3D;
	}
	else
	{
		return nullptr;
	}
}

//========================
//色設定
//========================
HRESULT CObject3D::SetCol(const D3DXCOLOR col)
{
	//頂点バッファのロックと頂点情報へのポインタを取得
	VERTEX_3D *pVtx;	//設定用ポインタ
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		return E_FAIL;
	}

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロック
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		return E_FAIL;
	}

	return S_OK;
}

float CObject3D::GetHeight(D3DXVECTOR3 pos)
{
	//頂点バッファのロックと頂点情報へのポインタを取得
	VERTEX_3D *pVtx;	//設定用ポインタ
	m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0);

	D3DXVECTOR3 dust;
	
	if (D3DXVec3Cross(&dust, &(pVtx[1].pos - pVtx[3].pos), &(pos - pVtx[3].pos))->y <= 0.0f &&
		D3DXVec3Cross(&dust, &(pVtx[2].pos - pVtx[1].pos), &(pos - pVtx[1].pos))->y <= 0.0f &&
		D3DXVec3Cross(&dust, &(pVtx[3].pos - pVtx[2].pos), &(pos - pVtx[2].pos))->y <= 0.0f)
	{
		D3DXVECTOR3 vec1, vec2, nor;
		vec1 = pVtx[2].pos - pVtx[3].pos;
		vec2 = pVtx[1].pos - pVtx[3].pos;
		D3DXVec3Cross(&nor, &vec1, &vec2);
		if (nor.y != 0.0f)
		{
			return (-((pos.x - pVtx[3].pos.x)*nor.x + (pos.z - pVtx[3].pos.z)*nor.z) / nor.y) + pVtx[3].pos.y;
		}
	}

	if (D3DXVec3Cross(&dust, &(pVtx[2].pos - pVtx[0].pos), &(pos - pVtx[0].pos))->y <= 0.0f &&
		D3DXVec3Cross(&dust, &(pVtx[1].pos - pVtx[2].pos), &(pos - pVtx[2].pos))->y <= 0.0f &&
		D3DXVec3Cross(&dust, &(pVtx[0].pos - pVtx[1].pos), &(pos - pVtx[1].pos))->y <= 0.0f)
	{
		D3DXVECTOR3 vec1, vec2, nor;
		vec1 = pVtx[1].pos - pVtx[0].pos;
		vec2 = pVtx[2].pos - pVtx[0].pos;
		D3DXVec3Cross(&nor, &vec1, &vec2);
		if (nor.y != 0.0f)
		{
			return (-((pos.x - pVtx[0].pos.x)*nor.x + (pos.z - pVtx[0].pos.z)*nor.z) / nor.y) + pVtx[0].pos.y;
		}
	}

	//頂点バッファをアンロック
	m_pVtxbuff->Unlock();

	return 0.0f;
}