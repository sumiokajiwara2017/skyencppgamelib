#pragma once

namespace sky { namespace lib {

/// <summary>
/// メモリープールクラス（指定の型(T)専用の高速簡易アロケーターのつもり）
/// </summary>
template< class T >
class CMemoryPool : public base::SkyRefObject
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CMemoryPool( u32 poolNum );

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CMemoryPool();

	/// <summary>
	/// 作成（ここで作成したインスタンスは当然ここのDeleteで消さなきゃダメ）
	/// </summary>
	inline T *Create();

	/// <summary>
	/// 削除
	/// </summary>
	inline void Delete( T *pValue );

	/// <summary>
	/// プール数取得
	/// </summary>
	u32 GetPoolNum() const;

private:

	/// <summary>
	/// プール数
	/// </summary>
	u32	m_PoolNum;

	/// <summary>
	/// バッファー単方向リスト
	/// </summary>
	SLinkListSize m_BufferList;
	
	/// <summary>
	/// インスタンス単方向リスト
	/// </summary>
	SLinkList< T > m_InstanceList;

	/// <summary>
	/// バッファ確保
	/// </summary>
	void CreateBuffer( u32 bufSize );
};

} }

#include "MemoryPool.inl"