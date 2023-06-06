//======================================================
//
//�^�C�}�[�����̃w�b�_[timer.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _TIMER_H_
#define _TIMER_H_
#include "main.h"
#include "object2D.h"

#define TIME_DIGIT	(3)	//�^�C�}�[�̌���

class CNumber;

class CTimer : public CObject
{
public:
	//�^�C�}�[�J�E���g���
	typedef enum
	{
		COUNT_UP = 0,
		COUNT_DOWN,
		COUNT_MAX
	} COUNT;

	//�R���X�g���N�^�E�f�X�g���N�^
	CTimer(int nPriority = PRIORITY_UI);				//�f�t�H���g
	CTimer(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight, int nPriority = PRIORITY_UI);	//�I�[�o�[���[�h
	~CTimer();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ǂݍ���
	static HRESULT Load(const char* pPath, int nPatWidth, int nPatHeight);
	static void Unload(void);

	//����
	static CTimer* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight);	//�I�u�W�F�N�g�𐶐�(fOneWidth,fOneHeight:1��������̃T�C�Y)

	//�擾�i�������z�֐��̊֌W��������Ă��邪�A�����ɃT�C�Y���͂���Ȃ��̂ł��ׂ�0��Ԃ��j
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return FLOAT_ZERO; }
	float GetHeight(void) { return FLOAT_ZERO; }

	//�ݒ�
	void Set(const int nScore, COUNT type);	//�^�C�}�[�ݒ�

private:
	void CutNumber(void);					//��������
	CNumber* m_pNumber[TIME_DIGIT];		//����
	static PatternTexture m_patTexture;		//�e�N�X�`��
	D3DXVECTOR3 m_pos;						//�ʒu�i1���ڂ̐����̒��S��z��j
	D3DXVECTOR3 m_rot;						//�����i1���ڂ̐����̒��S��z��j
	float m_fOneWidth;						//1��������̃T�C�Y��
	float m_fOneHeight;						//1��������̃T�C�Y����
	int m_nCounter;							//�^�C�}�[�J�E���^
	int m_nTime;							//�^�C��
	COUNT m_count;							//�J�E���g���
};

#endif // !_TIME_H_