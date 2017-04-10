#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// ���[�V�����L�[�t���[���f�[�^�i�s��j
/// </summary>
struct CAnimationKeyFrameMatrix
{
	/// <summary>
	/// �L�[����
	/// </summary>
	u32    m_Time;

	/// <summary>
	/// �L�[�t���[���f�[�^
	/// </summary>
	math::CMatrix4x3  m_Matrix;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CAnimationKeyFrameMatrix()
	{
		m_Time = 0;
	}
	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	virtual CAnimationKeyFrameMatrix& operator =( const CAnimationKeyFrameMatrix& src )
	{
		m_Time   = src.m_Time;
		m_Matrix = src.m_Matrix;
		return *this;
	}
};
typedef CVector< CAnimationKeyFrameMatrix >      CAnimationKeyFrameMatrixArray;
typedef CVector< CAnimationKeyFrameMatrixArray > CAnimationKeyFrameMatrixArrayVector;

/// <summary>
/// ���[�V�����L�[�t���[���f�[�^�i�g�k�j
/// </summary>
struct CAnimationKeyFrameScale
{
	/// <summary>
	/// �L�[����
	/// </summary>
	u32    m_Time;

	/// <summary>
	/// �L�[�t���[���f�[�^
	/// </summary>
	math::CBasicVector3  m_Scale;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CAnimationKeyFrameScale()
	{
		m_Time = 0;
	}
	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	virtual CAnimationKeyFrameScale& operator =( const CAnimationKeyFrameScale& src )
	{
		m_Time  = src.m_Time;
		m_Scale = src.m_Scale;

		return *this;
	}
};
typedef CVector< CAnimationKeyFrameScale >      CAnimationKeyFrameScaleArray;
typedef CVector< CAnimationKeyFrameScaleArray > CAnimationKeyFrameScaleArrayVector;

/// <summary>
/// ���[�V�����L�[�t���[���f�[�^�i�ړ��j
/// </summary>
struct CAnimationKeyFrameTrans
{
	/// <summary>
	/// �L�[����
	/// </summary>
	u32    m_Time;

	/// <summary>
	/// �L�[�t���[���f�[�^
	/// </summary>
	math::CBasicVector3  m_Trans;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CAnimationKeyFrameTrans()
	{
		m_Time = 0;
	}
	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	virtual CAnimationKeyFrameTrans& operator =( const CAnimationKeyFrameTrans& src )
	{
		m_Time  = src.m_Time;
		m_Trans = src.m_Trans;

		return *this;
	}
};
typedef CVector< CAnimationKeyFrameTrans >      CAnimationKeyFrameTransArray;
typedef CVector< CAnimationKeyFrameTransArray > CAnimationKeyFrameTransArrayVector;

/// <summary>
/// ���[�V�����L�[�t���[���f�[�^�i��]�j
/// </summary>
struct CAnimationKeyFrameRot
{
	/// <summary>
	/// �L�[����
	/// </summary>
	u32    m_Time;

	/// <summary>
	/// �L�[�t���[���f�[�^
	/// </summary>
	math::CQuaternion m_Rot;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CAnimationKeyFrameRot()
	{
		m_Time = 0;
	}
	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	virtual CAnimationKeyFrameRot& operator =( const CAnimationKeyFrameRot& src )
	{
		m_Time = src.m_Time;
		m_Rot  = src.m_Rot;

		return *this;
	}
};
typedef CVector< CAnimationKeyFrameRot >      CAnimationKeyFrameRotArray;
typedef CVector< CAnimationKeyFrameRotArray > CAnimationKeyFrameRotArrayVector;


/// <summary>
/// ���[�V�����f�[�^
/// </summary>
struct CAnimationData
{
	/// <summary>
	/// ���O
	/// </summary>
	CName Name;

	/// <summary>
	/// �Ή��W���C���g��
	/// </summary>
	hash::CHashStringPtr m_JointName;

	/// <summary>
	/// �L�[�t���[���f�[�^�z��i�s��j
	/// </summary>
	CAnimationKeyFrameMatrixArray m_KeyFrameArrayMatrix;

	/// <summary>
	/// �L�[�t���[���f�[�^�z��i�g�k�j
	/// </summary>
	CAnimationKeyFrameScaleArray m_KeyFrameArrayScale;

