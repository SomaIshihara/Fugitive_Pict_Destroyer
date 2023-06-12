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
#include "object3D.h"

//=================================
//コンストラクタ（デフォルト）
//=================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority)
{
	//クリア
	m_pVtxbuff = NULL;
	m_pTexture = NULL;
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
}

#if 0
//=================================
//コンストラクタ（オーバーロード 位置向き）
//=================================
CObject3D::CObject3D(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight, int nPriority) : CObject(nPriority)
{
	//クリア
	m_pVtxbuff = NULL;
	m_pTexture = NULL;
}
#endif

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
		NULL)))
	{
		return E_FAIL;
	}

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\icon32.png",
		&m_pTexture);

	VERTEX_3D *pVtx;

	//バッファロック
	//頂点バッファのロックと頂点情報へのポインタを取得
	m_pVtxbuff->Lock(0, 0, (void **)&pVtx, 0);

	//頂点座標（相対座標）
	pVtx[0].pos = D3DXVECTOR3(-50.0f, 0.0f, 50.0f);
	pVtx[1].pos = D3DXVECTOR3(50.0f, 0.0f, 50.0f);
	pVtx[2].pos = D3DXVECTOR3(-50.0f, 0.0f, -50.0f);
	pVtx[3].pos = D3DXVECTOR3(50.0f, 0.0f, -50.0f);

	//法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
void CObject3D::Update(void)
{
	
}

//========================
//描画処理
//========================
void CObject3D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得
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
	pDevice->SetTexture(0, m_pTexture);

	//描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//========================
//生成処理
//========================
CObject3D* CObject3D::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight)
{
	CObject3D* pObj2D = NULL;

	if (pObj2D == NULL)
	{
		//オブジェクト2Dの生成
		pObj2D = new CObject3D();

		//初期化
		pObj2D->Init();

		return pObj2D;
	}
	else
	{
		return NULL;
	}
}