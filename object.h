//======================================================
//
//�I�u�W�F�N�g�����̃w�b�_[object.h]
//Author:�Ό��D�n
//
//======================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
#include "main.h"

#define MAX_OBJ	(256)	//�I�u�W�F�N�g�ő吔
#define DEATH_LIFE		(0)		//���S�̗�

//�I�u�W�F�N�g�N���X
class CObject
{
public:
	//��ޗ�
	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_PLAYER,
		TYPE_ENEMY,
		TYPE_BULLET,
		TYPE_MAX
	} TYPE;

	//�R���X�g���N�^�E�f�X�g���N�^
	CObject();
	virtual ~CObject();

	//��{����
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	//�S�I�u�W�F�N�g�n
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

	//�擾�E�ݒ�
	void SetType(TYPE type) { m_Type = type; }
	TYPE GetType(void) { return m_Type; }
	static CObject* GetObject(int nIdx) { return m_apObject[nIdx]; }

	//�������z�擾
	virtual D3DXVECTOR3 GetPos(void) = 0;
	virtual D3DXVECTOR3 GetRot(void) = 0;
	virtual float GetWidth(void) = 0;
	virtual float GetHeight(void) = 0;

protected:
	void Release(void);

private:
	static CObject* m_apObject[MAX_OBJ];	//�|�C���^
	static int m_nNumAll;					//����
	int m_nID;								//�������g��ID
	TYPE m_Type;							//���
};

#endif // !_OBJECT_H_
