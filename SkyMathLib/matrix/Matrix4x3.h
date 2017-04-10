#pragma once

namespace sky { namespace lib { namespace math {

class CEulerAngles;
class CRotationMatrix;
class CQuaternion;

class CMatrix4x3;
typedef CList< u32 , CMatrix4x3 >	    CMatrix4x3List;	                   //���X�g��`
typedef CVectorPrimitive< CMatrix4x3 >	CMatrix4x3Array;	               //�z���`
typedef CHash< CMatrix4x3 >	            CMatrix4x3Hash;	                   //���O�z���`

/// <summary>
/// �R�c�����p�S�w�R�s��N���X
/// 
/// �s��̍\��
/// 
/// ���̃N���X�̖ړI�́A���̌��ʂ�������������܂ŁA�s��Ƀv���X�L����}�C�i�X�L����������
/// �]�u�����肵�Ă��炢�炷�邱�ƂȂ��A���W�ϊ����s����悤�ɂ��邱�Ƃł���B
/// �����������\���̎d�l�͏d�v�ł���B
/// ����́A���̃t�@�C�����̎����𐳂������邽�߂łȂ��A�s��ϐ��ւ̒��ڃA�N�Z�X�����X�K�v�ɂȂ�����A
/// �œK���̂��߂ɂȂ邩��ł���B���̂��߁A�����s��̖񑩎��������ɏ����Ă���
/// 
///               | m11 m12 m13 |
///     [ x y z ] | m21 m22 m23 | = [ x' y' z' ]
///               | m31 m32 m33 |
///               | tx  ty  tz  | 
///               
/// ���`�㐔�̃��[���Ɍ����ɏ]���ƁA���̏�Z�͎��ۂɂ͖���`�ƂȂ�
/// ������������ɂ́A���̓x�N�g���Əo�̓x�N�g�����A�S�Ԗڂ̍��W�P�������̂Ƃ��čl����
/// �܂��A���`�㐔�̃��[���ɂ��A�Z�p�I�ɂS�~�R�s��̋t�s������߂邱�Ƃ͂ł��Ȃ��̂ŁA
/// ��ԉE�[�̗��[ 0 0 0 1 ]��������̂Ɖ��肷��B
/// ������ȉ��Ɏ����B
/// 
///                 | m11 m12 m13 0 |
///     [ x y z 1 ] | m21 m22 m23 0 | = [ x' y' z' 1 ]
///                 | m31 m32 m33 0 |
///                 | tx  ty  tz  1 |
///
/// </summary>
class CMatrix4x3
{

public:

    /// <summary>
    /// �v���p�e�B
    /// </summary>
	union 
	{
		struct 
		{
			dectype	M11, M12, M13, M14;
			dectype	M21, M22, M23, M24;
			dectype	M31, M32, M33, M34;
			dectype	Tx ,  Ty,  Tz,  Tw;
		};
		dectype m[ 4 ][ 4 ];
	};

	/// <summary>
	/// =���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	inline CMatrix4x3& operator =( const CMatrix4x3& v );

	/// <summary>
	///+���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	inline CMatrix4x3  operator + ( const CMatrix4x3& src ) const;

	/// <summary>
	///-���Z�q�I�[�o�[���C�h�i�v�f�̑���j
	/// </summary>
	inline CMatrix4x3  operator - ( const CMatrix4x3& src ) const;

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
	inline CMatrix4x3();
	inline CMatrix4x3( const dectype mat[ 16 ] );
	inline CMatrix4x3( dectype m11 , dectype m12 , dectype m13 , dectype m14 ,
                dectype m21 , dectype m22 , dectype m23 , dectype m24 , 
                dectype m31 , dectype m32 , dectype m33 , dectype m34 , 
                dectype m41 , dectype m42 , dectype m43 , dectype m44 );

    /// <summary>
    /// �P���s���ݒ肷��
    /// </summary>
	inline CMatrix4x3& Identity();

    /// <summary>
    /// �s��̕��s�ړ������𒼐ڃA�N�Z�X����
    /// �Ō�̍s��[0,0,0]�ɐݒ肵���`�ϊ������i�R�~�R�̕����j�����̂܂܎c�����ƂŁA
    /// ���̍s��̕��s�ړ���������菜���܂��B
    /// </summary>
	inline CMatrix4x3& ZeroTranslation();

