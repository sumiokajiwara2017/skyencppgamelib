#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ユーグリッド空間ノード（※グラフの実体を作成する場合はコピーコンストラクタと代入演算子のオーバーライドを必ず行う）
/// </summary>
class CEuclidGraphNode : public CGraphNode
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CEuclidGraphNode(){}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CEuclidGraphNode() : m_EnableMoveRadius( 0 ){}
	inline CEuclidGraphNode( graphIndex index , const CBasicVector3 &pos ) : CGraphNode( index ) , m_Position( pos ) , m_EnableMoveRadius( 0 ){}

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	inline CEuclidGraphNode( const CEuclidGraphNode& src )
	{
		*this = src;
	};

	/// <summary>
	/// 代入演算子
	/// </summary>
	inline CEuclidGraphNode& operator =( const CEuclidGraphNode& src )
	{
		m_Position = src.m_Position;
		m_Index = src.m_Index;
		return *this;
	}

	/// <summary>
	/// 座標
	/// </summary>
	CBasicVector3 m_Position;

	/// <summary>
	/// 可動範囲円半径
	/// </summary>
	dectype       m_EnableMoveRadius;
};
typedef CStack< CEuclidGraphNode > CEuclidGraphNodeStack;

/// <summary>
/// ユーグリッド空間ノードエッジ（コスト＝距離）
/// </summary>
class CEuclidGraphEdge : public CGraphEdge
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CEuclidGraphEdge(){}

	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	inline CEuclidGraphEdge( const CEuclidGraphEdge& src )
	{
		*this = src;
	};

	/// <summary>
	/// 代入演算子
	/// </summary>
	inline CEuclidGraphEdge& operator =( const CEuclidGraphEdge& src )
	{
		m_FromIndex = src.m_FromIndex;
		m_ToIndex   = src.m_ToIndex;
		m_Cost      = src.m_Cost;
		return *this;
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CEuclidGraphEdge( graphIndex fromIndex , graphIndex toIndex , dectype cost ) : CGraphEdge( fromIndex , toIndex , cost ){};
	inline CEuclidGraphEdge( graphIndex fromIndex , graphIndex toIndex ) : CGraphEdge( fromIndex , toIndex ){};
	inline CEuclidGraphEdge(){};
};
typedef CStack< CEuclidGraphEdge > CEuclidGraphEdgeStack;

/// <summary>
/// ユーグリッド空間グラフクラス
/// </summary>
class CEuclidGraph : public CSparseGraph< CEuclidGraphNode , CEuclidGraphEdge >
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CEuclidGraph( skyBool isDigraph ) : CSparseGraph< CEuclidGraphNode , CEuclidGraphEdge >( isDigraph ){}

	/// <summary>
	/// 指定の点をもったノードが存在するか判定する
	/// </summary>
	inline skyBool IsPositionNodeOn( const CBasicVector3 &pos , graphIndex *index = skyNull )
	{
		skyBool result = skyFalse;
		for ( u32 i = 0 ; i < GetAllNodeNum() ; i++ )
		{
			if ( GetNode( i ).m_Position == pos )
			{
				result = skyTrue;
				if ( index != skyNull )
				{
					*index = GetNode( i ).GetIndex();
				}
				break;
			}
		}
		return result;
	};

	/// <summary>
	/// エッジのラインリストを返す
	/// </summary>
	inline void GetEdgeLineList( CBasicLine3List &dest )
	{
		FOREACH( EdgeTypeListPtrVector , it1 , m_EdgyListVector )
		{
			FOREACH( EdgeTypeList , it2 , *( *it1 ) )
			{
				CBasicLine3 line3( GetNode( ( *it2 ).GetFromIndex() ).m_Position , GetNode( ( *it2 ).GetToIndex() ).m_Position );
				dest.AddTail( line3 );
			}
		}
	}

	/// <summary>
	/// 指定の座標から指定の距離の範囲にあるノード最初に見つかったもののインデックスを返す
	/// もし見つからない場合はその座標に最も近い座標を持つノードのインデックスを返す
	/// </summary>
	inline graphIndex GetNearNodeIndex( const CBasicVector3 &pos , dectype distance )
	{
		graphIndex result = GRAPH_INVALID_INDEX_NO;
		graphIndex nearNodeIndex = GRAPH_INVALID_INDEX_NO;
		dectype nearDistance = FLT_MAX;

		skyBool isFound = skyFalse;
		dectype checkDistance = 0;
		FOREACH( NodeTypeVector , it , m_NodeVector )
		{
			//距離を測る
			checkDistance = Distance< dectype >( ( *it ).m_Position , pos );

			//条件に合ったノードを探す
			if ( checkDistance <= distance )
			{
				result = ( *it ).GetIndex();
				isFound = skyTrue;
				break;
			}
			//最も近いノードを探す
			else if ( checkDistance < nearDistance )
			{
				nearDistance = checkDistance;
				nearNodeIndex = ( *it ).GetIndex();
			}
		}

		if ( isFound == skyFalse )
		{
			result = nearNodeIndex;
		}

		return result;
	}
};