	/// <summary>
	/// �L�[�t���[���f�[�^�z��i�ړ��j
	/// </summary>
	CAnimationKeyFrameTransArray m_KeyFrameArrayTrans;

	/// <summary>
	/// �L�[�t���[���f�[�^�z��i��]�j
	/// </summary>
	CAnimationKeyFrameRotArray   m_KeyFrameArrayRot;

	/// <summary>
	/// �L�[�t���[���f�[�^�z���ݒ肷��
	/// </summary>
	void SetKeyFrameMatrixArray( const CAnimationKeyFrameMatrixArray &keyFrameArray )
	{
		m_KeyFrameArrayMatrix.Clear();
		for( u32 i = 0 ; i < keyFrameArray.GetNum() ; i++ )
		{
			m_KeyFrameArrayMatrix.Add( keyFrameArray[ i ] );
		}
	}
	void SetKeyFrameScaleArray( const CAnimationKeyFrameScaleArray &keyFrameArray )
	{
		m_KeyFrameArrayScale.Clear();
		for( u32 i = 0 ; i < keyFrameArray.GetNum() ; i++ )
		{
			m_KeyFrameArrayScale.Add( keyFrameArray[ i ] );
		}
	}
	void SetKeyFrameTransArray( const CAnimationKeyFrameTransArray &keyFrameArray )
	{
		m_KeyFrameArrayTrans.Clear();
		for( u32 i = 0 ; i < keyFrameArray.GetNum() ; i++ )
		{
			m_KeyFrameArrayTrans.Add( keyFrameArray[ i ] );
		}
	}
	void SetKeyFrameRotArray( const CAnimationKeyFrameRotArray &keyFrameArray )
	{
		m_KeyFrameArrayRot.Clear();
		for( u32 i = 0 ; i < keyFrameArray.GetNum() ; i++ )
		{
			m_KeyFrameArrayRot.Add( keyFrameArray[ i ] );
		}
	}

	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	virtual CAnimationData& operator =( const CAnimationData& src )
	{
		Name                 = src.Name;
		m_JointName          = src.m_JointName;

		//�L�[�t���[���f�[�^�z��
		m_KeyFrameArrayMatrix.Clear();
		for( u32 i = 0 ; i < src.m_KeyFrameArrayMatrix.GetNum() ; i++ )
		{
			m_KeyFrameArrayMatrix.Add( src.m_KeyFrameArrayMatrix[ i ] );
		}

		m_KeyFrameArrayScale.Clear();
		for( u32 i = 0 ; i < src.m_KeyFrameArrayScale.GetNum() ; i++ )
		{
			m_KeyFrameArrayScale.Add( src.m_KeyFrameArrayScale[ i ] );
		}

		m_KeyFrameArrayTrans.Clear();
		for( u32 i = 0 ; i < src.m_KeyFrameArrayTrans.GetNum() ; i++ )
		{
			m_KeyFrameArrayTrans.Add( src.m_KeyFrameArrayTrans[ i ] );
		}

		m_KeyFrameArrayRot.Clear();
		for( u32 i = 0 ; i < src.m_KeyFrameArrayRot.GetNum() ; i++ )
		{
			m_KeyFrameArrayRot.Add( src.m_KeyFrameArrayRot[ i ] );
		}
		return *this;
	}
};
typedef CVector< CAnimationData > CAnimationDataPallet;

/// <summary>
/// �^��`
/// </summary>
class CMotionData;
typedef SmartPointer< CMotionData >	 CMotionDataPtr;					//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CMotionDataPtr > CMotionDataPtrList;
typedef CHash< CMotionDataPtr >       CMotionDataPtrHash;
typedef CVector< CMotionDataPtr >     CMotionDataPtrPallet;

/// <summary>
/// CStatic���b�V���̃��[�V�����f�[�^�N���X�ł�
/// ���̃C���X�^���X�P�ɂ��P���[�V�����i�����A����A�R��A���铙�����ꂩ��j
/// </summary>
class CMotionData : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// ���O
	/// </summary>
    CName Name;

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CMotionData();

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CMotionData();

	/// <summary>
	/// ���[�V�����p���b�g��ݒ�
	/// </summary>
	void SetAnimationPallet( const CAnimationDataPallet &pallet );

	/// <summary>
	/// ���[�V�����f�[�^����Ԃ��i�ʏ�W���C���g�̐��Ƃ��Ȃ��ɂȂ�͂��j
	/// </summary>
	u32 GetAnimationNum() const;

	/// <summary>
	///  ���[�V�����f�[�^�̎擾
	/// </summary>
	const CAnimationData &GetAnimationData( u32 index ) const;

	/// <summary>
	/// �ő厞��
	/// </summary>
	void SetMaxTime( f32 maxTime );
	f32  GetMaxTime() const;

