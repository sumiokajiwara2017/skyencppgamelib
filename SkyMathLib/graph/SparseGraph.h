#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// グラフクラス（このクラスに任意のノード、任意のエッジを付けて使用する）
/// </summary>
template < class node_type , class edge_type >
class CSparseGraph : public base::SkyObject
{

public:

	/// <summary>
	/// 型の再定義
	/// </summary>
	typedef node_type NodeType;
	typedef edge_type EdgeType;

	typedef CVectorPrimitive< NodeType >       NodeTypeVector;
	typedef CList< u32 , EdgeType >   EdgeTypeList;
	typedef CVectorPrimitive< EdgeTypeList * > EdgeTypeListPtrVector;

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CSparseGraph(){};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CSparseGraph( skyBool isDigraph );

	/// <summary>
	/// 指定のインデックスのノードを返す
	/// </summary>
	inline const NodeType& GetNode( graphIndex index ) const;
	inline NodeType&       GetNode( graphIndex index );

	/// <summary>
	/// 指定のインデックスのエッジを返す
	/// </summary>
	inline const EdgeTypeList& GetEdgeList( graphIndex index ) const;
	inline const EdgeType&     GetEdge( graphIndex fromIndex , graphIndex toIndex ) const;
	inline EdgeType&           GetEdge( graphIndex fromIndex , graphIndex toIndex );

	/// <summary>
	/// 次のフリーノードインデックスを取得する
	/// </summary>
	inline graphIndex GetNextFreeNodeIndex() const;

	/// <summary>
	/// このグラフにノードを追加し、インデックスを返す
	/// </summary>
	inline graphIndex AddNode( const NodeType &node );

	/// <summary>
	/// ノードを無効化する
	/// </summary>
	inline void RemoveNode( graphIndex index );

	/// <summary>
	/// エッジを追加／削除する
	/// </summary>
	inline void AddEdge( EdgeType edge );
	inline void RemoveEdge( graphIndex fromIndex , graphIndex toIndex );
	inline void RemoveAllEdge();

	/// <summary>
	/// このグラフ内のアクティブ＋非アクティブなノードの個数を返す
	/// </summary>
	inline u32 GetAllNodeNum() const;

	/// <summary>
	/// このグラフ内のアクティブなノードの個数を返す
	/// </summary>
	inline u32 GetActiveNodeNum() const;

	/// <summary>
	/// このグラフ内のエッジの個数を返す
	/// </summary>
	inline u32 GetEdgeNum() const;

	/// <summary>
	/// このグラフが有向グラフか？
	/// </summary>
	inline skyBool IsDigraph() const;

	/// <summary>
	/// ノードが空か？
	/// </summary>
	inline skyBool IsEmpty() const;

	/// <summary>
	/// グラフをクリアする
	/// </summary>
	inline void Clear();

	/// <summary>
	/// 指定のインデックスのノードが有効か判定する
	/// </summary>
	inline skyBool IsNodePresent( graphIndex index ) const;

	/// <summary>
	/// 指定のインデックスのエッジが有効か判定する
	/// </summary>
	inline skyBool IsEdgePresent( graphIndex fromIndex , graphIndex toIndex ) const; 

	/// <summary>
	/// 無効なエッジをすべて消す
	/// </summary>
	inline void  CullInvalidEdges();

	/// <summary>
	/// エッジに新しいコストを設定する
	/// </summary>
	inline void SetEdgeCost( graphIndex fromIndex , graphIndex toIndex , dectype newCost );

	/// <summary>
	/// 指定の経路が存在しなければtrueを返す
	/// </summary>
	skyBool IsUniqueEdge( graphIndex fromIndex , graphIndex toIndex ) const;

protected:

	/// <summary>
	/// ノード群
	/// </summary>
	NodeTypeVector m_NodeVector;

	/// <summary>
	/// ノードに関連するエッジリスト配列
	/// （各ノードのインデックスは、そのノードに関連付けられたエッジのリストのキーとなる）
	/// </summary>
	EdgeTypeListPtrVector m_EdgyListVector;

	/// <summary>
	/// 指向性グラフフラグ
	/// </summary>
	skyBool m_IsDigraph;

	/// <summary>
	/// 追加予定のインデックス
	/// </summary>
	graphIndex m_NextnodeIndex;
};

} } }

#include "SparseGraph.inl"