/*

ヒューリスティックとは
Ａ＊プログラムで使用する比較コストを計算するための実装クラス
必ず下記の関数を実装する

static dectype Calculate( const graph_type &graph , graphIndex node1 , graphIndex node2 );

この実装はノードの型によって変わる。ユーグリッド空間ヒューリスティックはコスト＝距離なので
ノード間の距離を計算している。

*/

/// <summary>
/// ユーグリッド空間ヒューリスティック
/// </summary>
class CEuclidHeuristic
{

public:

	/// <summary>
	/// ノード１からノード２までの直線距離（コスト）を計算する
	/// </summary>
	template < class graph_type >
	static dectype Calculate( const graph_type &graph , graphIndex node1 , graphIndex node2 )
	{
		return Distance( graph.GetNode( node1 ).m_Position , graph.GetNode( node2 ).m_Position );
	}
};

//---便利関数---

typedef skyBool ( * CEuclidGraph_CollisionFunc )( const CBasicLine3 &line3 );

//３次元空間に指定のフェンスの範囲内で一定のルールでノードを配置し、エッジで結び、方向無しのグラフを形成する
//再帰はオーバーフローする可能性があるのでスタックでやる。すべての再帰処理はスタック処理に置き換えられる
inline void CEuclidGraph_Create8DirectionsGraph( CEuclidGraph &dest , const CBasicVector3 &startPos , const CBasicVector3Vector &fence , dectype edgeLength , skyBool isHitCheck2D = skyTrue , CEuclidGraph_CollisionFunc colFunc = skyNull , u32 stackSize = 512 )
{
	//グラフノード番号
	graphIndex nextId = GRAPH_INVALID_INDEX_NO;

	//準備として８方向のベクトルを作成する
	CBasicVector3 edgeLines[ 8 ];
	edgeLines[ 0 ] = CBasicVector3( 0.0f        , 0.0f , edgeLength );
	edgeLines[ 1 ] = CBasicVector3( edgeLength  , 0.0f , edgeLength );
	edgeLines[ 2 ] = CBasicVector3( edgeLength  , 0.0f , 0.0f );
	edgeLines[ 3 ] = CBasicVector3( edgeLength  , 0.0f , -edgeLength );
	edgeLines[ 4 ] = CBasicVector3( 0.0f        , 0.0f , -edgeLength );
	edgeLines[ 5 ] = CBasicVector3( -edgeLength , 0.0f , -edgeLength );
	edgeLines[ 6 ] = CBasicVector3( -edgeLength , 0.0f , 0.0f );
	edgeLines[ 7 ] = CBasicVector3( -edgeLength , 0.0f , edgeLength );

	//スタックを作る
	CEuclidGraphNodeStack nodeStack( stackSize );
	CEuclidGraphEdgeStack edgeStack( stackSize );

	//スタックに開始地点を投入
	nodeStack.Push( CEuclidGraphNode( GRAPH_INVALID_INDEX_NO , startPos ) );

	CBasicVector3 nextPos;
	while( nodeStack.GetNum() > 0 )
	{
		//対象の座標をスタックから取得
		CEuclidGraphNode nowNode = nodeStack.GetTop();
		nowNode.SetIndex( dest.GetNextFreeNodeIndex() ); //ＩＤ発行
		nodeStack.Pop();

		//位置にノードを作る（必ずここには作成できる座標しかこない）
		dest.AddNode( nowNode );

		//エッジがあればエッジを作成する
		if ( edgeStack.GetNum() > 0 )
		{
			edgeStack.GetTop().SetToIndex( nowNode.GetIndex() ); //エッジを完成させる
			edgeStack.GetTop().SetCost( Distance< dectype >( dest.GetNode( edgeStack.GetTop().GetFromIndex() ).m_Position , nowNode.m_Position ) );
			dest.AddEdge( edgeStack.GetTop() );
			edgeStack.Pop();
		}

		//８方向に向けてエッジを飛ばす
		for ( u32 i = 0 ; i < 8 ; i++ )
		{
			nextPos = edgeLines[ i ] + nowNode.m_Position;

			//スタックにすでに積んであったら何もしない
			skyBool isStackOn = skyFalse;
			for( u32 i = 0 ; i < nodeStack.GetNum() ; i++ )
			{
				if ( nextPos == nodeStack[ i ].m_Position )
				{
					isStackOn = skyTrue;
					nextId = nodeStack[ i ].GetIndex();
					break;
				}
			}

			if ( isStackOn == skyFalse && dest.IsPositionNodeOn( nextPos , &nextId ) == skyFalse )
			{
				//線分を作成する
				CBasicLine3 edge( nowNode.m_Position , nextPos );

				skyBool hitCheckFlg = ( isHitCheck2D ) ? 
					            GeometryCollisionUtil::Line3CrossLine3Fence( edge , fence , skyTrue ) : //２Ｄ（ＸＺ座標）で当たりをとる
				                GeometryCollisionUtil::Line3CrossLine3Fence( edge , fence );            //３Ｄで当たりをとる

				if ( hitCheckFlg == skyFalse && colFunc != skyNull )
				{//追加のヒットチェック
					hitCheckFlg = colFunc( edge );
				}

				//線分とフェンスの衝突判定を行う
				if ( hitCheckFlg == skyFalse )
				{//この方向には進める。

					//座標をスタックに追加
					nodeStack.Push( CEuclidGraphNode( GRAPH_INVALID_INDEX_NO , nextPos )  );

					//エッジを張る（この段階では未完成）
					edgeStack.Push( CEuclidGraphEdge( nowNode.GetIndex() , GRAPH_INVALID_INDEX_NO ) );
				}
			}
			else
			{
				if ( nextId != GRAPH_INVALID_INDEX_NO )
				{
					//線分を作成する
					CBasicLine3 edge( nowNode.m_Position , dest.GetNode( nextId ).m_Position );

					skyBool hitCheckFlg = skyFalse;

					if ( colFunc != skyNull )
					{//追加のヒットチェック
						hitCheckFlg = colFunc( edge );
					}

					if ( hitCheckFlg == skyFalse )
					{
						//エッジを張る
						dest.AddEdge( CEuclidGraphEdge( nowNode.GetIndex() , nextId , Distance< dectype >( nowNode.m_Position , dest.GetNode( nextId ).m_Position ) ) );
					}
				}
			}
		}
	}

	//８方向にエッジがあるノードはエッジの長さ＝ノードを中心とした絶対可動範囲とする
	for ( graphIndex i = 0 ; i < ( graphIndex )dest.GetAllNodeNum() ; i++ )
	{
		//有効なノードであり１６本の（８方向相互）にエッジがある場合
		if ( dest.IsNodePresent( i ) == skyTrue && dest.GetEdgeList( i ).GetNum() == 8 )
		{
			dest.GetNode( i ).m_EnableMoveRadius = edgeLength;
		}
	}
}

} } }