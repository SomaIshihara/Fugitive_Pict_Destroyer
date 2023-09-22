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
#include "picto.h"
#include "score.h"
#include "xmodel.h"
#include "koban.h"
#include "policemanager.h"
#include <stdio.h>
#include <assert.h>

//マクロ
#define BUILDING_DAMAGE_ALPHA	(0.65f)	//建物がダメージを食らった時の赤色の具合最大値
#define BUILDING_DAMAGE_TIME	(60)	//赤くする時間

//静的メンバ変数
CBuilding* CBuilding::m_apBuilding[MAX_OBJ];
int CBuilding::m_nNumAll = 0;

//=================================
//コンストラクタ（デフォルト）
//=================================
CBuilding::CBuilding()
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apBuilding[cnt] == nullptr)
		{//空っぽ
			m_apBuilding[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_pos = CManager::VEC3_ZERO;
	m_rot = CManager::VEC3_ZERO;
	m_fWidth = CManager::FLOAT_ZERO;
	m_fHeight = CManager::FLOAT_ZERO;
	m_fDepth = CManager::FLOAT_ZERO;
	m_nEndurance = CManager::INT_ZERO;

	//パラメータ
	m_bUnique = false;
	m_nLv = CManager::INT_ZERO;
	m_fSigValue = CManager::FLOAT_ZERO;
	m_nPowValue = CManager::INT_ZERO;
	m_fSigEndurance = CManager::FLOAT_ZERO;
	m_nPowEndurance = CManager::INT_ZERO;
	m_nExp = CManager::INT_ZERO;
}

//=================================
//コンストラクタ（オーバーロード）
//=================================
CBuilding::CBuilding(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel) : CObject(PRIORITY_DEFMM)
{
	for (int cnt = 0; cnt < MAX_OBJ; cnt++)
	{//すべて確認
		if (m_apBuilding[cnt] == nullptr)
		{//空っぽ
			m_apBuilding[cnt] = this;	//自分自身のポインタを登録
			m_nID = cnt;	//ID覚える
			m_nNumAll++;	//総数増やす
			break;
		}
	}
	m_pos = pos;
	m_rot = rot;
	m_fWidth = CManager::FLOAT_ZERO;
	m_fHeight = CManager::FLOAT_ZERO;
	m_fDepth = CManager::FLOAT_ZERO;
	m_pModel = pModel;

	//パラメータ
	m_bUnique = false;
	m_nLv = CManager::INT_ZERO;
	m_fSigValue = CManager::FLOAT_ZERO;
	m_nPowValue = CManager::INT_ZERO;
	m_fSigEndurance = CManager::FLOAT_ZERO;
	m_nPowEndurance = CManager::INT_ZERO;
	m_nExp = CManager::INT_ZERO;

	int nModelNum = 0;
	CXModel* pXModel = CXModel::GetTop();
	while (pXModel != nullptr && pXModel != m_pModel)
	{
		pXModel = pXModel->GetNext();
		nModelNum++;
	}

	//サイズ設定
	D3DXVECTOR3 vtxMin, vtxMax;
	m_pModel->GetCollision().GetVtx(&vtxMin, &vtxMax);
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
	m_apBuilding[m_nID] = nullptr;

	//建物個数減らす
	m_nNumAll--;

	//自分自身破棄
	Release();
}

//========================
//更新処理
//========================
void CBuilding::Update(void)
{
	if (m_fRedAlpha >= CManager::FLOAT_ZERO)
	{//まだ赤い
		//赤色具合を減らす
		m_fRedAlpha -= BUILDING_DAMAGE_ALPHA / BUILDING_DAMAGE_TIME;

		if (m_fRedAlpha < CManager::FLOAT_ZERO)
		{//赤くなくなった
			m_fRedAlpha = CManager::FLOAT_ZERO;
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
		pMat = (D3DXMATERIAL*)m_pModel->GetBufMat()->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_pModel->GetNumMat(); nCntMat++)
		{
			//マテリアル設定
			D3DMATERIAL9 changeMat = pMat[nCntMat].MatD3D;

			//ダメージ状態なら赤追加
			if (m_fRedAlpha > CManager::FLOAT_ZERO)
			{
				changeMat.Diffuse.r = 1.0f * m_fRedAlpha + changeMat.Diffuse.r * (1.0f - m_fRedAlpha);
				changeMat.Diffuse.g = 0.0f * m_fRedAlpha + changeMat.Diffuse.g * (1.0f - m_fRedAlpha);
				changeMat.Diffuse.b = 0.0f * m_fRedAlpha + changeMat.Diffuse.b * (1.0f - m_fRedAlpha);
			}

			pDevice->SetMaterial(&changeMat);

			//テクスチャ設定
			pDevice->SetTexture(0, pTexture->GetAddress(m_pModel->GetIdxTexture()[nCntMat]));

			//モデル描画
			m_pModel->GetMesh()->DrawSubset(nCntMat);
		}

		//マテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//========================
//生成処理
//========================
CBuilding* CBuilding::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CXModel* pModel)
{
	CBuilding* pBuilding = nullptr;

	if (pBuilding == nullptr)
	{
		//オブジェクト2Dの生成
		pBuilding = new CBuilding(pos, rot, pModel);

		//初期化
		pBuilding->Init();

		return pBuilding;
	}
	else
	{
		return nullptr;
	}
}

//========================
//レベル設定処理（と耐久設定処理）
//========================
void CBuilding::SetLv(const int nLv)
{
	m_nLv = nLv;
	m_nEndurance = HAVE_LIFE(nLv);
}

//========================
//耐久有効数字設定（と耐久設定処理）
//========================
void CBuilding::SetSigEndurance(const float fSignificant)
{
	m_fSigEndurance = fSignificant;
	CulcEndurance();
}

//========================
//耐久べき乗設定（と耐久設定処理）
//========================
void CBuilding::SetPowEndurance(const int nPower)
{
	m_nPowEndurance = nPower;
	CulcEndurance();
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
			CPictoDestroyer* pPicto = CPictoDestroyer::GetPicto(cnt);	//オブジェクト取得

			if (pPicto != nullptr)	//ヌルチェ
			{//なんかある
				if (pPicto->GetTargetObj() == this)
				{//自分がターゲット
					pPicto->UnsetTarget();
				}
			}
		}

		//経験値付与
		CPictoDestroyer::AddExp(DROP_EXP(m_nLv));
	}
	else
	{
		//ダメージ報告
		CPoliceManager::ReportDamage(m_nID);

		//赤くする
		m_fRedAlpha = BUILDING_DAMAGE_ALPHA;
	}
}

//========================
//耐久計算処理
//========================
void CBuilding::CulcEndurance(void)
{
	m_nEndurance = m_fSigEndurance * pow(10, m_nPowEndurance);
}