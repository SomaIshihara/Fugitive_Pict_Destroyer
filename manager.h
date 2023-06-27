//======================================================
//
//�}�l�[�W���̃w�b�_[manager.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "main.h"

//�O���錾
class CInputKeyboard;
class CInputMouse;
class CRenderer;
class CPlayer;
class CDebugProc;
class CSound;
class CCamera;
class CLight;
class CTexture;
class CPlayer;
class CMeshField;
class CButton3D;

//�}�N��
#define INT_ZERO	(0)							//int�^��0
#define FLOAT_ZERO	(0.0f)						//float�^��0
#define VEC2_ZERO	D3DXVECTOR2(0.0f,0.0f)		//vec2�^��0
#define VEC3_ZERO	D3DXVECTOR3(0.0f,0.0f,0.0f)	//vec3�^��0

class CManager
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CManager();
	~CManager();

	//��{����
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	//FPS�v��
	void CheckFPS(DWORD dwCurrentTime, DWORD dwExecLastTime);

	//�擾
	static CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse; }
	static CSound* GetSound(void) { return m_pSound; }
	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CDebugProc* GetDebProc(void) { return m_pDebProc; }
	static CCamera* GetCamera(void) { return m_pCamera; }
	static CLight* GetLight(void) { return m_pLight; }
	static CTexture* GetTexture(void) { return m_pTexture; }
	//��
	static CMeshField* GetMeshField(void) { return m_pMeshField; }

private:
	//�Ĕz�u
	void ResetObj(void);

	//�ϐ�
	static CInputKeyboard* m_pInputKeyboard;	//�L�[�{�[�h
	static CInputMouse* m_pInputMouse;			//�}�E�X
	static CSound* m_pSound;					//�T�E���h
	static CRenderer* m_pRenderer;				//�����_���[
	static CDebugProc* m_pDebProc;				//�f�o�b�O
	static CPlayer* m_pPlayer;					//�v���C���[
	static CCamera* m_pCamera;					//�J����
	static CLight* m_pLight;					//���C�g
	static CTexture* m_pTexture;				//�e�N�X�`��
	static CButton3D* m_pButton3D;				//3D�{�^��
	//��
	static CMeshField* m_pMeshField;			//���F���b�V���t�B�[���h
	//��
	static int m_nFPS;							//FPS
	static DWORD m_dwFrameCount;				//�񂵂��t���[����
};

#endif