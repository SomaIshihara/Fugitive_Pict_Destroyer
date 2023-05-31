//======================================================
//
//オブジェクト（2D）処理[object2D.cpp]
//Author:石原颯馬
//
//======================================================
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "object2D.h"

//=================================
//コンストラクタ（デフォルト）
//=================================
CObject2D::CObject2D()
{
	//クリア
	m_pVtxbuff = NULL;
	m_pTexture = NULL;
	m_fLength = INIT_ZERO;
	m_fAngle = INIT_ZERO;
	m_fWidth = 100.0f;
	m_fHeight = 100.0f;

	m_pos = D3DXVECTOR3(428.0f, 428.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=================================
//コンストラクタ（オーバーロード 位置向き）
//=================================
CObject2D::CObject2D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	//クリア
	m_pVtxbuff = NULL;
	m_pTexture = NULL;
	m_fLength = INIT_ZERO;
	m_fAngle = INIT_ZERO;
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	m_pos = pos;
	m_rot = rot;
}

//=================================
//デストラクタ
//=================================
CObject2D::~CObject2D()
{
}

//========================
//初期化処理
//========================
HRESULT CObject2D::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxbuff,
		NULL)))
	{
		return E_FAIL;
	}

	//長さ計算
	m_fLength = sqrtf(m_fWidth * m_fWidth + m_fHeight * m_fHeight) * 0.5f;

	//角度計算
	m_fAngle = atan2f(m_fWidth, m_fHeight);

	//位置設定
	if (FAILED(SetPos(m_pos)))
	{
		return E_FAIL;
	}

	//向き設定
	if (FAILED(SetRot(m_rot)))
	{
		return E_FAIL;
	}

	//頂点バッファのロックと頂点情報へのポインタを取得
	VERTEX_2D *pVtx;	//設定用ポインタ
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		return E_FAIL;
	}

	//rhw
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点バッファをアンロック
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		return E_FAIL;
	}

	//テクスチャ座標設定
	if (FAILED(SetTex(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f))))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================
//終了処理
//========================
void CObject2D::Uninit(void)
{
	////テクスチャ破棄
	//if (m_pTexture != NULL)
	//{//Releaseはしない（Releaseするとstaticのテクスチャを使えなくするから）
	//	m_pTexture = NULL;
	//}

	//頂点バッファの破棄
	if (m_pVtxbuff != NULL)
	{
		m_pVtxbuff->Release();
		m_pVtxbuff = NULL;
	}

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CObject2D::Update(void)
{
	
}

//========================
//描画処理
//========================
void CObject2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxbuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャ設定
	pDevice->SetTexture(0, m_pTexture);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================
//生成処理（テクスチャなし）
//========================
CObject2D* CObject2D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	CObject2D* pObj2D = NULL;

	if (pObj2D == NULL)
	{
		//オブジェクト2Dの生成
		pObj2D = new CObject2D(pos, rot, fWidth, fHeight);

		//初期化
		pObj2D->Init();

		//値設定
		pObj2D->m_fWidth = fWidth;
		pObj2D->m_fHeight = fHeight;

		return pObj2D;
	}
	else
	{
		return NULL;
	}
}

//========================
//位置設定
//========================
HRESULT CObject2D::SetPos(const D3DXVECTOR3 pos)
{
	//値設定
	m_pos = pos;

	return SetVtxPos();
}

//========================
//向き設定
//========================
HRESULT CObject2D::SetRot(const D3DXVECTOR3 rot)
{
	//値設定
	m_rot = rot;

	return SetVtxPos();
}

//========================
//テクスチャ座標設定
//========================
HRESULT CObject2D::SetTex(const D3DXVECTOR2 tex0, const D3DXVECTOR2 tex3)
{
	//頂点バッファのロックと頂点情報へのポインタを取得
	VERTEX_2D *pVtx;	//設定用ポインタ
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		return E_FAIL;
	}

	//テクスチャ座標
	pVtx[0].tex = tex0;
	pVtx[1].tex = D3DXVECTOR2(tex3.x, tex0.y);
	pVtx[2].tex = D3DXVECTOR2(tex0.x, tex3.y);
	pVtx[3].tex = tex3;

	//頂点バッファをアンロック
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//========================
//頂点座標設定
//========================
HRESULT CObject2D::SetVtxPos(void)
{
	//頂点バッファのロックと頂点情報へのポインタを取得
	VERTEX_2D *pVtx;	//設定用ポインタ
	if (FAILED(m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0)))
	{
		return E_FAIL;
	}

	//頂点座標の設定
	pVtx[0].pos.x = m_pos.x + sinf((m_rot.z * D3DX_PI) + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.y = m_pos.y + cosf((m_rot.z * D3DX_PI) + (-D3DX_PI + m_fAngle)) * m_fLength;
	pVtx[0].pos.z = 0.0f;
	pVtx[1].pos.x = m_pos.x + sinf((m_rot.z * D3DX_PI) + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.y = m_pos.y + cosf((m_rot.z * D3DX_PI) + (D3DX_PI - m_fAngle)) * m_fLength;
	pVtx[1].pos.z = 0.0f;
	pVtx[2].pos.x = m_pos.x + sinf((m_rot.z * D3DX_PI) + (0 - m_fAngle)) * m_fLength;
	pVtx[2].pos.y = m_pos.y + cosf((m_rot.z * D3DX_PI) + (0 - m_fAngle)) * m_fLength;
	pVtx[2].pos.z = 0.0f;
	pVtx[3].pos.x = m_pos.x + sinf((m_rot.z * D3DX_PI) + (0 + m_fAngle)) * m_fLength;
	pVtx[3].pos.y = m_pos.y + cosf((m_rot.z * D3DX_PI) + (0 + m_fAngle)) * m_fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロック
	if (FAILED(m_pVtxbuff->Unlock()))
	{
		return E_FAIL;
	}

	return S_OK;
}
