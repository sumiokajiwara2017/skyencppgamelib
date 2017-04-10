#pragma once

namespace sky { namespace lib { namespace thread {

/// <summary>
///	クラスにスレッド対応／非対応機能を付加する
/// </summary>
namespace ThreadingModels
{
	/// <summary>
	///	スレッド非対応クラスが継承する
	/// </summary>
	class CSingleThread
	{

	protected:

		/// <summary>
		///	コンストラクタ
		/// </summary>
		inline CSingleThread() : m_pCs( skyNull ){} //クリティカルセクションがskyNullの場合は何もしないことを意味する

		/// <summary>
		///	クリティカルセクション
		/// </summary>
		CriticalSection *m_pCs;

	};

	/// <summary>
	///	スレッド対応クラスが継承する
	/// </summary>
	class CMultiThread
	{

	protected:

		/// <summary>
		///	コンストラクタ
		/// </summary>
		inline CMultiThread() :	m_pCs( CriticalSection::Create() ){}

		/// <summary>
		///	デストラクタ
		/// </summary>
		inline ~CMultiThread()
		{
			CriticalSection::Delete( m_pCs );
		}

		/// <summary>
		///	クリティカルセクション
		/// </summary>
		CriticalSection *m_pCs;
	};
}

} } }