#pragma once

namespace sky { namespace lib {

/// <summary>
/// �P�������X�g�N���X
/// </summary>
template< class T >
class SLinkList : public base::SkyRefObject
{

public:

	/// <summary>
	/// �o�^
	/// </summary>
	void Link( T *pItem );

	/// <summary>
	/// �擾�i�擪�A�C�e���������N���X�g����O���ĕԂ��B�Ȃ��ꍇ��skyNull��Ԃ��j
	/// </summary>
	T *GetTop();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SLinkList();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SLinkList(){};

private:

	/// <summary>
	/// ���X�g�v�f�\����
	/// </summary>
	struct Item
	{
		/// <summary>
		/// ���X�g�̎��̗v�f
		/// </summary>
		Item *m_pNext;

		/// <summary>
		/// �R���X�g���N�^�Ɠ����̏���������֐�
		/// </summary>
		void Constructor()
		{
			m_pNext = skyNull;
		}
	};

	/// <summary>
	/// ���X�g�擪�A�h���X
	/// </summary>
	Item *m_pTop;

};

} }

#include "SLinkList.inl"