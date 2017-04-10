#pragma once

namespace sky { namespace lib { namespace math {

class CGraphNode;
typedef CVectorPrimitive< CGraphNode > CGraphNodeVector;			//配列

/// <summary>
/// グラフのノードクラス
/// </summary>
class CGraphNode : public base::SkyObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CGraphNode();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CGraphNode();
	inline CGraphNode( graphIndex index );

	/// <summary>
	/// インデックスの取得設定
	/// </summary>
	inline graphIndex GetIndex() const;
	inline void SetIndex( graphIndex index );

protected:

	/// <summary>
	/// インデックス
	/// </summary>
	graphIndex m_Index;

};

} } }

#include "GraphNode.inl"