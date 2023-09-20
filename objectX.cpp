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
#include "building.h"
#include "koban.h"
#include "xmodel.h"
#include <assert.h>

//マクロ
#define PATH_LENGTH	(256)

//静的メンバ変数
CObjectX* CObjectX::m_pTop = nullptr;
CObjectX* CObjectX::m_pCur = nullptr;
int CObjectX::m_nNumAll = 0;

using namespace std;

//=================================
//コンストラクタ（デフォルト）
//=================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	//クリア
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;

	if (m_pCur == nullptr)
	{//最後尾がいない（すなわち先頭もいない）
		m_pTop = this;		//俺が先頭
		m_pPrev = nullptr;		//前後誰もいない
		m_pNext = nullptr;
	}
	else
	{//最後尾がいる
		m_pPrev = m_pCur;		//最後尾が自分の前のオブジェ
		m_pCur->m_pNext = this;	//最後尾の次のオブジェが自分
		m_pNext = nullptr;			//自分の次のオブジェはいない
	}
	m_pCur = this;				//俺が最後尾
	m_bExclusion = false;		//生きてる
	m_pModel = nullptr;
	m_nNumAll++;
}

//=================================
//コンストラクタ（オーバーロード 位置向き）
//=================================
CObjectX::CObjectX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel, int nPriority) : CObject(nPriority)
{
	//クリア
	m_pos = pos;
	m_rot = rot;

	if (m_pCur == nullptr)
	{//最後尾がいない（すなわち先頭もいない）
		m_pTop = this;		//俺が先頭
		m_pPrev = nullptr;		//前後誰もいない
		m_pNext = nullptr;
	}
	else
	{//最後尾がいる
		m_pPrev = m_pCur;		//最後尾が自分の前のオブジェ
		m_pCur->m_pNext = this;	//最後尾の次のオブジェが自分
		m_pNext = nullptr;			//自分の次のオブジェはいない
	}
	m_pCur = this;				//俺が最後尾
	m_bExclusion = false;		//生きてる
	m_pModel = pModel;			//モデル設定

	//サイズ設定
	D3DXVECTOR3 vtxMin, vtxMax;
	m_pModel->GetCollision().GetVtx(&vtxMin, &vtxMax);
	m_fWidth = vtxMax.x - vtxMin.x;
	m_fHeight = vtxMax.y - vtxMin.y;
	m_fDepth = vtxMax.z - vtxMin.z;

	m_nNumAll++;	//オブジェクト個数増やす
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
	SetType(TYPE_OBJECT);
	return S_OK;
}

//========================
//終了処理
//========================
void CObjectX::Uninit(void)
{
	m_bExclusion = true;		//除外予定

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
	pMat = (D3DXMATERIAL*)m_pModel->GetBufMat()->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_pModel->GetNumMat(); nCntMat++)
	{
		//マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャ設定
		pDevice->SetTexture(0, pTexture->GetAddress(m_pModel->GetIdxTexture()[nCntMat]));

		//モデル描画
		m_pModel->GetMesh()->DrawSubset(nCntMat);
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//========================
//生成処理
//========================
CObjectX* CObjectX::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel)
{
	CObjectX* pObjX = nullptr;

	if (pObjX == nullptr)
	{
		//オブジェクト2Dの生成
		pObjX = new CObjectX(pos, rot, pModel);

		//初期化
		pObjX->Init();

		return pObjX;
	}
	else
	{
		return nullptr;
	}
}

//========================
//オブジェクト単位除外処理
//========================
void CObjectX::Delete(CXModel * pTarget)
{
	CObjectX* pObject = m_pTop;	//先頭を入れる

	while (pObject != nullptr)
	{//最後尾まで回し続ける
		CObjectX* pObjectNext = pObject->m_pNext;	//次のオブジェ保存

		if (pObject->GetModel() == pTarget)
		{
			pObject->Uninit();	//除外
		}

		pObject = pObjectNext;	//次を入れる
	}
}