    /// <summary>
    /// �s��̕��s�ړ������𒼐ڃA�N�Z�X����
    /// �s��̕��s�ړ��������w�肳�ꂽ�l�ɐݒ肵�A�R�~�R�̕����͍P���s��ɂ��܂��B�i�܂菃���ȕ��s�ړ��s��ɂȂ�j
    /// </summary>
	inline CMatrix4x3& SetupTranslation( const CBasicVector3 &d );

    /// <summary>
    /// �s��̕��s�ړ��������㏑������
    /// �s��̕��s�ړ��������w�肳�ꂽ�l�ɐݒ肵�A�R�~�R�̕����͕ύX���܂���B
    /// </summary>
	inline CMatrix4x3& SetTranslation( const CBasicVector3 &d );

    /// <summary>
    /// �s��̕��s�ړ������ɉ��Z����
    /// �s��̕��s�ړ��������w�肳�ꂽ�l�ɐݒ肵�A�R�~�R�̕����͕ύX���܂���B
    /// </summary>
	inline CMatrix4x3& AddTranslation( const CBasicVector3 &d );

    /// <summary>
    /// �R���̈ړ������擾����
    /// </summary>
	inline dectype GetTranslation( eAxis axis ) const;

    /// <summary>
    /// �e�̊���W���̃��[�J���̊���W�n�̈ʒu�ƕ������w�肵�A
    /// ���[�J�����e�ւ̍��W�ϊ������s����s����Z�b�g�A�b�v����
    /// 
    /// ����̍ł��悭����g�����́A�I�u�W�F�N�g�s�񁨃��[���h�s��̍쐬�ł���
    /// ��Ƃ��āA���̏ꍇ�̍��W�ϊ��͊ȒP�ł���
    /// �ŏ��ɁA�I�u�W�F�N�g��Ԃ��犵����Ԃ։�]���A���̌�A���[���h��Ԃ֕��s�ړ�����
    /// 
    /// �����́A�I�C���[�p��RotationMatrix�̂����ꂩ�Ŏw��ł���B
    /// 
    /// </summary>
	CMatrix4x3& SetupLocalToParent( const CBasicVector3 &pos , const CEulerAngles &orient );
	CMatrix4x3& SetupLocalToParent( const CBasicVector3 &pos , const CRotationMatrix &orient );

    /// <summary>
    /// �e�̊���W�n���̃��[�J���̊���W�n�̈ʒu�ƕ������w�肵�A
    /// �e�����[�J���̍��W�ϊ������s����s����Z�b�g�A�b�v����
    /// 
    /// ����̍ł��悭����g�����́A���[���h�s�񁨃I�u�W�F�N�g�s��̍쐬�ł���B
    /// ���̍��W�ϊ������s����ɂ́A�ʏ�A�ŏ��Ƀ��[���h���犵����Ԃ֍��W�ϊ����A
    /// ���̌�A������Ԃ���I�u�W�F�N�g��Ԃ։�]����
    /// �������Ȃ���A�����̂S�~�R�s��́A��ɍŌ�ɕ��s�ړ����s��
    /// ���̂��߁A�Q�̍s��s�Ƃq�̍쐬���l���A�l���s�q�Ƃ����`�ŘA������
    /// 
    /// �����́A�I�C���[�p��RotationMatrix�̂����ꂩ�Ŏw��ł���
    /// </summary>
	CMatrix4x3& SetupParentToLocal( const CBasicVector3 &pos , const CEulerAngles &orient );
	CMatrix4x3& SetupParentToLocal( const CBasicVector3 &pos , const CRotationMatrix &orient );

    /// <summary>
    /// ��{���̎���̉�]�����s����s����Z�b�g�A�b�v����
    /// 
    /// ��]�̎��́A�ȉ��̂悤�ɂP���x�[�X�ɂ����C���f�b�N�X��p���Ďw�肳���
    /// 
    /// eAxis_x���w���Ɋւ����]
    /// eAxis_y���x���Ɋւ����]
    /// eAxis_z���y���Ɋւ����]
    /// 
    /// theta�͉�]�̗ʁi���W�A���j�A����̃��[���Ő��̉�]���`����
    /// 
    /// ���s�ړ������̓��Z�b�g�����
    /// 
    /// </summary>
	inline CMatrix4x3& SetupRotate( eAxis axis , dectype theta );

