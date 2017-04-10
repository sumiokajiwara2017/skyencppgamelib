#pragma once

namespace sky { namespace lib { namespace thread {

/// <summary>
///	�N���X�ɃX���b�h�Ή��^��Ή��@�\��t������
/// </summary>
namespace ThreadingModels
{
	/// <summary>
	///	�X���b�h��Ή��N���X���p������
	/// </summary>
	class CSingleThread
	{

	protected:

		/// <summary>
		///	�R���X�g���N�^
		/// </summary>
		inline CSingleThread() : m_pCs( skyNull ){} //�N���e�B�J���Z�N�V������skyNull�̏ꍇ�͉������Ȃ����Ƃ��Ӗ�����

		/// <summary>
		///	�N���e�B�J���Z�N�V����
		/// </summary>
		CriticalSection *m_pCs;

	};

	/// <summary>
	///	�X���b�h�Ή��N���X���p������
	/// </summary>
	class CMultiThread
	{

	protected:

		/// <summary>
		///	�R���X�g���N�^
		/// </summary>
		inline CMultiThread() :	m_pCs( CriticalSection::Create() ){}

		/// <summary>
		///	�f�X�g���N�^
		/// </summary>
		inline ~CMultiThread()
		{
			CriticalSection::Delete( m_pCs );
		}

		/// <summary>
		///	�N���e�B�J���Z�N�V����
		/// </summary>
		CriticalSection *m_pCs;
	};
}

} } }