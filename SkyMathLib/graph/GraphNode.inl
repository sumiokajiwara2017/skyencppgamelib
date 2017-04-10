namespace sky { namespace lib { namespace math {

inline CGraphNode::~CGraphNode()
{

}

inline CGraphNode::CGraphNode() : m_Index( GRAPH_INVALID_INDEX_NO )
{
}

inline CGraphNode::CGraphNode( graphIndex index ) : m_Index( index )
{
}

inline graphIndex CGraphNode::GetIndex() const
{
	return m_Index;
}
inline void CGraphNode::SetIndex( graphIndex index )
{
	m_Index = index;
}

} } }