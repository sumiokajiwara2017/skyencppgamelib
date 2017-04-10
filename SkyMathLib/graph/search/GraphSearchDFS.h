#pragma once

namespace sky { namespace lib { namespace math {

static const u32 GRAPH_DFS_DEFAULT_SEARCH_USED_STACK_SIZE = 256;

/// <summary>
/// 深さ優先探索（Depth First Search）
/// </summary>
template< class graph_type >
class CGraphSearchDFS
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CGraphSearchDFS();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CGraphSearchDFS( const graph_type& graph ,
		                    graphIndex        sourceIndex ,
							graphIndex        targetIndex = GRAPH_INVALID_INDEX_NO ,
		                    u32               useSearchStacSize = GRAPH_DFS_DEFAULT_SEARCH_USED_STACK_SIZE
							);
    CGraphSearchDFS& operator=( const CGraphSearchDFS& ); //警告封じのため定義。実際このクラスを代入でまわしたりはできない。

	/// <summary>
	/// ターゲットが見つかったらTrueを返す
	/// </summary>
	skyBool Found() const;

	/// <summary>
	/// ソースからターゲットへの最短パスからなるノードインデックスのリストを返す
	/// </summary>
	GraphIndexList &GetPathToTarget( GraphIndexList &dest ) const;

protected:

	/// <summary>
	/// マーク種別
	/// </summary>
	enum eMark
	{
		eMark_visited = 0 ,
		eMark_unvisited ,
		no_parent_assigned ,
	};
	typedef CVectorPrimitive< eMark > MarkVector;

	/// <summary>
	/// エッジとノードの型を再定義する
	/// </summary>
	typedef typename graph_type::NodeType Node;
	typedef typename graph_type::EdgeType Edge;
	typedef CStack< const Edge * >        EdgePtrStack;

	/// <summary>
	/// 探索されるグラフへの参照
	/// </summary>
	const graph_type& m_Graph;

	/// <summary>
	/// インデックスの訪問記録を保存する配列
	/// </summary>
	MarkVector m_VisitedIndexVector;

	/// <summary>
	/// 訪問済みのノードのインデックスを格納した配列
	/// </summary>
	GraphIndexVector m_RouteIndexVector;

	/// <summary>
	/// 出発インデックス
	/// </summary>
	graphIndex m_SourceIndex;

	/// <summary>
	/// 目標インデックス
	/// </summary>
	graphIndex m_TargetIndex;

	/// <summary>
	/// ターゲットまでのパスが求まったらskyTrue
	/// </summary>
	skyBool m_IsTargetFound;

	/// <summary>
	/// 検索処理
	/// </summary>
	inline skyBool Search( u32 stacSize );
};

} } }

#include "GraphSearchDFS.inl"