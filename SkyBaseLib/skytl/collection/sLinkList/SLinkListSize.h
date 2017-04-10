#pragma once

namespace sky { namespace lib {

/// <summary>
/// �P�������X�g�N���X�i�T�C�Y�w��j
/// </summary>
class SLinkListSize : public base::SkyRefObject
{

public:

	/// <summary>
	/// �o�^(�n�����A�h���X�̃f�[�^�T�C�Y���R���X�g���N�^�Ŏw�肵���T�C�Y�ȏ�ł��邱�Ƃ́A�M���邵���Ȃ��B���̑���X�s�[�h���ł�j
	/// </summary>
	inline void Link( void *pItem );

	/// <summary>
	/// �擾�i�擪�A�C�e���������N���X�g����O���ĕԂ��B�Ȃ��ꍇ��skyNull��Ԃ��j
	/// </summary>
	inline void *GetTop();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline SLinkListSize( u32 blockSize );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~SLinkListSize(){};

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

	/// <summary>
	/// �u���b�N�T�C�Y
	/// </summary>
	u32 m_uiBlockSize;

};

} }

#include "SLinkListSize.inl"