#include "StdAfx.h"
#include "../GoogleTestInclude.h"
#include "../SkyLibInclude.h"

#ifdef TEST_GRAPH

using namespace sky::lib;

#include "SkyMathLib/Graph.h"

class CTestGraphNode : public math::CGraphNode
{

public:

   CTestGraphNode(){}
   CTestGraphNode& operator=( const math::CGraphNode& ) { return *this; }	
};

class CTestGraphEdge : public math::CGraphEdge
{

public:

   CTestGraphEdge(){}
   CTestGraphEdge& operator=( const math::CGraphEdge& ) { return *this; }	
};

typedef math::CSparseGraph< CTestGraphNode , CTestGraphEdge > CTestGraph;

TEST( graph , 1 ) 
{
	SKY_MATH_LIB_INIT();

	CTestGraph graph( skyTrue );
	graph.IsNodePresent( 0 );
	graph.IsEdgePresent( 0 , 1 );
	graph.GetAllNodeNum();
	graph.GetActiveNodeNum();
	graph.IsDigraph();
	graph.IsEmpty();
	graph.Clear();
	graph.GetEdgeNum();
	graph.AddNode( CTestGraphNode() );
	graph.RemoveNode( 0 );
	graph.SetEdgeCost( 0 , 1 , 0.5 );
	graph.IsUniqueEdge( 0 , 1 );
	graph.AddEdge( CTestGraphEdge() );
	graph.GetEdge( 0 , 1 );
	graph.GetNode( 0 );
	const CTestGraph::EdgeTypeList &edgeList = graph.GetEdgeList( 0 );

	math::CGraphSearchDFS< CTestGraph > testGraphSearchDFS( graph , 0 );
	testGraphSearchDFS.Found();
	math::GraphIndexList pathList;
	testGraphSearchDFS.GetPathToTarget( pathList );

	math::CGraphSearchBFS< CTestGraph > testGraphSearchBFS( graph , 0 );
	testGraphSearchBFS.Found();
	testGraphSearchBFS.GetPathToTarget( pathList );

	math::CGraphSearchDijkstra< CTestGraph > testGraphSearchDJ( graph , 0 );
	math::CGraphSearchDijkstra< CTestGraph >::EdgePtrVector vector;
	testGraphSearchDJ.GetSPT( vector );
	testGraphSearchDJ.GetPathToTarget( pathList );
	testGraphSearchDJ.GetCostToTarget();

	math::CEuclidGraph euGraph( skyTrue );
	math::CGraphSearchAStar< math::CEuclidGraph , math::CEuclidHeuristic > astar( euGraph , 0 );
	testGraphSearchDJ.GetPathToTarget( pathList );

	SKY_MATH_LIB_TERM();
}

#endif