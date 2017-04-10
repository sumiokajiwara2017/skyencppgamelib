#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ダイクストラの最短経路アルゴリズムクラス
/// </summary>
template< class graph_type >
class CGraphSearchDijkstra
{

public:

	/// <summary>
	/// エッジとノードの型を再定義する
	/// </summary>
	typedef typename graph_type::NodeType Node;
	typedef typename graph_type::EdgeType Edge;
	typedef CVectorPrimitive< const Edge * >       EdgePtrVector;

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CGraphSearchDijkstra();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CGraphSearchDijkstra( const graph_type& graph ,
		                         graphIndex        sourceIndex ,
							     graphIndex        targetIndex = GRAPH_INVALID_INDEX_NO );
    CGraphSearchDijkstra& operator=( const CGraphSearchDijkstra& ); //警告封じのため定義。実際このクラスを代入でまわしたりはできない。

	/// <summary>
	/// ＳＰＴを定義するエッジのベクターを返す。コンストラクタにターゲットが与えられると、
	/// これは、ターゲットが見つかる前に、調べられたすべてのノードから構成されるＳＰＴとなる。
	/// そうでない場合は、このグラフ内のすべてのノードを含む
	/// </summary>
	EdgePtrVector &GetSPT( EdgePtrVector &vector ) const;

	/// <summary>
	/// ソースからターゲットへの最短経路を構成するノードインデックスのベクターを返す。
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
	/// 最短の経路ツリー( ＳＰＴ )
	/// このグラフにおいて、ＳＰＴ上のすべてのノードからソース硬度までの最も良い経路をカプセル化する向きを持つサブツリー
	/// </summary>
	EdgePtrVector m_ShortestPathTree;

	/// <summary>
	/// ノードインデックスでインデックス付けされた、与えられたノードに対して
	/// これまで見つかった最も良い経路のトータルコストを持つ
    /// 例えば、m_CostToThisNode[ 5 ]は、探索でこれまでに見つかったノード５までの
	/// 最も良い経路から恒星されているすべてのエッジのトータルコストを持つ（もちろんノード５が存在し、これまでに訪れ他ことがある場合）
	/// </summary>
	CostVector    m_CostToThisNode;

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

#include "GraphSearchDJ.inl"