//========================
//データ読み込み
//========================
CObjectX::LOADRESULT CObjectX::LoadData(const char * pPath)
{
	FILE* pFile;
	BINCODE code;
	bool bRead = false;
	char** ppFilePath = nullptr;
	int nReadedModel = 0;

	pFile = fopen(pPath, "rb");

	if (pFile != nullptr)
	{//開けた
		while (1)
		{
			fread(&code, sizeof(BINCODE), 1, pFile);

			//文字列チェック
			if (code == BIN_CODE_SCRIPT)
			{//読み取り開始
				bRead = true;
			}
			else if (code == BIN_CODE_END_SCRIPT)
			{//読み取り終了
				bRead = false;
				break;
			}
			else if (bRead == true)
			{//読み取り
				if (code == BIN_CODE_TEXTURE_FILENAME)
				{
					char aPath[PATH_LENGTH];
					fread(&aPath[0], sizeof(char), PATH_LENGTH, pFile);
					CManager::GetTexture()->Load(&aPath[0]);
				}
				else if (code == BIN_CODE_MODEL_NUM)
				{
					int nNumAll;
					fread(&nNumAll, sizeof(int), 1, pFile);
					ppFilePath = new char*[nNumAll];
				}
				else if (code == BIN_CODE_MODEL_FILENAME)
				{
					char aPath[PATH_LENGTH];
					fread(&aPath[0], sizeof(char), PATH_LENGTH, pFile);
					CXModel::Load(&aPath[0]);

					//モデルパス読み取り（引き出し用に使う）
					ppFilePath[nReadedModel] = new char[strlen(&aPath[0]) + 1];
					strcpy(ppFilePath[nReadedModel], &aPath[0]);
					nReadedModel++;
				}
				else if (code == BIN_CODE_MODELSET)
				{
					D3DXVECTOR3 pos, rot;
					int nModelNum = -1;
					CXModel* pModel = nullptr;
					fread(&pos, sizeof(D3DXVECTOR3), 1, pFile);
					fread(&rot, sizeof(D3DXVECTOR3), 1, pFile);
					fread(&nModelNum, sizeof(int), 1, pFile);
					pModel = CXModel::Load(ppFilePath[nModelNum]);

					//state設定（破壊可能・交番設定）
					int nState = CManager::INT_ZERO;
					fread(&nState, sizeof(int), 1, pFile);

					//生成
					if (nState == STATE_NONE)
					{//オブジェクト
						CObjectX::Create(pos, rot, pModel);
					}
					else if (nState == STATE_BREAKABLE)
					{//建物
						CBuilding* pObject = CBuilding::Create(pos, rot, pModel);

						if (nState == STATE_BREAKABLE)
						{//破壊可能設定の場合
						 //個別設定
							bool bUnique;
							fread(&bUnique, sizeof(bool), 1, pFile);
							pObject->SetUnique(bUnique);

							if (bUnique == false)
							{//個別設定なし
								int nLv;
								fread(&nLv, sizeof(int), 1, pFile);
								pObject->SetLv(nLv);
							}
							else
							{//個別設定あり
								int nNum;

								//価値有効数字
								fread(&nNum, sizeof(int), 1, pFile);
								pObject->SetSigValue(nNum);

								//価値べき乗
								fread(&nNum, sizeof(int), 1, pFile);
								pObject->SetPowValue(nNum);

								//耐久有効数字
								fread(&nNum, sizeof(int), 1, pFile);
								pObject->SetSigEndurance(nNum);

								//耐久べき乗
								fread(&nNum, sizeof(int), 1, pFile);
								pObject->SetPowEndurance(nNum);

								//経験値
								fread(&nNum, sizeof(int), 1, pFile);
								pObject->SetExp(nNum);
							}
						}
					}
					else if (nState == STATE_KOBAN)
					{//交番
						CKoban::Create(pos, rot, pModel);
					}
				}
			}
		}

		fclose(pFile);
		return RES_OK;
	}
	else
	{//開けなかった（ファイルないんじゃね？）
		return RES_ERR_FILE_NOTFOUND;
	}
}

