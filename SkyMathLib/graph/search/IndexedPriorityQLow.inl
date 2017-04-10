namespace sky { namespace lib { namespace math {

template< class KeyType >
CIndexedPriorityQLow< KeyType >::CIndexedPriorityQLow( CVectorPrimitive< KeyType >& keys , s32 MaxSize ) : 
	                                        m_vecKeys( keys ) ,
                                            m_iMaxSize( MaxSize ) ,
                                            m_iSize( 0 )
{
	m_Heap.Assign( MaxSize + 1 , 0 );
	m_invHeap.Assign( MaxSize + 1 , 0 );
}

template< class KeyType >
CIndexedPriorityQLow< KeyType >& CIndexedPriorityQLow< KeyType >::operator=( const CIndexedPriorityQLow& )
{ 
	SKY_PANIC_MSG( "this proc is invalid." );
	return *this;
}	

template< class KeyType >
skyBool CIndexedPriorityQLow< KeyType >::IsEmpty() const
{ 
	return ( m_iSize == 0 )? skyTrue : skyFalse;
}

template< class KeyType >
void CIndexedPriorityQLow< KeyType >::Insert( const s32 idx )
{
	SKY_ASSERT( m_iSize + 1 <= m_iMaxSize );
    ++m_iSize;
    m_Heap[ m_iSize ] = idx;
    m_invHeap[ idx ]  = m_iSize;
    ReorderUpwards( m_iSize );
}

template< class KeyType >
s32 CIndexedPriorityQLow< KeyType >::Pop()
{
    Swap( 1 , m_iSize );
    ReorderDownwards( 1 , m_iSize - 1 );
    return m_Heap[ m_iSize-- ];
}

template< class KeyType >
void CIndexedPriorityQLow< KeyType >::ChangePriority( const s32 idx )
{
    ReorderUpwards( m_invHeap[ idx ] );
}

template< class KeyType >
void CIndexedPriorityQLow< KeyType >::Swap( s32 a, s32 b )
{
    int temp = m_Heap[ a ]; m_Heap[ a ] = m_Heap[ b ]; m_Heap[ b ] = temp;

    //change the handles too
    m_invHeap[m_Heap[a]] = a; m_invHeap[m_Heap[b]] = b;
}

template< class KeyType >
void CIndexedPriorityQLow< KeyType >::ReorderUpwards( s32 nd )
{
    //move up the heap swapping the elements until the heap is ordered
    while ( ( nd > 1 ) && ( m_vecKeys[ m_Heap[ nd / 2 ] ] > m_vecKeys[ m_Heap[ nd ] ] ) )
    {      
        Swap( nd / 2 , nd );

        nd /= 2;
    }
}

template< class KeyType >
void CIndexedPriorityQLow< KeyType >::ReorderDownwards( s32 nd , s32 HeapSize )
{
    //move down the heap from node nd swapping the elements until
    //the heap is reordered
    while ( 2 * nd <= HeapSize )
    {
        int child = 2 * nd;

        //set child to smaller of nd's two children
        if ( ( child < HeapSize ) && ( m_vecKeys[ m_Heap[ child ] ] > m_vecKeys[ m_Heap[ child + 1 ] ] ) )
        {
            ++child;
        }

        //if this nd is larger than its child, swap
        if ( m_vecKeys[ m_Heap[ nd ] ] > m_vecKeys[ m_Heap[ child ] ] )
        {
            Swap( child , nd );

            //move the current node down the tree
            nd = child;
        }
        else
        {
            break;
        }
    }
}

} } }