//---Serialize
//�����[�V�����o�C�i���f�[�^�̓V�[���O���t�̃o�C�i���Ɋ܂߂Ȃ��B
//  �Ǝ��̃o�C�i���Ƃ��ĕۑ�����@

	/// <summary>
	/// �o�C�i���ɃZ�[�u
	/// </summary>
	skyBool SaveBinary_( const stream::IStreamPtr& stream );
			
	/// <summary>
	/// �o�C�i�����烍�[�h
	/// </summary>
	skyBool LoadBinary_( const stream::IStreamPtr& stream );

//---Static

	/// <summary>
	/// �쐬
	/// </summary>
	static CMotionDataPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CMotionDataPtr spResult = CMotionDataPtr( NEW__( CMotionData , CMotionData() ) );
		if ( name.IsEmpty() == skyFalse ) spResult->Name.SetName( name );
		return spResult;
	}
			
private:

	/// <summary>
	/// �ő厞��
	/// </summary>
	f32 m_MaxTime;

	/// <summary>
	/// ���[�V�����f�[�^�p���b�g
	/// �z��̃C���f�b�N�X�̓W���C���g�p���b�g�̃W���C���g�C���f�b�N�X�ƑΉ�
	/// �܂�W���C���g���̃��[�V�����������Ă���
	/// </summary>
	CAnimationDataPallet m_AnimationDataPallet;
};

#define CMotionDataCreate_()           sky::lib::math::CMotionData::Create()
#define CMotionDataCreateName_( name ) sky::lib::math::CMotionData::Create( name )

//-----------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// �^��`
/// </summary>
class CMotionDataContena;
typedef SmartPointer< CMotionDataContena >	CMotionDataContenaPtr;					//�X�}�[�g�|�C���^�Œ�`
typedef CList< u32 , CMotionDataContenaPtr > CMotionDataContenaPtrList;

/// <summary>
/// ���[�V�����f�[�^�R���e�i
/// �����̃��[�V�����i�����A����A�R��A����j���i�[����R���e�i�N���X
/// </summary>
class CMotionDataContena : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

	/// <summary>
	/// ���O
	/// </summary>
    CName Name;

	/// <summary>
	/// ���[�V�����̒ǉ��i�ǉ��̏��Ԃ��擾���̃C���f�b�N�X�ɂȂ�܂��j
	/// </summary>
	void AddAnimation( const CMotionDataPtr &spAnimation );

	/// <summary>
	/// ���[�V�������̎擾
	/// </summary>
	u32 GetAnimationNum();

	/// <summary>
	/// ���[�V�������C���f�b�N�X�Ŏ擾
	/// </summary>
	CMotionDataPtr GetAnimation( u32 index );

	/// <summary>
	/// ���[�V�����𖼑O�Ŏ擾
	/// </summary>
	CMotionDataPtr GetAnimation( const hash::CHashStringPtr &name );

//---Static

	/// <summary>
	/// �쐬
	/// </summary>
	static CMotionDataContenaPtr Create( const hash::CHashStringPtr &name = hash::CHashStringPtr() )
	{
		CMotionDataContenaPtr spResult = CMotionDataContenaPtr( NEW__( CMotionDataContena , CMotionDataContena() ) );
		if ( name.IsEmpty() == skyFalse ) spResult->Name.SetName( name );
		return spResult;
	}

private:

	/// <summary>
	/// ���[�V�����f�[�^�z��
	/// </summary>
	CMotionDataPtrPallet m_AnimationPallet;

	/// <summary>
	/// ���[�V�������O�R���e�i
	/// </summary>
	CMotionDataPtrHash   m_AnimationHash;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CMotionDataContena();
};

} } }

#define CMotionDataContenaCreate_()           sky::lib::math::CMotionDataContena::Create()
#define CMotionDataContenaCreateName_( name ) sky::lib::math::CMotionDataContena::Create( name )