//========================
//データ書き込み
//========================
CObjectX::LOADRESULT CObjectX::SaveData(const char * pPath)
{
	FILE* pFile;

	pFile = fopen(pPath, "wb");

	if (pFile != nullptr)
	{//普通に開けた
		//開始コード書き込み
		BINCODE code = BIN_CODE_SCRIPT;
		fwrite(&code, sizeof(BINCODE), 1, pFile);

		//モデル個数書き込み
		code = BIN_CODE_MODEL_NUM;
		int nNumAll = CXModel::GetNumAll();
		fwrite(&code, sizeof(BINCODE), 1, pFile);
		fwrite(&nNumAll, sizeof(int), 1, pFile);

		//モデルファイルパス書き込み
		code = BIN_CODE_MODEL_FILENAME;
		CXModel* pModel = CXModel::GetTop();	//リスト書き込み体制
		while (pModel != nullptr)
		{
			CXModel* pObjectNext = pModel->GetNext();

			fwrite(&code, sizeof(BINCODE), 1, pFile);				//コード
			fwrite(pModel->GetPath(), sizeof(char), 256, pFile);	//データ

			pModel = pObjectNext;
		}

		//モデル配置情報書き込み
		code = BIN_CODE_MODELSET;
		CObjectX* pObject = CObjectX::GetTop();
		while (pObject != nullptr)
		{
			CObjectX* pObjectNext = pObject->GetNext();

			fwrite(&code, sizeof(BINCODE), 1, pFile);				//コード
			fwrite(&pObject->GetPos(), sizeof(D3DXVECTOR3), 1, pFile);
			fwrite(&pObject->GetRot(), sizeof(D3DXVECTOR3), 1, pFile);

			//モデル種類番号化
			int nModelNum = 0;
			CXModel* pModel = CXModel::GetTop();
			while (pModel != nullptr && pModel != pObject->GetModel())
			{
				pModel = pModel->GetNext();
				nModelNum++;
			}
			fwrite(&nModelNum, sizeof(int), 1, pFile);

			//state設定（破壊可能・交番設定）
			int nState = CManager::INT_ZERO;
			if (pObject->GetBreakable() == true)
			{//破壊可能設定
				nState = STATE_BREAKABLE;
			}
			else if (pObject->GetStateKoban() == true)
			{//交番設定
				nState = STATE_KOBAN;
			}
			fwrite(&nState, sizeof(int), 1, pFile);
#if 0
			if (nState == STATE_BREAKABLE)
			{//破壊可能設定の場合
			 //個別設定
				bool bUnique = pObject->GetUnique();
				fwrite(&bUnique, sizeof(bool), 1, pFile);

				if (bUnique == false)
				{//個別設定なし
					int nLv = pObject->GetLv();
					fwrite(&nLv, sizeof(int), 1, pFile);
				}
				else
				{//個別設定あり
					int nNum;

					//価値有効数字
					nNum = pObject->GetSigValue();
					fwrite(&nNum, sizeof(int), 1, pFile);

					//価値べき乗
					nNum = pObject->GetPowValue();
					fwrite(&nNum, sizeof(int), 1, pFile);

					//耐久有効数字
					nNum = pObject->GetSigEndurance();
					fwrite(&nNum, sizeof(int), 1, pFile);

					//耐久べき乗
					nNum = pObject->GetPowEndurance();
					fwrite(&nNum, sizeof(int), 1, pFile);

					//経験値
					nNum = pObject->GetExp();
					fwrite(&nNum, sizeof(int), 1, pFile);
				}
			}
#endif

			pObject = pObjectNext;
		}

		//終了コード書き込み
		code = BIN_CODE_END_SCRIPT;
		fwrite(&code, sizeof(BINCODE), 1, pFile);

		fclose(pFile);
		return RES_OK;
	}
	else
	{//なぜか開けなかった（なんで？）
		return RES_ERR_FILE_NOTFOUND;
	}
}

//========================
//除外処理
//========================
void CObjectX::Exclusion(void)
{
	CObjectX* pObject = m_pTop;	//先頭を入れる

	while (pObject != nullptr)
	{//最後尾まで回し続ける
		CObjectX* pObjectNext = pObject->m_pNext;	//次のオブジェ保存

		if (pObject->m_bExclusion == true)
		{//死亡フラグが立ってる
			if (pObject->m_pPrev != nullptr)
			{//前にオブジェがいる
				pObject->m_pPrev->m_pNext = pObject->m_pNext;	//前のオブジェの次のオブジェは自分の次のオブジェ
			}
			if (pObject->m_pNext != nullptr)
			{
				pObject->m_pNext->m_pPrev = pObject->m_pPrev;	//次のオブジェの前のオブジェは自分の前のオブジェ
			}

			if (m_pCur == pObject)
			{//最後尾でした
				m_pCur = pObject->m_pPrev;	//最後尾を自分の前のオブジェにする
			}
			if (m_pTop == pObject)
			{
				m_pTop = pObject->m_pNext;	//先頭を自分の次のオブジェにする
			}

			//成仏
			m_nNumAll--;	//総数減らす
		}

		pObject = pObjectNext;	//次を入れる
	}
}