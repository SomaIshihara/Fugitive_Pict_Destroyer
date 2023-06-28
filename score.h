//======================================================
//
//�X�R�A�����̃w�b�_[score.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"
#include "object2D.h"

#define SCORE_DIGIT	(4)	//�X�R�A�̌���

class CNumber;
class CSymbol;

class CScore : public CObject
{
public:
	//�X�R�A�L����
	typedef enum
	{
		TYPE_HMILLION = 0,
		TYPE_TEN_HUNDRED,
		TYPE_PIC = 5,
		TYPE_MAX
	} SYMBOLTYPE;

	//�R���X�g���N�^�E�f�X�g���N�^
	CScore(int nPriority = PRIORITY_UI);				//�f�t�H���g
	CScore(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight, int nPriority = PRIORITY_UI);	//�I�[�o�[���[�h
	~CScore();

	//��{����
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//����
	static CScore* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fOneWidth, const float fOneHeight);	//�I�u�W�F�N�g�𐶐�(fOneWidth,fOneHeight:1��������̃T�C�Y)

	//�擾�i�������z�֐��̊֌W��������Ă��邪�A�����ɃT�C�Y���͂���Ȃ��̂ł��ׂ�0��Ԃ��j
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetWidth(void) { return FLOAT_ZERO; }
	float GetHeight(void) { return FLOAT_ZERO; }
	float GetDepth(void) { return FLOAT_ZERO; }
	int GetModelIdx(void) { return -1; }

	//�ݒ�
	static void Set(const int nScore);	//�X�R�A�ݒ�
	static void Add(const int nAdd);	//�X�R�A���Z

private:
	//�֐�
	void CutNumber(void);				//��������

	//�ϐ�
	CSymbol* m_pSymbolPic;						//�ʉ݋L��
	CNumber* m_pNumberHMillion[SCORE_DIGIT];	//���̐���
	CSymbol* m_pSymbolHMillion;					//��
	CNumber* m_pNumberTenHundred[SCORE_DIGIT];	//���̐���
	CSymbol* m_pSymbolTenHundred;				//��
	int m_nIdxTextureNumber;			//�e�N�X�`���ԍ��i�����j
	int m_nIdxtextureSymbol;			//�e�N�X�`���ԍ��i�L���j
	D3DXVECTOR3 m_pos;					//�ʒu�i1���ڂ̐����̒��S��z��j
	D3DXVECTOR3 m_rot;					//�����i1���ڂ̐����̒��S��z��j
	float m_fOneWidth;					//1��������̃T�C�Y��
	float m_fOneHeight;					//1��������̃T�C�Y����
	static long long m_nScore;			//�X�R�A
};

#endif // !_SCORE_H_