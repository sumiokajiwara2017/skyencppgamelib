#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// �W���C���g�O���t�X����p�֗��֐��Q
/// </summary>
class CJointUtil
{

public:

	/// <summary>
	/// �w��̃m�[�h�̎q���w��̌^���������Ďw��̌^�ŕԂ��e���v���[�g���\�b�h
	/// isFound��skyFalse�ŏ��������Ă��������B
	/// </summary>
	template< class SEARCH_TYPE , class RESULT_TYPE >
	static skyBool SeaachChildName( const CJointPtr &node , const hash::CHashStringPtr &name , RESULT_TYPE &result )
	{
		if ( node.IsEmpty() == skyTrue ) return skyFalse;

		if( SkyIsKindOf( SEARCH_TYPE , node.GetPtrNoConst() ) && node->Name == name )
        {
             result = node;
	         return skyTrue;
        }

		FOREACH( CJointPtrList , it , node->ChildList() )
		{
            skyBool isFound = SeaachChildName< SEARCH_TYPE , RESULT_TYPE >( *it , name , result );
	        if( isFound == skyTrue )
	        {
	             return skyTrue;
	        }
		}

		return skyFalse;
	}
};

} } }