    /// <summary>
    /// �C�ӂ̎��̎���̉�]�����s����s����Z�b�g�A�b�v����
    /// 
	/// ��]�̊|�����킹������ނ�����B
    /// 
    /// ���s�ړ������̓��Z�b�g�����
    /// 
    /// </summary>
	inline CMatrix4x3& SetupRotateXYZ( const CBasicVector3 &vec );
	inline CMatrix4x3& SetupRotateXZY( const CBasicVector3 &vec );
	inline CMatrix4x3& SetupRotateYXZ( const CBasicVector3 &vec );
	inline CMatrix4x3& SetupRotateYZX( const CBasicVector3 &vec );
	inline CMatrix4x3& SetupRotateZXY( const CBasicVector3 &vec );
	inline CMatrix4x3& SetupRotateZYX( const CBasicVector3 &vec );

    /// <summary>
    /// �C�ӂ̎��̎���̉�]�����s����s����Z�b�g�A�b�v����
    /// ��]�̎��́A���_��ʂ�Ȃ���΂Ȃ�Ȃ�
    /// 
    /// axis�͉�]�̎����`���A�P�ʃx�N�g���łȂ���΂Ȃ炢
    /// 
    /// theta�͉�]�̗ʁi���W�A���j�A����̃��[���Ő��̉�]���`����
    /// 
    /// ���s�ړ������̓��Z�b�g�����
    /// 
    /// </summary>
	inline CMatrix4x3& SetupRotate( const CBasicVector3 &axis , dectype theta );

    /// <summary>
    /// ��]�s������ɃZ�b�g�A�b�v����
    /// </summary>
	CMatrix4x3& SetupRotate( const CRotationMatrix &rotmat );

    /// <summary>
    /// �N�I�[�^�j�I�������ɃZ�b�g�A�b�v����
    /// </summary>
	CMatrix4x3& SetupRotate( const CQuaternion &q );

    /// <summary>
    /// ��]�s����쐬����
    /// </summary>
	const CRotationMatrix &CreateRotateMatrix( CRotationMatrix &r ) const;

    /// <summary>
    /// �R���̌������擾����(�P���s��̏ꍇ�w����( 1.0f , 0.0f , 0.0f ) �x����( 0.0f , 1.0f , 0.0f ) �y����( 0.0f , 0.0f , 1.0f )
	/// isNormalizeExec:���K�����邩���Ȃ���
    /// </summary>
	inline void    GetDirection( eAxis axis , CBasicVector3 &dest , skyBool isNormalizeExec = skyTrue ) const;

    /// <summary>
    /// 
    /// �e���ŃX�P�[�����O�����s����s����Z�b�g�A�b�v����
    /// k�����ϓ��ɃX�P�[�����O����ꍇ�́ACBasicVector3(k,k,k)�̌`���̃x�N�g����p����
    /// 
    /// ���s�ړ������̓��Z�b�g�����
    /// 
    /// </summary>
	inline CMatrix4x3& SetupScale( const CBasicVector3 &sc );

    /// <summary>
    /// 
    /// �e���ŃX�P�[�����O�����s����s����Z�b�g�A�b�v����
    /// k�����ϓ��ɃX�P�[�����O����ꍇ�́ACBasicVector3(k,k,k)�̌`���̃x�N�g����p����
    /// 
    /// </summary>
	inline CMatrix4x3& SetScale( const CBasicVector3 &sc );

    /// <summary>
    /// �R���̊g��k�������擾����
    /// </summary>
	inline dectype       GetScale( eAxis axis ) const;
	inline CBasicVector3 GetScale() const;

    /// <summary>
    /// 
    /// �C�ӂ̎��ɉ������X�P�[�����O�����s����s����Z�b�g�A�b�v����
    /// 
    /// axis�͒P�ʃx�N�g���Ŏw�肳���
    /// 
    /// ���s�ړ������̓��Z�b�g�����
    /// 
    /// </summary>
	inline CMatrix4x3& SetupScaleAlongAxis( const CBasicVector3 &axis , dectype k );

