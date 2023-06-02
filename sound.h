//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 石原颯馬
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,	// タイトルBGM
	SOUND_LABEL_BGM_GAME,		// ゲームBGM
	SOUND_LABEL_BGM_RESULT,		// 結果
	SOUND_LABEL_SE_START,		// スタート
	SOUND_LABEL_SE_SHOT,		// 弾発射
	SOUND_LABEL_SE_CLEAR,		// クリア
	SOUND_LABEL_SE_OVER,		// 失敗
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void PauseSound(SOUND_LABEL label);
void RestartSound(SOUND_LABEL label);

//クラス化したものがこちらです
class CSound
{
public:
	//サウンド種類
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,	// タイトルBGM
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//コンストラクタ・デストラクタ
	CSound();
	~CSound();

	//基本処理
	HRESULT Init(HWND hWnd);
	void Uninit(void);

	//流す処理
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	static void Stop(SOUND_LABEL label);
	void Pause(SOUND_LABEL label);
	void Restart(SOUND_LABEL label);
private:
	static IXAudio2 *m_pXAudio2;							// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;		// マスターボイス
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	BYTE *g_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	DWORD g_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
};

#endif
