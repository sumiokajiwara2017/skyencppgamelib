#pragma once

namespace sky { namespace lib {

/// <summary>
/// 単方向リストクラス
/// </summary>
template< class T >
class SLinkList : public base::SkyRefObject
{

public:

	/// <summary>
	/// 登録
	/// </summary>
	void Link( T *pItem );

	/// <summary>
	/// 取得（先頭アイテムをリンクリストから外して返す。ない場合はskyNullを返す）
	/// </summary>
	T *GetTop();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SLinkList();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~SLinkList(){};

private:

	/// <summary>
	/// リスト要素構造体
	/// </summary>
	struct Item
	{
		/// <summary>
		/// リストの次の要素
		/// </summary>
		Item *m_pNext;

		/// <summary>
		/// コンストラクタと同等の処理をする関数
		/// </summary>
		void Constructor()
		{
			m_pNext = skyNull;
		}
	};

	/// <summary>
	/// リスト先頭アドレス
	/// </summary>
	Item *m_pTop;

};

} }

#include "SLinkList.inl"