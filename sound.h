//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 石原颯馬
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//サウンドクラス
class CSound
{
public:
	//*****************************************************************************
	// サウンド一覧
	//*****************************************************************************
	typedef enum
	{
		SOUND_LABEL_BGM = 0,		//BGM
		SOUND_LABEL_SE_BUTTON,		//ボタンクリック
		SOUND_LABEL_SE_DISPATCH,	//出動
		SOUND_LABEL_SE_SELECT,		//建物・ピクト選択
		SOUND_LABEL_SE_COUNTDOWN,	//カウントダウン
		SOUND_LABEL_SE_ROCKET,		//ロケラン発射
		SOUND_LABEL_SE_GUN,			//銃発射
		SOUND_LABEL_SE_EXPLOSION,	//建物爆発
		SOUND_LABEL_SE_DAMAGE,		//ピクトダメージ
		SOUND_LABEL_SE_POPRESULT,	//リザルト出現
		SOUND_LABEL_MAX,
	} SOUND_LABEL;
	//*****************************************************************************

	//コンストラクタ・デストラクタ
	CSound();
	~CSound();

	//基本処理
	HRESULT Init(HWND hWnd);
	void Uninit(void);

	//流す処理
	HRESULT Play(SOUND_LABEL label);	//流す
	void Stop(SOUND_LABEL label);		//1つ止める
	void Stop(void);					//全部止める
	void Pause(SOUND_LABEL label);		//ポーズ
	void Restart(SOUND_LABEL label);	//再開
private:
	IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
};

#endif
