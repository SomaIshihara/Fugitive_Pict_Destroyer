//======================================================
//
//���U���g�V�[���̃w�b�_[result.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//�O���錾
class CButton2D;

//���U���g�N���X
class CResult
{
public:
	//�Œ�l
	static const int HEADLINE_TIME;	//���o���\������
	static const int ATK_TIME;		//�U�����\������
	static const int DEST_TIME;		//�S�󐔕\������
	static const int VALUE_TIME;	//��Q���z�\������
	static const int RANK_TIME;		//�{�^���\������

	//�R���X�g���N�^�E�f�X�g���N�^
	CResult();
	~CResult();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	CButton2D* m_pButton;
	int m_nCounter;
};

#endif // !_RESULT_H_
