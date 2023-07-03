//======================================================
//
//建物処理[building.cpp]
//Author:石原颯馬
//
//======================================================
#include "building.h"
#include "renderer.h"
#include "texture.h"
#include "objectX.h"
#include "pict.h"
#include "score.h"
#include <stdio.h>
#include <assert.h>

//マクロ
#define BUILDING_DAMAGE_ALPHA	(0.65f)	//建物がダメージを食らった時の赤色の具合最大値
#define BUILDING_DAMAGE_TIME	(60)	//赤くする時間

//静的メンバ変数
CBuilding* CBuilding::m_apBuilding[MAX_OBJ];
CBuilding::BuildingParam CBuilding::m_aBuildingParam[MAX_OBJ];
int CBuilding::m_nNumAll = 0;

//=================================
//コンストラクタ（デフォルト）
//=================================
CBuilding::CBuilding()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apBuilding[cnt] == NULL)
		{//空っぽ
			m_apBuilding[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nEndurance = INT_ZERO;
	m_pos = VEC3_ZERO;
	m_rot = VEC3_ZERO;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;
	m_nIdx = 0;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CBuilding::CBuilding(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apBuilding[cnt] == NULL)
		{//空っぽ
			m_apBuilding[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_nIdx = nIdx;
	m_nEndurance = m_aBuildingParam[m_nIdx].nEndurance;
	m_pos = pos;
	m_rot = rot;
	m_fWidth = FLOAT_ZERO;
	m_fHeight = FLOAT_ZERO;
	m_fDepth = FLOAT_ZERO;

	//サイズ設定
	D3DXVECTOR3 vtxMin, vtxMax;
	CObjectX::Model* model = CObjectX::GetModel(m_nIdx);
	model->m_collision.GetVtx(&vtxMin, &vtxMax);
	m_fWidth = vtxMax.x - vtxMin.x;
	m_fHeight = vtxMax.y - vtxMin.y;
	m_fDepth = vtxMax.z - vtxMin.z;
}

//=================================
//デストラクタ
//=================================
CBuilding::~CBuilding()
{
}

//========================
//初期化処理
//========================
HRESULT CBuilding::Init(void)
{
	SetType(TYPE_BUILDING);	//建物とする

	return S_OK;
}

//========================
//終了処理
//========================
void CBuilding::Uninit(void)
{
	//建物クラス内での処理
	m_apBuilding[m_nID] = NULL;

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CBuilding::Update(void)
{
	if (m_fRedAlpha >= FLOAT_ZERO)
	{//まだ赤い
		//赤色具合を減らす
		m_fRedAlpha -= BUILDING_DAMAGE_ALPHA / BUILDING_DAMAGE_TIME;

		if (m_fRedAlpha < FLOAT_ZERO)
		{//赤くなくなった
			m_fRedAlpha = FLOAT_ZERO;
		}
	}
}

//========================
//描画処理
//========================
void CBuilding::Draw(void)
{
	if (m_nEndurance > 0)
	{//耐久が残っている
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

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//位置反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		//ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		//マテリアルデータへのポインタ取得
		CObjectX::Model* model = CObjectX::GetModel(m_nIdx);
		pMat = (D3DXMATERIAL*)model->m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)model->m_dwNumMatModel; nCntMat++)
		{
			//マテリアル設定
			D3DMATERIAL9 changeMat = pMat[nCntMat].MatD3D;

			//ダメージ状態なら赤追加
			if (m_fRedAlpha > FLOAT_ZERO)
			{
				changeMat.Diffuse.r = 1.0f * m_fRedAlpha + changeMat.Diffuse.r * (1.0f - m_fRedAlpha);
				changeMat.Diffuse.g = 0.0f * m_fRedAlpha + changeMat.Diffuse.g * (1.0f - m_fRedAlpha);
				changeMat.Diffuse.b = 0.0f * m_fRedAlpha + changeMat.Diffuse.b * (1.0f - m_fRedAlpha);
			}

			pDevice->SetMaterial(&changeMat);

			//テクスチャ設定
			pDevice->SetTexture(0, pTexture->GetAddress(model->m_pIdxtexture[nCntMat]));

			//モデル描画
			model->m_pMesh->DrawSubset(nCntMat);
		}

		//マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//========================
//生成処理
//========================
CBuilding* CBuilding::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nIdx)
{
	CBuilding* pBuilding = NULL;

	if (pBuilding == NULL)
	{
		//オブジェクト2Dの生成
		pBuilding = new CBuilding(pos, rot, nIdx);

		//初期化
		pBuilding->Init();

		return pBuilding;
	}
	else
	{
		return NULL;
	}
}

//========================
//ダメージ付与処理
//========================
void CBuilding::AddDamage(int nDamage)
{
	m_nEndurance -= nDamage;
	if (m_nEndurance <= 0)
	{//全壊
		m_nEndurance = 0;	//耐久0にする

		for (int cnt = 0; cnt < MAX_OBJ; cnt++)
		{//全オブジェクト見る
			CPictDestroyer* pPict = CPictDestroyer::GetPict(cnt);	//オブジェクト取得

			if (pPict != NULL)	//ヌルチェ
			{//なんかある
				if (pPict->GetTarget() == this)
				{//自分がターゲット
					pPict->UnsetTarget();	//ターゲット外す
				}
			}
		}
	}

	//赤くする
	m_fRedAlpha = BUILDING_DAMAGE_ALPHA;
}

//========================
//パラメータ読み込み
//========================
void CBuilding::LoadParam(const char* pPath)
{
	FILE* pFile;	//ファイルポインタ

	pFile = fopen(pPath, "r");

	if (pFile != NULL)
	{//読み込めた
		int cnt = 0;		//カウンタ
		char aData[128];	//読み取ったデータ
		char* pSprit;		//分割したもの
		int nSignificant;	//有効数字
		int nPower;			//べき乗

		fgets(&aData[0],128,pFile);	//ごみ読み取り

		//データ読み取り
		while (1)
		{
			fgets(&aData[0], 128, pFile);	//全文読み取り

			if (feof(pFile))
			{//いや、これはEOFだ
				break;
			}

			//建物名
			pSprit = strtok(&aData[0], ",");
			strcpy(&m_aBuildingParam[cnt].aName[0], pSprit);

			//価値
			pSprit = strtok(NULL, ",");
			nSignificant = atoi(pSprit);
			pSprit = strtok(NULL, ",");
			nPower = atoi(pSprit);
			m_aBuildingParam[cnt].nValue = (long long)nSignificant * pow(10, nPower);

			//耐久
			pSprit = strtok(NULL, ",");
			nSignificant = atoi(pSprit);
			pSprit = strtok(NULL, ",");
			nPower = atoi(pSprit);
			m_aBuildingParam[cnt].nEndurance = nSignificant * (int)pow(10, nPower);
			
			cnt++;	//カウンタ増やす
		}
	}
	else
	{
		assert(false);
	}
}