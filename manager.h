//======================================================
//
//�}�l�[�W���̃w�b�_[manager.h]
//Author:�Ό��D�n
//
//======================================================
#include "main.h"

class CInputKeyboard;
class CRenderer;
class CPlayer;
class CDebugProc;

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
	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CPlayer* GetPlayer(void) { return m_pPlayer; }
	static CDebugProc* GetDebProc(void) { return m_pDebProc; }

private:
	static CInputKeyboard* m_pInputKeyboard;	//�L�[�{�[�h
	static CRenderer* m_pRenderer;				//�����_���[
	static CDebugProc* m_pDebProc;				//�f�o�b�O
	static CPlayer* m_pPlayer;					//�v���C���[
	static int m_nFPS;							//FPS
	static DWORD m_dwFrameCount;				//�񂵂��t���[����
};