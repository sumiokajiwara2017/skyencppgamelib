#pragma once

namespace sky { namespace lib { namespace math {

class CGraphEdge;
typedef CList< u32 , CGraphEdge > CGraphEdgeList;
typedef CVectorPrimitive< CGraphEdgeList > CGraphEdgeListVector;

/// <summary>
/// グラフのエッジクラス
/// </summary>
class CGraphEdge : public base::SkyObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CGraphEdge();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CGraphEdge( graphIndex fromIndex , graphIndex toIndex , dectype cost );
	inline CGraphEdge( graphIndex fromIndex , graphIndex toIndex );
	inline CGraphEdge();

	/// <summary>
	/// 開始インデックスの取得／設定
	/// </summary>
	inline graphIndex GetFromIndex() const;
	inline void       SetFromIndex( graphIndex index );

	/// <summary>
	/// 到達インデックスの取得／設定
	/// </summary>
	inline graphIndex GetToIndex() const;
	inline void       SetToIndex( graphIndex index );

	/// <summary>
	/// コスト
	/// </summary>
	inline dectype GetCost() const;
	inline void    SetCost( dectype cost );

protected:

	/// <summary>
	/// グラフの開始インデックス
	/// </summary>
	graphIndex m_FromIndex;

	/// <summary>
	/// グラフの到達インデックス
	/// </summary>
	graphIndex m_ToIndex;

	/// <summary>
	/// コスト
	/// </summary>
	dectype m_Cost;

};

} } }

#include "GraphEdge.inl"