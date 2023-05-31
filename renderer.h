//======================================================
//
//�����_�����O�����̃w�b�_[renderer.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

class CRenderer
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CRenderer();
	~CRenderer();

	//��{����
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�擾
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }

private:
	LPDIRECT3D9 m_pD3D;						//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//Direct3D�f�o�C�X�ւ̃|�C���^
};

#endif // !_RENDERER_H_