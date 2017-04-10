#pragma once

namespace sky { namespace lib {

/// <summary>
/// メモリープールクラス（指定のサイズ専用の高速簡易アロケーターのつもり）
/// </summary>
class CMemoryPoolSize : public base::SkyRefObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CMemoryPoolSize( u32 blockSize , u32 poolNum );

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CMemoryPoolSize();

	/// <summary>
	/// 作成（ここで作成したインスタンスは当然ここのDeleteで消さなきゃダメ）
	/// </summary>
	inline void *Create();

	/// <summary>
	/// 削除
	/// </summary>
	inline void Delete( void *pValue );

private:

	/// <summary>
	/// バッファサイズ
	/// </summary>
	u32	m_PoolNum;

	/// <summary>
	/// ブロックサイズ
	/// </summary>
	u32 m_uiBlockSize;

	/// <summary>
	/// バッファー単方向リスト
	/// </summary>
	SLinkListSize m_BufferList;
	
	/// <summary>
	/// インスタンス単方向リスト
	/// </summary>
	SLinkListSize m_InstanceList;

	/// <summary>
	/// バッファ確保
	/// </summary>
	inline void CreateBuffer( u32 bufSize );
};

} }

#include "MemoryPoolSize.inl"