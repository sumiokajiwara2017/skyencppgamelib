#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// Ａ＊探索
/// </summary>
template < class graph_type , class heuristic >
class CGraphSearchAStar
{

public:

	/// <summary>
	/// 型の再定義
	/// </summary>
	typedef typename graph_type::EdgeType    Edge;
	typedef CVectorPrimitive< const Edge * > EdgePtrVector;

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CGraphSearchAStar();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CGraphSearchAStar( const graph_type& graph ,
		                            graphIndex  sourceIndex ,
							        graphIndex  targetIndex = GRAPH_INVALID_INDEX_NO );
    CGraphSearchAStar& operator=( const CGraphSearchAStar& ); //警告封じのため定義。実際このクラスを代入でまわしたりはできない。

	/// <summary>
	/// ＳＰＴを定義するエッジのベクターを返す。コンストラクタにターゲットが与えられると、
	/// これは、ターゲットが見つかる前に、調べられたすべてのノードから構成されるＳＰＴとなる。
	/// そうでない場合は、このグラフ内のすべてのノードを含む
	/// </summary>
	EdgePtrVector &GetSPT( EdgePtrVector &vector ) const;

	/// <summary>
	/// ソースからターゲットへの最短経路を構成するノードインデックのリストを返す。
	/// これはＳＰＴをターゲットから後ろ向きに動作することで経路を計算する
	/// </summary>
	GraphIndexList &GetPathToTarget( GraphIndexList &dest ) const;

	/// <summary>
	/// ターゲットへのトータルコストを返す
	/// </summary>
	dectype GetCostToTarget() const;

private:

	/// <summary>
	/// 探索されるグラフへの参照
	/// </summary>
	const graph_type& m_Graph;

	/// <summary>
	/// ノードでインデックス付けされる。そのノードへの「実際」の累積コストを含む
	/// </summary>
	CostVector m_GCosts;

	///ノードでインデックス付けされる。m_GCosts[n]をnからターゲットノードまでの
	///ヒューリスティックなコストを加えたコストを含む
	///これは、iPQでインデックス付けたベクターである
	CostVector m_FCosts;

	/// <summary>
	/// 最短の経路ツリー( ＳＰＴ )
	/// このグラフにおいて、ＳＰＴ上のすべてのノードからソース硬度までの最も良い経路をカプセル化する向きを持つサブツリー
	/// </summary>
	EdgePtrVector m_ShortestPathTree;

	/// <summary>
	/// ＳＰＴに接続されたノードにつながる「親」エッジの（ノードによって）インデックス付け
	/// されたベクターである。ただし、これはまだＳＰＴに追加されていない。
	/// </summary>
	EdgePtrVector m_SearchFrontier;

	/// <summary>
	/// 出発インデックス
	/// </summary>
	graphIndex m_SourceIndex;

	/// <summary>
	/// 目標インデックス
	/// </summary>
	graphIndex m_TargetIndex;

	/// <summary>
	/// 検索処理
	/// </summary>
	inline void Search();

};

} } }

#include "GraphSearchAStar.inl"