    /// <summary>
    /// ����f�����s����s����Z�b�g�A�b�v����
    /// 
    /// ����f�̎�ނ́A�P���x�[�X�ɂ������iaxis�j�̃C���f�b�N�X�Ŏw�肳���
    /// ���̍s��œ_�����W�ϊ������ꍇ�̌��ʂ́A�ȉ��̋[���R�[�h�Ŏ������Ƃ���ł���B
    /// 
    /// axis == eAxis_x �� y += s*x , z += t*x
    /// axis == eAxis_y �� x += s*y, z += t*y
    /// axis == eAxis_z �� x += s*z, y += t*z
    /// 
    /// ���s�ړ������̓��Z�b�g�����
    /// 
    /// 
    /// </summary>
	inline CMatrix4x3& SetupShear( eAxis axis , dectype s , dectype t );

    /// <summary>
    /// ���_��ʂ�ʏ�ւ̓��e�����s����s����Z�b�g�A�b�v����
    /// ���̖ʂ́A�P�ʃx�N�g�����ɐ����ł���
    /// </summary>
	inline CMatrix4x3& SetupProject( const CBasicVector3 &n );
	inline CMatrix4x3& SetupProject( const CBasicPlane &p , const CBasicVector3 &lightPos ); //lightPos�͏Ƃ炵�������̂���̃I�t�Z�b�g�l�Ƃ��Ă݂܂��B

    /// <summary>
    /// ��{���ɕ��s�ȖʂɊւ��郊�t���N�V���������s����s����Z�b�g�A�b�v����
    /// 
    /// axis�͂P���x�[�X�ɂ����C���f�b�N�X�ŁA���e�����ʂ��w�肷��
    /// 
    /// eAxis_x �� �ʂ������Ɋւ��郊�t���N�V����
    /// eAxis_y �� �ʂ������Ɋւ��郊�t���N�V����
    /// eAxis_z �� �ʂ������Ɋւ��郊�t���N�V����
    /// 
    /// ���s�ړ��͓K�؂ɐݒ肳���i���s�ړ��́A�� �I�� �O�̏ꍇ�ɂ����N���邩��ł���j
    /// 
    /// </summary>
	inline CMatrix4x3& SetupReflect( eAxis axis , dectype k = 0.0f );

    /// <summary>
    /// 
    /// ���_��ʂ�C�ӂ̕��ʂɊւ��郊�t���N�V���������s����s����Z�b�g�A�b�v����
    /// �P�ʃx�N�g�����́A���̖ʂɐ����ł���
    /// 
    /// ���s�ړ������̓��Z�b�g�����
    /// 
    /// </summary>
	inline CMatrix4x3& SetupReflect( const CBasicVector3 &n );

    /// <summary>
    /// 
    /// ���[���h���W�e�N�X�`�����W�ϊ��s��쐬
    /// 
    /// </summary>
	inline CMatrix4x3& SetupPosToUv();

    /// <summary>
    /// View�s����쐬����
    /// </summary>
	inline CMatrix4x3& SetupViewLH( const CBasicVector3 &eye , const CBasicVector3 &tag , const CBasicVector3 &up );
	inline CMatrix4x3& SetupViewRH( const CBasicVector3 &eye , const CBasicVector3 &tag , const CBasicVector3 &up );

    /// <summary>
    /// Projection�s����쐬����
    /// </summary>
	inline CMatrix4x3& SetupProjectionLH( dectype fovY , dectype width , dectype height , dectype zn , dectype zf );
	inline CMatrix4x3& SetupProjectionRH( dectype fovY , dectype width , dectype height , dectype zn , dectype zf );
	inline CMatrix4x3& SetupProjectionLH( dectype fovY , dectype aspect , dectype zn , dectype zf );
	inline CMatrix4x3& SetupProjectionRH( dectype fovY , dectype aspect , dectype zn , dectype zf );
	inline CMatrix4x3& SetupProjectionOffsetCenterLH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf ); //�r���[�|�[�g�̒��S��ς��鎞�Ȃ� 
	inline CMatrix4x3& SetupProjectionOffsetCenterRH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf ); //�r���[�|�[�g�̒��S��ς��鎞�Ȃ� 

