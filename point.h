//======================================================
//
//�ړ��|�C���g�̃w�b�_[point.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _POINT_H_
#define _POINT_H_

//�|�C���g�N���X
class CPoint
{
public:
	//�R���X�g���N�^�E�f�X�g���N�^
	CPoint();
	~CPoint();

	//��{����
	HRESULT Init(void);
	static void Update(void);
	static void ReleaseAll(void);

	//����
	static CPoint* Create(const D3DXVECTOR3 pos);

	//�T��
	static CPoint* Search(const D3DXVECTOR3 pos, const CPoint* pNowPoint = NULL);

	//���X�g
	static CPoint* GetTop(void) { return m_pTop; }
	CPoint* GetNext(void) { return m_pNext; }

	//�擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }

private:
	static CPoint* m_pTop;	//�擪�I�u�W�F�N�g
	static CPoint* m_pCur;	//�Ō���I�u�W�F�N�g
	CPoint* m_pNext;		//���̃I�u�W�F�N�g
	CPoint* m_pPrev;		//�O�̃I�u�W�F�N�g
	static int m_nNumAll;	//����
	D3DXVECTOR3 m_pos;		//�ʒu
};

#endif // !_POINT_H_
