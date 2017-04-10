namespace sky { namespace lib { namespace math {

namespace VectorUtil 
{

template< typename VECTOR3 >
inline void ComputeBestFitNormal( const VECTOR3 v[] , s32 num , VECTOR3& dest )
{
	//�[���ŏ�����
	dest.Zero();

	//���O�̒��_���Ō�̒��_�Ƃ��Ďn�߂�
	//����ɂ��A���[�v����if��������ł���B

	const VECTOR3 *p = &v[ num - 1 ];

	//���ׂĂ̒��_�ɂ��ČJ��Ԃ�

	for ( s32 i = 0 ; i < num ; i++ )
	{
		//���݂̒��_�ւ̃V���[�g�J�b�g�𓾂�

		const VECTOR3 *c = &v[ i ];

		//�Ӄx�N�g���̐Ȃ�K�؂ɉ�����

		dest.X += ( p->Z + c->Z ) * ( p->Y - c->Y );
		dest.Y += ( p->X + c->X ) * ( p->Z - c->Z );
		dest.Z += ( p->Y + c->Y ) * ( p->X - c->X );

		// ���̒��_�A�ǂ���

		p = c;
	
	}

	//���K������
	dest.Normalize();
}

}

} } }