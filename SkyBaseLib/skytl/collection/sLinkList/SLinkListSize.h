#pragma once

namespace sky { namespace lib {

/// <summary>
/// 単方向リストクラス（サイズ指定）
/// </summary>
class SLinkListSize : public base::SkyRefObject
{

public:

	/// <summary>
	/// 登録(渡されるアドレスのデータサイズがコンストラクタで指定したサイズ以上であることは、信じるしかない。その代わりスピードがでる）
	/// </summary>
	inline void Link( void *pItem );

	/// <summary>
	/// 取得（先頭アイテムをリンクリストから外して返す。ない場合はskyNullを返す）
	/// </summary>
	inline void *GetTop();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline SLinkListSize( u32 blockSize );

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~SLinkListSize(){};

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

	/// <summary>
	/// ブロックサイズ
	/// </summary>
	u32 m_uiBlockSize;

};

} }

#include "SLinkListSize.inl"