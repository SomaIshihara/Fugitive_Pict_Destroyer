//==========================================
//
//�J�����v���O�����̃w�b�_[camera.h]
//Author:�Ό��D�n
//
//==========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//�N���X���������̂�������ł�
class CCamera
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CCamera();
	~CCamera();
	
	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	//�ݒ菈��
	void SetCamera(void);
private:
	//�p�x�C��
	void FixPosV(void);
	void FixPosR(void);
	void FixRot(void);

	//�����o�ϐ�
	D3DXMATRIX m_mtxView;		//�r���[�}�g��
	D3DXMATRIX m_mtxProjection;	//�v���W�F�N�V�����}�g��
	D3DXVECTOR3 m_posV;			//���_
	D3DXVECTOR3 m_posR;			//�����_
	D3DXVECTOR3 m_vecU;			//��x�N�g��
	D3DXVECTOR3 m_rot;			//�p�x
};

#endif // !_CAMERA_H_
