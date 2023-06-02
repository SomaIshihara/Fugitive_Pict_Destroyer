//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �Ό��D�n
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,	// �^�C�g��BGM
	SOUND_LABEL_BGM_GAME,		// �Q�[��BGM
	SOUND_LABEL_BGM_RESULT,		// ����
	SOUND_LABEL_SE_START,		// �X�^�[�g
	SOUND_LABEL_SE_SHOT,		// �e����
	SOUND_LABEL_SE_CLEAR,		// �N���A
	SOUND_LABEL_SE_OVER,		// ���s
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void PauseSound(SOUND_LABEL label);
void RestartSound(SOUND_LABEL label);

//�N���X���������̂�������ł�
class CSound
{
public:
	//�T�E���h���
	typedef enum
	{
		SOUND_LABEL_BGM_TITLE = 0,	// �^�C�g��BGM
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	//�R���X�g���N�^�E�f�X�g���N�^
	CSound();
	~CSound();

	//��{����
	HRESULT Init(HWND hWnd);
	void Uninit(void);

	//��������
	HRESULT Play(SOUND_LABEL label);
	void Stop(SOUND_LABEL label);
	static void Stop(SOUND_LABEL label);
	void Pause(SOUND_LABEL label);
	void Restart(SOUND_LABEL label);
private:
	static IXAudio2 *m_pXAudio2;							// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	static IXAudio2MasteringVoice *m_pMasteringVoice;		// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	BYTE *g_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	DWORD g_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
};

#endif