    /// <summary>
    /// ���ˉe�s����쐬����
    /// </summary>
	inline CMatrix4x3& SetupOrthoMatrixLH( dectype width , dectype height , dectype zn , dectype zf );
	inline CMatrix4x3& SetupOrthoMatrixRH( dectype width , dectype height , dectype zn , dectype zf );
	inline CMatrix4x3& SetupOrthoMatrixOffsetCenterLH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf );//�r���[�|�[�g�̒��S��ς��鎞�Ȃ�
	inline CMatrix4x3& SetupOrthoMatrixOffsetCenterRH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf );//�r���[�|�[�g�̒��S��ς��鎞�Ȃ�
	
    /// <summary>
    /// �e�N�X�`�����W�ϊ��s����쐬����
    /// </summary>
	inline CMatrix4x3& SetupTexturePositionMatrix();

    /// <summary>
    /// �s���x�N�g���Ƃ��Ď擾
    /// </summary>
	inline CBasicVector3 &GetCol0( CBasicVector3 &dest ) const;
	inline CBasicVector3 &GetCol1( CBasicVector3 &dest ) const;
	inline CBasicVector3 &GetCol2( CBasicVector3 &dest ) const;
	inline CBasicVector3 &GetCol3( CBasicVector3 &dest ) const;

    /// <summary>
    /// �s���x�N�g���Ƃ��Đݒ�
    /// </summary>
	inline CMatrix4x3& SetCol0( CBasicVector3 &src , dectype w );
	inline CMatrix4x3& SetCol1( CBasicVector3 &src , dectype w );
	inline CMatrix4x3& SetCol2( CBasicVector3 &src , dectype w );
	inline CMatrix4x3& SetCol3( CBasicVector3 &src , dectype w );

    /// <summary>
    /// �z��ɃR�s�[����
    /// </summary>
	template< class T >
	void CopyArray( T *dest ) const;

    /// <summary>
    /// �v�����g����
    /// </summary>
	void DebugPrint() const;

    /// <summary>
    /// f32[16]�̔z���P�ʍs��ɏ���������
    /// </summary>
	static void IdentityArray( f32 *dest );
};

/// <summary>
/// �R����ZERO�x�N�g��
/// </summary>
static const CMatrix4x3 CMatrix4x3_Identity( 1.0f , 0.0f , 0.0f , 0.0f ,
	                                         0.0f , 1.0f , 0.0f , 0.0f ,
	                                         0.0f , 0.0f , 1.0f , 0.0f ,
	                                         0.0f , 0.0f , 0.0f , 1.0f );

} } }

/// <summary>
/// *���Z�q�̃I�[�o�[���C�h
/// 
/// �_�����W�ϊ�����
/// ����́A���̏�̐��`�㐔�L�@�Ɏ����ăx�N�g���N���X���g����悤�ɂ���
/// </summary>
inline sky::lib::math::CBasicVector3	operator*( const sky::lib::math::CBasicVector3 &p , const sky::lib::math::CMatrix4x3 &m );
inline sky::lib::math::CMatrix4x3		operator*( const sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CMatrix4x3 &m2);
inline sky::lib::math::CBasicVector3	&operator*=( sky::lib::math::CBasicVector3 &p , const sky::lib::math::CMatrix4x3 &m );
inline sky::lib::math::CMatrix4x3      &operator*=( sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CMatrix4x3 &m2 );

sky::lib::math::CMatrix4x3		operator*( const sky::lib::math::CRotationMatrix &m1 , const sky::lib::math::CMatrix4x3 &m2 );
sky::lib::math::CMatrix4x3		operator*( const sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CRotationMatrix &m2 );
sky::lib::math::CBasicVector3	operator*( const sky::lib::math::CBasicVector3 &p , const sky::lib::math::CRotationMatrix &m );
sky::lib::math::CBasicVector3	&operator*=( sky::lib::math::CBasicVector3 &p , const sky::lib::math::CRotationMatrix &m );
sky::lib::math::CMatrix4x3		&operator*=( sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CRotationMatrix &m2 );

