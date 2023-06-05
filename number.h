//======================================================
//
//���������̃w�b�_[number.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"
#include "object2D.h"

class CNumber : public CObject2D
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CNumber();																						//�f�t�H���g
	CNumber(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);	//�I�[�o�[���[�h�i�ʒu�����p�^�[���j
	~CNumber();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ݒ�
	void SetNumber(const int nSource, const int nDigit);	//���ƂƂȂ鐔���ƌ���������ƕ������Ă����

	//����
	static CNumber* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);

private:
};

#endif // !_OBJ_ANIM2D_H_