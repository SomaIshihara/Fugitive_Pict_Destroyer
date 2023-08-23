//======================================================
//
//Xファイルモデルのヘッダ[xmodel.h]
//Author:石原颯馬
//
//======================================================
#include "xmodel.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//静的メンバ変数
CXModel* CXModel::m_pTop = NULL;
CXModel* CXModel::m_pCur = NULL;
int CXModel::m_nNumAll = 0;

//=================================
//コンストラクタ
//=================================
CXModel::CXModel()
{
	if (m_pCur == NULL)
	{//最後尾がいない（すなわち先頭もいない）
		m_pTop = this;		//俺が先頭
		m_pPrev = NULL;		//前後誰もいない
		m_pNext = NULL;
	}
	else
	{//最後尾がいる
		m_pPrev = m_pCur;		//最後尾が自分の前のオブジェ
		m_pCur->m_pNext = this;	//最後尾の次のオブジェが自分
		m_pNext = NULL;			//自分の次のオブジェはいない
	}
	m_pCur = this;				//俺が最後尾
	m_bExclusion = false;		//生きてる
	m_nNumAll++;
}

//=================================
//デストラクタ
//=================================
CXModel::~CXModel()
{
}

//=================================
//読み込み
//=================================
CXModel* CXModel::Load(const char* pPath)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイス取得
	CTexture* pTexture = CManager::GetTexture();						//テクスチャオブジェクト取得

	CXModel* pObject = m_pTop;	//先頭を入れる

	while (pObject != NULL)
	{//事前検知
		CXModel* pObjectNext = pObject->m_pNext;	//次のオブジェ保存

		if (pObject->m_pFilePath != NULL && strcmp(pObject->m_pFilePath, pPath) == 0)
		{
			return pObject;
		}

		pObject = pObjectNext;	//次を入れる
	}

	pObject = new CXModel;
	pObject->m_pIdxtexture = NULL;	//テクスチャ番号ポインタをNULLにする

	if (SUCCEEDED(D3DXLoadMeshFromX(
		pPath,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&pObject->m_pBuffMat,
		NULL,
		&pObject->m_dwNumMatModel,
		&pObject->m_pMesh)))
	{
		//テクスチャポインタ確保
		if (pObject->m_pIdxtexture == NULL)
		{//NULL
		 //テクスチャ番号配列確保
			pObject->m_pIdxtexture = new int[(int)pObject->m_dwNumMatModel];

			//当たり判定生成
			int nNumVtx;		//頂点数
			DWORD dwSizeFVF;	//頂点フォーマットのサイズ

			//頂点数を取得
			nNumVtx = pObject->m_pMesh->GetNumVertices();

			//頂点フォーマット
			dwSizeFVF = D3DXGetFVFVertexSize(pObject->m_pMesh->GetFVF());

			//頂点バッファロック
			BYTE *pVtxBuff;		//頂点バッファポインタ
			pObject->m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void **)&pVtxBuff);

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
			pObject->m_collision.SetVtx(vtxMin, vtxMax);

			//頂点バッファアンロック
			pObject->m_pMesh->UnlockVertexBuffer();

			//テクスチャ読み込み
			D3DXMATERIAL* pMat;	//マテリアルポインタ

			//マテリアル情報に対するポインタ取得
			pMat = (D3DXMATERIAL*)pObject->m_pBuffMat->GetBufferPointer();

			//テクスチャ読み込み
			for (int nCntTex = 0; nCntTex < (int)pObject->m_dwNumMatModel; nCntTex++)
			{
				pObject->m_pIdxtexture[nCntTex] = NULL;
				if (pMat[nCntTex].pTextureFilename != NULL)
				{//テクスチャあるよ
				 //テクスチャ読み込み
					pObject->m_pIdxtexture[nCntTex] = pTexture->Regist(pMat[nCntTex].pTextureFilename);
				}
				else
				{//ないよ
					pObject->m_pIdxtexture[nCntTex] = -1;	//テクスチャ取得時にNULLになるようにする
				}
			}
		}
		else
		{//おかしい
			assert(false);
		}

		//ファイルパス保存
		pObject->m_pFilePath = new char[strlen(pPath)+1];
		strcpy(pObject->m_pFilePath, pPath);

		//番号返す
		return pObject;
	}
	else
	{
		delete pObject;
		m_nNumAll--;
		return NULL;
	}

	return NULL;
}

//=================================
//破棄
//=================================
void CXModel::Unload(void)
{
	if (m_pPrev != NULL)
	{//前にオブジェがいる
		m_pPrev->m_pNext = m_pNext;	//前のオブジェの次のオブジェは自分の次のオブジェ
	}
	if (m_pNext != NULL)
	{
		m_pNext->m_pPrev = m_pPrev;	//次のオブジェの前のオブジェは自分の前のオブジェ
	}

	if (m_pCur == this)
	{//最後尾でした
		m_pCur = m_pPrev;	//最後尾を自分の前のオブジェにする
	}
	if (m_pTop == this)
	{
		m_pTop = m_pNext;	//先頭を自分の次のオブジェにする
	}

	//メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//テクスチャ番号破棄
	if (m_pIdxtexture != NULL)
	{
		delete[] m_pIdxtexture;
		m_pIdxtexture = NULL;
	}

	//ファイルパス破棄
	if (m_pFilePath != NULL)
	{
		delete[] m_pFilePath;
		m_pFilePath = NULL;
	}

	//成仏	
	delete this;	//自分自身破棄
	m_nNumAll--;	//総数減らす
}

//=================================
//全破棄
//=================================
void CXModel::UnloadAll(void)
{
	CXModel* pObject = m_pTop;	//先頭を入れる

	while (pObject != NULL)
	{//最後尾まで回し続ける
		CXModel* pObjectNext = pObject->m_pNext;	//次のオブジェ保存

		pObject->Unload();	//一つ消す

		pObject = pObjectNext;	//次を入れる
	}
}

//=================================
//リスト読み込み
//=================================
CXModel::LOADRESULT CXModel::LoadList(const char * pPath)
{
	FILE* pFile;

	pFile = fopen(pPath, "rb");

	if (pFile != NULL)
	{//開けた
		int nNumAll = 0;
		fread(&nNumAll, sizeof(int), 1, pFile);	//個数書き込み

		char aPath[256];

		for (int cnt = 0; cnt < nNumAll; cnt++)
		{
			fread(&aPath[0], sizeof(char), 256, pFile);	//ファイルパス読み込み
			CXModel::Load(&aPath[0]);
		}

		fclose(pFile);
		return RES_OK;
	}
	else
	{//開けなかった（ファイルないんじゃね？）
		return RES_ERR_LIST_NOTFOUND;
	}
}

//=================================
//リスト書き出し
//=================================
CXModel::LOADRESULT CXModel::SaveList(const char * pPath)
{
	FILE* pFile;

	pFile = fopen(pPath, "wb");

	if (pFile != NULL)
	{//普通に開けた
		int nNumAll = CXModel::GetNumAll();
		fwrite(&nNumAll, sizeof(int), 1, pFile);	//個数書き込み

		CXModel* pObject = m_pTop;	//リスト書き込み体制
		while (pObject != NULL)
		{
			CXModel* pObjectNext = pObject->m_pNext;

			fwrite(pObject->m_pFilePath, sizeof(char), 256, pFile);

			pObject = pObjectNext;
		}

		fclose(pFile);
		return RES_OK;
	}
	else
	{//なぜか開けなかった（なんで？）
		return RES_ERR_LIST_NOTFOUND;
	}
}
