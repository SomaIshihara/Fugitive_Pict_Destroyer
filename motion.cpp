//======================================================
//
//モーション処理[motion..cpp]
//Author:石原颯馬
//
//======================================================
#include "motion.h"
#include "model.h"
#include "manager.h"

//========================
//コンストラクタ
//========================
CMotion::CMotion()
{
	for (int cntInfo = 0; cntInfo < INFO_NUM; cntInfo++)
	{
		for (int cntKeyInfo = 0; cntKeyInfo < KEYINFO_NUM; cntKeyInfo++)
		{
			m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_pKey = nullptr;
			m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_nFrame = 0;
		}
		m_aInfo[cntInfo].m_bLoop = false;
		m_aInfo[cntInfo].m_nNumKey = CManager::INT_ZERO;
	}
}

//========================
//デストラクタ
//========================
CMotion::~CMotion()
{
}

//========================
//初期化処理
//========================
HRESULT CMotion::Init(void)
{
	return S_OK;
}

//========================
//終了処理
//========================
void CMotion::Uninit(void)
{
	for (int cntInfo = 0; cntInfo < INFO_NUM; cntInfo++)
	{
		for (int cntKeyInfo = 0; cntKeyInfo < KEYINFO_NUM; cntKeyInfo++)
		{
			if (m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_pKey != nullptr)
			{
				delete m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_pKey;
				m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_pKey = nullptr;
			}
		}
	}
}

//========================
//モーション設定処理
//========================
void CMotion::Set(int nType)
{
	//モーション設定
	m_nType = nType;

	//初期設定
	m_bLoop = m_aInfo[m_nType].m_bLoop;
	m_nNumKey = m_aInfo[m_nType].m_nNumKey;
	m_nKey = CManager::INT_ZERO;
	m_nCounter = CManager::INT_ZERO;
	m_bFinish = false;
}

//========================
//更新処理
//========================
void CMotion::Update(void)
{
	//今のキーと次のキーを入れておく
	int nNowKey = m_nKey;
	int nNextKey = (m_nKey + 1) % m_nNumKey;

	for (int CntModel = 0; CntModel < m_nNumModel; CntModel++)
	{
		//オフセットを入れておく
		D3DXVECTOR3 posMotioned = m_ppModel[CntModel]->GetPosOffset();
		D3DXVECTOR3 rotMotioned = m_ppModel[CntModel]->GetRotOffset();

		//差分算出
		float posDiffX = m_aInfo[m_nType].m_aKeyInfo[nNextKey].m_pKey[CntModel].m_fPosX -
			m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fPosX;
		float posDiffY = m_aInfo[m_nType].m_aKeyInfo[nNextKey].m_pKey[CntModel].m_fPosY -
			m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fPosY;
		float posDiffZ = m_aInfo[m_nType].m_aKeyInfo[nNextKey].m_pKey[CntModel].m_fPosZ -
			m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fPosZ;
		float rotDiffX = m_aInfo[m_nType].m_aKeyInfo[nNextKey].m_pKey[CntModel].m_fRotX -
			m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fRotX;
		float rotDiffY = m_aInfo[m_nType].m_aKeyInfo[nNextKey].m_pKey[CntModel].m_fRotY -
			m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fRotY;
		float rotDiffZ = m_aInfo[m_nType].m_aKeyInfo[nNextKey].m_pKey[CntModel].m_fRotZ -
			m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fRotZ;

		//位置向き算出
		float posDemandX = m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fPosX +
			posDiffX * ((float)m_nCounter / m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_nFrame);
		float posDemandY = m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fPosY +
			posDiffY * ((float)m_nCounter / m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_nFrame);
		float posDemandZ = m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fPosZ +
			posDiffZ * ((float)m_nCounter / m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_nFrame);
		float rotDemandX = m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fRotX +
			rotDiffX * ((float)m_nCounter / m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_nFrame);
		float rotDemandY = m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fRotY +
			rotDiffY * ((float)m_nCounter / m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_nFrame);
		float rotDemandZ = m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_pKey[CntModel].m_fRotZ +
			rotDiffZ * ((float)m_nCounter / m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_nFrame);

		//パーツの位置向き設定
		posMotioned += D3DXVECTOR3(posDemandX, posDemandY, posDemandZ);
		rotMotioned += D3DXVECTOR3(rotDemandX, rotDemandY, rotDemandZ);
		m_ppModel[CntModel]->SetPosMotioned(posMotioned);
		m_ppModel[CntModel]->SetRotMotioned(rotMotioned);
	}
	m_nCounter++;

	//再生フレーム後の挙動
	if (m_nCounter == m_aInfo[m_nType].m_aKeyInfo[nNowKey].m_nFrame)
	{//再生フレーム数に達したら
	 //カウンターをリセットしてキーを一つ増やす
		m_nCounter = 0;
		m_nKey++;

		//キーの最大数に達したらループするか否かに応じて再設定する
		if (m_nKey == m_nNumKey)
		{
			if (m_bLoop == true)
			{//ループさせる
				m_nKey = 0;
			}
			else
			{//モーション終了
				m_bFinish = true;
			}
		}
	}
}

//========================
//モーション情報設定処理
//========================
void CMotion::SetInfo(INFO info)
{
	//情報入れてカウント増やす
	m_aInfo[m_nNumAll] = info;
	m_nNumAll++;
}

//========================
//モデル設定処理
//========================
void CMotion::SetModel(CModel** ppModel, int nNumModel)
{
	//モデル入れて総数も入れる
	m_ppModel = ppModel;
	m_nNumModel = nNumModel;

	//動的確保
	for (int cntInfo = 0; cntInfo < INFO_NUM; cntInfo++)
	{
		for (int cntKeyInfo = 0; cntKeyInfo < KEYINFO_NUM; cntKeyInfo++)
		{
			m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_pKey = new KEY[m_nNumModel];
			for (int cntKey = 0; cntKey < m_nNumModel; cntKey++)
			{
				m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_pKey[cntKey].m_fPosX = CManager::FLOAT_ZERO;
				m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_pKey[cntKey].m_fPosY = CManager::FLOAT_ZERO;
				m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_pKey[cntKey].m_fPosZ = CManager::FLOAT_ZERO;
				m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_pKey[cntKey].m_fRotX = CManager::FLOAT_ZERO;
				m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_pKey[cntKey].m_fRotY = CManager::FLOAT_ZERO;
				m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_pKey[cntKey].m_fRotZ = CManager::FLOAT_ZERO;
			}
			m_aInfo[cntInfo].m_aKeyInfo[cntKeyInfo].m_nFrame = 0;
		}
		m_aInfo[cntInfo].m_bLoop = false;
		m_aInfo[cntInfo].m_nNumKey = CManager::INT_ZERO;
	}
}
