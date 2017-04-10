namespace sky { namespace lib { namespace math {

CGraphEdge::~CGraphEdge()
{
}

CGraphEdge::CGraphEdge( graphIndex fromIndex , graphIndex toIndex , dectype cost ) : m_FromIndex( fromIndex ) , m_ToIndex( toIndex ) , m_Cost( cost )
{
}

CGraphEdge::CGraphEdge( graphIndex fromIndex , graphIndex toIndex ) : m_FromIndex( fromIndex ) , m_ToIndex( toIndex ) , m_Cost( 1.0f )
{
}

CGraphEdge::CGraphEdge() : m_FromIndex( GRAPH_INVALID_INDEX_NO ) , m_ToIndex( GRAPH_INVALID_INDEX_NO ) , m_Cost( 1.0f )
{
}

graphIndex CGraphEdge::GetFromIndex() const
{
	return m_FromIndex;
}

void CGraphEdge::SetFromIndex( graphIndex index )
{
	m_FromIndex = index;
}

graphIndex CGraphEdge::GetToIndex() const
{
	return m_ToIndex;
}

void CGraphEdge::SetToIndex( graphIndex index )
{
	m_ToIndex = index;
}

dectype CGraphEdge::GetCost() const
{
	return m_Cost;
}

void CGraphEdge::SetCost( dectype cost )
{
	m_Cost = cost;
}

} } }