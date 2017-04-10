#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ���[�O���b�h��ԃm�[�h�i���O���t�̎��̂��쐬����ꍇ�̓R�s�[�R���X�g���N�^�Ƒ�����Z�q�̃I�[�o�[���C�h��K���s���j
/// </summary>
class CEuclidGraphNode : public CGraphNode
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CEuclidGraphNode(){}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CEuclidGraphNode() : m_EnableMoveRadius( 0 ){}
	inline CEuclidGraphNode( graphIndex index , const CBasicVector3 &pos ) : CGraphNode( index ) , m_Position( pos ) , m_EnableMoveRadius( 0 ){}

	/// <summary>
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	inline CEuclidGraphNode( const CEuclidGraphNode& src )
	{
		*this = src;
	};

	/// <summary>
	/// ������Z�q
	/// </summary>
	inline CEuclidGraphNode& operator =( const CEuclidGraphNode& src )
	{
		m_Position = src.m_Position;
		m_Index = src.m_Index;
		return *this;
	}

	/// <summary>
	/// ���W
	/// </summary>
	CBasicVector3 m_Position;

	/// <summary>
	/// ���͈͉~���a
	/// </summary>
	dectype       m_EnableMoveRadius;
};
typedef CStack< CEuclidGraphNode > CEuclidGraphNodeStack;

/// <summary>
/// ���[�O���b�h��ԃm�[�h�G�b�W�i�R�X�g�������j
/// </summary>
class CEuclidGraphEdge : public CGraphEdge
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CEuclidGraphEdge(){}

	/// <summary>
	/// �R�s�[�R���X�g���N�^
	/// </summary>
	inline CEuclidGraphEdge( const CEuclidGraphEdge& src )
	{
		*this = src;
	};

	/// <summary>
	/// ������Z�q
	/// </summary>
	inline CEuclidGraphEdge& operator =( const CEuclidGraphEdge& src )
	{
		m_FromIndex = src.m_FromIndex;
		m_ToIndex   = src.m_ToIndex;
		m_Cost      = src.m_Cost;
		return *this;
	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CEuclidGraphEdge( graphIndex fromIndex , graphIndex toIndex , dectype cost ) : CGraphEdge( fromIndex , toIndex , cost ){};
	inline CEuclidGraphEdge( graphIndex fromIndex , graphIndex toIndex ) : CGraphEdge( fromIndex , toIndex ){};
	inline CEuclidGraphEdge(){};
};
typedef CStack< CEuclidGraphEdge > CEuclidGraphEdgeStack;

/// <summary>
/// ���[�O���b�h��ԃO���t�N���X
/// </summary>
class CEuclidGraph : public CSparseGraph< CEuclidGraphNode , CEuclidGraphEdge >
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CEuclidGraph( skyBool isDigraph ) : CSparseGraph< CEuclidGraphNode , CEuclidGraphEdge >( isDigraph ){}

	/// <summary>
	/// �w��̓_���������m�[�h�����݂��邩���肷��
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
	/// �G�b�W�̃��C�����X�g��Ԃ�
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
	/// �w��̍��W����w��̋����͈̔͂ɂ���m�[�h�ŏ��Ɍ����������̂̃C���f�b�N�X��Ԃ�
	/// ����������Ȃ��ꍇ�͂��̍��W�ɍł��߂����W�����m�[�h�̃C���f�b�N�X��Ԃ�
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
			//�����𑪂�
			checkDistance = Distance< dectype >( ( *it ).m_Position , pos );

			//�����ɍ������m�[�h��T��
			if ( checkDistance <= distance )
			{
				result = ( *it ).GetIndex();
				isFound = skyTrue;
				break;
			}
			//�ł��߂��m�[�h��T��
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