/// <summary>
/// �s��̂S�~�S�����̍s�񎮂����Z����i��L*���Z�q�̃I�[�o�[���C�h�ł͂S�~�R�̌v�Z�������Ă��Ȃ��B���̏ꍇ���ƃr���[�s���v���W�F�N�V�����s��̌v�Z�ɂ͎g���Ȃ��j
/// </summary>
inline sky::lib::math::CMatrix4x3 MulMatrix4x4( const sky::lib::math::CMatrix4x3 &m1, const sky::lib::math::CMatrix4x3 &m2 );      //�s���M1 M2 M3 �s��w�l�̌v�Z���͂��ǂ��Ă��܂��B�J�����̌v�Z�Ƃ��͂���ŏ\��
inline sky::lib::math::CMatrix4x3 MulMatrix4x4Full( const sky::lib::math::CMatrix4x3 &m1, const sky::lib::math::CMatrix4x3 &m2 );  //�S�Ă̗v�f�������Ɗ|�����킹�Ă��܂��B�X�L�����b�V���A�j���[�V�����̏ꍇ���ꂶ��Ȃ��ƃ_���B
inline sky::lib::math::CBasicVector3 MulMatrix4x4( const sky::lib::math::CBasicVector3 &v, const sky::lib::math::CMatrix4x3 &m );   //�S�Ă̗v�f�������Ɗ|�����킹�Ă��܂��B�X�L�����b�V���A�j���[�V�����̏ꍇ���ꂶ��Ȃ��ƃ_���B
inline sky::lib::math::CBasicVector4 MulMatrix4x4( const sky::lib::math::CBasicVector4 &v, const sky::lib::math::CMatrix4x3 &m );   //�S�Ă̗v�f�������Ɗ|�����킹�Ă��܂��B�X�L�����b�V���A�j���[�V�����̏ꍇ���ꂶ��Ȃ��ƃ_���B

/// <summary>
/// �S���R�s��̕��s�ړ������̂ݍ�������
/// </summary>
inline sky::lib::math::CMatrix4x3 MulMatrix4x4TransOnly( const sky::lib::math::CMatrix4x3 &m1, const sky::lib::math::CMatrix4x3 &m2 );

/// <summary>
/// �s��̂R�~�R�����̍s�񎮂��v�Z����
/// </summary>
inline dectype	Determinant(const sky::lib::math::CMatrix4x3 &m);
inline dectype	Determinant4x4(const sky::lib::math::CMatrix4x3 &m);

/// <summary>
/// �t�s����v�Z����
/// determinant���\�b�h�ŏ��Z���ꂽ�ÓT�I�Ȑ����s���p����
/// </summary>
inline sky::lib::math::CMatrix4x3 Invert(const sky::lib::math::CMatrix4x3 &m);
inline sky::lib::math::CMatrix4x3 Invert4x4(const sky::lib::math::CMatrix4x3 &m);

/// <summary>
/// ���s�ړ��p�x�N�g���N���X��Ԃ�
/// </summary>
inline sky::lib::math::CBasicVector3	GetTranslationVector(const sky::lib::math::CMatrix4x3 &m);

/// <summary>
/// �e�����[�J�����W�ϊ��s��i���[���h�s�񁨃I�u�W�F�N�g�s��Ȃǁj��^��
/// �I�u�W�F�N�g�̈ʒu�����o��
/// 
/// ���̍s��́A���̂̍��W�ϊ���\���Ă���Ƃ����O�񂪂���
/// �X�P�[�����O�A�X�L���[�A�~���[���܂܂Ȃ�
///
/// </summary>
inline sky::lib::math::CBasicVector3	GetPositionFromParentToLocalMatrix(const sky::lib::math::CMatrix4x3 &m);

/// <summary>
/// ���[�J�����e�̍��W�ϊ��s��i�Ⴆ�΁A�I�u�W�F�N�g�s�񁨃��[���h�s��j��^���A
/// �I�u�W�F�N�g�̈ʒu�����o��
/// </summary>
inline sky::lib::math::CBasicVector3	GetPositionFromLocalToParentMatrix(const sky::lib::math::CMatrix4x3 &m);

/// <summary>
/// �s��̕��
/// </summary>
sky::lib::math::CMatrix4x3 Lerp(sky::lib::math::CMatrix4x3 a,sky::lib::math::CMatrix4x3 b,f32 t);

/// <summary>
/// �s�񂩂�N�I�[�^�j�I���֕ϊ�
/// </summary>
skyBool TransformRotMatToQuaternion(
    f32 &qx , f32 &qy , f32 &qz , f32 &qw ,
    f32 m11 , f32 m12 , f32 m13 ,
    f32 m21 , f32 m22 , f32 m23 ,
    f32 m31 , f32 m32 , f32 m33
);
skyBool TransformRotMatToQuaternion( sky::lib::math::CQuaternion &dest , const sky::lib::math::CMatrix4x3 &src );

#include "Matrix4x3.inl"