�q���[���X�e�B�b�N�Ƃ�
�`���v���O�����Ŏg�p�����r�R�X�g���v�Z���邽�߂̎����N���X
�K�����L�̊֐�����������

static dectype Calculate( const graph_type &graph , graphIndex node1 , graphIndex node2 );

���̎����̓m�[�h�̌^�ɂ���ĕς��B���[�O���b�h��ԃq���[���X�e�B�b�N�̓R�X�g�������Ȃ̂�
�m�[�h�Ԃ̋������v�Z���Ă���B

*/

/// <summary>
/// ���[�O���b�h��ԃq���[���X�e�B�b�N
/// </summary>
class CEuclidHeuristic
{

public:

	/// <summary>
	/// �m�[�h�P����m�[�h�Q�܂ł̒��������i�R�X�g�j���v�Z����
	/// </summary>
	template < class graph_type >
	static dectype Calculate( const graph_type &graph , graphIndex node1 , graphIndex node2 )
	{
		return Distance( graph.GetNode( node1 ).m_Position , graph.GetNode( node2 ).m_Position );
	}
};

//---�֗��֐�---

typedef skyBool ( * CEuclidGraph_CollisionFunc )( const CBasicLine3 &line3 );

//�R������ԂɎw��̃t�F���X�͈͓̔��ň��̃��[���Ńm�[�h��z�u���A�G�b�W�Ō��сA���������̃O���t���`������
//�ċA�̓I�[�o�[�t���[����\��������̂ŃX�^�b�N�ł��B���ׂĂ̍ċA�����̓X�^�b�N�����ɒu����������
inline void CEuclidGraph_Create8DirectionsGraph( CEuclidGraph &dest , const CBasicVector3 &startPos , const CBasicVector3Vector &fence , dectype edgeLength , skyBool isHitCheck2D = skyTrue , CEuclidGraph_CollisionFunc colFunc = skyNull , u32 stackSize = 512 )
{
	//�O���t�m�[�h�ԍ�
	graphIndex nextId = GRAPH_INVALID_INDEX_NO;

	//�����Ƃ��ĂW�����̃x�N�g�����쐬����
	CBasicVector3 edgeLines[ 8 ];
	edgeLines[ 0 ] = CBasicVector3( 0.0f        , 0.0f , edgeLength );
	edgeLines[ 1 ] = CBasicVector3( edgeLength  , 0.0f , edgeLength );
	edgeLines[ 2 ] = CBasicVector3( edgeLength  , 0.0f , 0.0f );
	edgeLines[ 3 ] = CBasicVector3( edgeLength  , 0.0f , -edgeLength );
	edgeLines[ 4 ] = CBasicVector3( 0.0f        , 0.0f , -edgeLength );
	edgeLines[ 5 ] = CBasicVector3( -edgeLength , 0.0f , -edgeLength );
	edgeLines[ 6 ] = CBasicVector3( -edgeLength , 0.0f , 0.0f );
	edgeLines[ 7 ] = CBasicVector3( -edgeLength , 0.0f , edgeLength );

	//�X�^�b�N�����
	CEuclidGraphNodeStack nodeStack( stackSize );
	CEuclidGraphEdgeStack edgeStack( stackSize );

	//�X�^�b�N�ɊJ�n�n�_�𓊓�
	nodeStack.Push( CEuclidGraphNode( GRAPH_INVALID_INDEX_NO , startPos ) );

	CBasicVector3 nextPos;
	while( nodeStack.GetNum() > 0 )
	{
		//�Ώۂ̍��W���X�^�b�N����擾
		CEuclidGraphNode nowNode = nodeStack.GetTop();
		nowNode.SetIndex( dest.GetNextFreeNodeIndex() ); //�h�c���s
		nodeStack.Pop();

		//�ʒu�Ƀm�[�h�����i�K�������ɂ͍쐬�ł�����W�������Ȃ��j
		dest.AddNode( nowNode );

		//�G�b�W������΃G�b�W���쐬����
		if ( edgeStack.GetNum() > 0 )
		{
			edgeStack.GetTop().SetToIndex( nowNode.GetIndex() ); //�G�b�W������������
			edgeStack.GetTop().SetCost( Distance< dectype >( dest.GetNode( edgeStack.GetTop().GetFromIndex() ).m_Position , nowNode.m_Position ) );
			dest.AddEdge( edgeStack.GetTop() );
			edgeStack.Pop();
		}

		//�W�����Ɍ����ăG�b�W���΂�
		for ( u32 i = 0 ; i < 8 ; i++ )
		{
			nextPos = edgeLines[ i ] + nowNode.m_Position;

			//�X�^�b�N�ɂ��łɐς�ł������牽�����Ȃ�
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
				//�������쐬����
				CBasicLine3 edge( nowNode.m_Position , nextPos );

				skyBool hitCheckFlg = ( isHitCheck2D ) ? 
					            GeometryCollisionUtil::Line3CrossLine3Fence( edge , fence , skyTrue ) : //�Q�c�i�w�y���W�j�œ�������Ƃ�
				                GeometryCollisionUtil::Line3CrossLine3Fence( edge , fence );            //�R�c�œ�������Ƃ�

				if ( hitCheckFlg == skyFalse && colFunc != skyNull )
				{//�ǉ��̃q�b�g�`�F�b�N
					hitCheckFlg = colFunc( edge );
				}

				//�����ƃt�F���X�̏Փ˔�����s��
				if ( hitCheckFlg == skyFalse )
				{//���̕����ɂ͐i�߂�B

					//���W���X�^�b�N�ɒǉ�
					nodeStack.Push( CEuclidGraphNode( GRAPH_INVALID_INDEX_NO , nextPos )  );

					//�G�b�W�𒣂�i���̒i�K�ł͖������j
					edgeStack.Push( CEuclidGraphEdge( nowNode.GetIndex() , GRAPH_INVALID_INDEX_NO ) );
				}
			}
			else
			{
				if ( nextId != GRAPH_INVALID_INDEX_NO )
				{
					//�������쐬����
					CBasicLine3 edge( nowNode.m_Position , dest.GetNode( nextId ).m_Position );

					skyBool hitCheckFlg = skyFalse;

					if ( colFunc != skyNull )
					{//�ǉ��̃q�b�g�`�F�b�N
						hitCheckFlg = colFunc( edge );
					}

					if ( hitCheckFlg == skyFalse )
					{
						//�G�b�W�𒣂�
						dest.AddEdge( CEuclidGraphEdge( nowNode.GetIndex() , nextId , Distance< dectype >( nowNode.m_Position , dest.GetNode( nextId ).m_Position ) ) );
					}
				}
			}
		}
	}

	//�W�����ɃG�b�W������m�[�h�̓G�b�W�̒������m�[�h�𒆐S�Ƃ�����Ή��͈͂Ƃ���
	for ( graphIndex i = 0 ; i < ( graphIndex )dest.GetAllNodeNum() ; i++ )
	{
		//�L���ȃm�[�h�ł���P�U�{�́i�W�������݁j�ɃG�b�W������ꍇ
		if ( dest.IsNodePresent( i ) == skyTrue && dest.GetEdgeList( i ).GetNum() == 8 )
		{
			dest.GetNode( i ).m_EnableMoveRadius = edgeLength;
		}
	}
}

} } }