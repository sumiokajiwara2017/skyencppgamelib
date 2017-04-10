#pragma once

namespace sky { namespace lib { namespace math {

class CEntity3D;
typedef SmartPointer< CEntity3D >       CEntity3DPtr; //�X�}�[�g�|�C���^�^��`
typedef kfm::CFCurve< CEntity3D >       CEntity3DFCurve;
typedef kfm::CFCurvePlayer< CEntity3D > CEntity3DFCurvePlayer;

/// <summary>
/// �O�������W���N���X
/// CJoint�ɖ�����D��ꂽ������B�ߋ��̈�Y�ł��B
/// </summary>
class CEntity3D : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
    virtual ~CEntity3D(){ SAFE_DELETE( Before ); }

	/// <summary>
	/// �g��k��
	/// </summary>
	inline void SetSc( const CBasicVector3 &sc )
	{
		m_IsUpdate = skyTrue;
		m_Sc = sc;
	}
	inline void SetSc_X( dectype scX )
	{
		m_IsUpdate = skyTrue;
		m_Sc.X = scX;
	}
	inline void SetSc_Y( dectype scY )
	{
		m_IsUpdate = skyTrue;
		m_Sc.Y = scY;
	}
	inline void SetSc_Z( dectype scZ )
	{
		m_IsUpdate = skyTrue;
		m_Sc.Z = scZ;
	}
	inline void AddSc( const CBasicVector3 &sc )
	{
		m_IsUpdate = skyTrue;
		m_Sc += sc;
	}
	inline void AddSc_X( dectype scX )
	{
		m_IsUpdate = skyTrue;
		m_Sc.X += scX;
	}
	inline void AddSc_Y( dectype scY )
	{
		m_IsUpdate = skyTrue;
		m_Sc.Y += scY;
	}
	inline void AddSc_Z( dectype scZ )
	{
		m_IsUpdate = skyTrue;
		m_Sc.Z += scZ;
	}
	inline const CBasicVector3 &GetSc()  { return m_Sc; }
	inline const dectype        GetSc_X(){ return m_Sc.X; }
	inline const dectype        GetSc_Y(){ return m_Sc.Y; }
	inline const dectype        GetSc_Z(){ return m_Sc.Z; }

	/// <summary>
	/// ��]�i�x�Őݒ�j
	/// </summary>
	inline void SetRot( const CBasicAngleVector3 &rot )
	{
		m_IsUpdate = skyTrue;
		m_Rot = rot;
	}
	inline void SetRot_X( CBasicAngle rotX )
	{
		m_IsUpdate = skyTrue;
		m_Rot.X() = rotX;
	}
	inline void SetRot_Y( CBasicAngle rotY )
	{
		m_IsUpdate = skyTrue;
		m_Rot.Y() = rotY;
	}
	inline void SetRot_Z( CBasicAngle rotZ )
	{
		m_IsUpdate = skyTrue;
		m_Rot.Z() = rotZ;
	}
	inline void AddRot( const CBasicAngleVector3 &rot )
	{
		m_IsUpdate = skyTrue;
		m_Rot += rot;
	}
	inline void AddRot_X( CBasicAngle rotX )
	{
		m_IsUpdate = skyTrue;
		m_Rot.X() += rotX;
	}
	inline void AddRot_Y( CBasicAngle rotY )
	{
		m_IsUpdate = skyTrue;
		m_Rot.Y() += rotY;
	}
	inline void AddRot_Z( CBasicAngle rotZ )
	{
		m_IsUpdate = skyTrue;
		m_Rot.Z() += rotZ;
	}
	inline CBasicAngleVector3 &GetRot()  { return m_Rot; }
	inline CBasicAngle         GetRot_X(){ return m_Rot.X(); }
	inline CBasicAngle         GetRot_Y(){ return m_Rot.Y(); }
	inline CBasicAngle         GetRot_Z(){ return m_Rot.Z(); }

	/// <summary>
	/// �ʒu
	/// </summary>
	inline void SetPos( const CBasicVector3 &pos )
	{
		m_IsUpdate = skyTrue;
		m_Pos = pos;
	}
	inline void SetPos_X( dectype posX )
	{
		m_IsUpdate = skyTrue;
		m_Pos.X = posX;
	}
	inline void SetPos_Y( dectype posY )
	{
		m_IsUpdate = skyTrue;
		m_Pos.Y = posY;
	}
	inline void SetPos_Z( dectype posZ )
	{
		m_IsUpdate = skyTrue;
		m_Pos.Z = posZ;
	}
	inline void AddPos( const CBasicVector3 &pos )
	{
		m_IsUpdate = skyTrue;
		m_Pos += pos;
	}
	inline void AddPos_X( dectype posX )
	{
		m_IsUpdate = skyTrue;
		m_Pos.X += posX;
	}
	inline void AddPos_Y( dectype posY )
	{
		m_IsUpdate = skyTrue;
		m_Pos.Y += posY;
	}
	inline void AddPos_Z( dectype posZ )
	{
		m_IsUpdate = skyTrue;
		m_Pos.Z += posZ;
	}
	inline const CBasicVector3 &GetPos()  { return m_Pos; }
	inline const dectype        GetPos_X(){ return m_Pos.X; }
	inline const dectype        GetPos_Y(){ return m_Pos.Y; }
	inline const dectype        GetPos_Z(){ return m_Pos.Z; }

	/// <summary>
	/// ���e���s��ɂ��Ď擾
	/// </summary>
	inline const CMatrix4x3 &ColMatrix()
	{
		//�X�V���ꂽ��v�Z���Ȃ���
		if ( m_IsUpdate == skyTrue )
		{
			//�P���s��(�������݂����Ȃ��́j
			m_LocalMatrix.Identity();

//----�e���ݒl���烏�[���h�}�g���b�N�X���쐬����

			//�g�k���ݒ�i�������גႢ�j
			m_LocalMatrix.SetupScale( m_Sc );

			//��]�i�������׍����j
			CMatrix4x3 rot;
			m_LocalMatrix *= m_Rot.ToMatrixYXZ( rot );

			//�ړ��ݒ�i�������גႢ�j
			m_LocalMatrix.SetTranslation( m_Pos );

			//�X�V�t���O���I�t��
			m_IsUpdate = skyFalse;
		}

		//�g�k����]���ړ��}�g���b�N�X��Ԃ�
		return m_LocalMatrix;
	}

	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset()
	{
		m_LocalMatrix.Identity();

		m_Sc  = CBasicVector3( 1.0f , 1.0f , 1.0f );
		m_Rot = CBasicAngleVector3( 0.0f , 0.0f , 0.0f );
		m_Pos = CBasicVector3( 0.0f , 0.0f , 0.0f );
	}

	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	virtual CEntity3D& operator =( const CEntity3D& src )
	{
        m_Pos      = src.m_Pos;
        m_Sc       = src.m_Sc;
        m_Rot      = src.m_Rot;
		m_IsUpdate = skyTrue;
        return *this;
	}

	/// <summary>
	/// ==���Z�q�I�[�o�[���C�h
	/// </summary>
	skyBool operator ==( const CEntity3D& src ) const
	{
		return (
        m_Pos      == src.m_Pos &&
        m_Sc       == src.m_Sc  &&
        m_Rot      == src.m_Rot );
	}

	/// <summary>
	/// *���Z�q�I�[�o�[���C�h
	/// </summary>
	CEntity3D operator *( dectype src ) const
	{
		return CEntity3D( m_Pos * src , m_Rot * src , m_Sc * src );
	}
	CEntity3D operator *( const CEntity3D &src ) const
	{
		return CEntity3D( m_Pos * src.m_Pos , m_Rot * src.m_Rot , m_Sc * src.m_Sc );
	}

	/// <summary>
	/// -���Z�q�I�[�o�[���C�h
	/// </summary>
	CEntity3D operator -( const CEntity3D& src ) const
	{
		return CEntity3D( m_Pos - src.m_Pos , m_Rot - src.m_Rot , m_Sc - src.m_Sc );
	}

	/// <summary>
	/// +���Z�q�I�[�o�[���C�h
	/// </summary>
	CEntity3D operator +( const CEntity3D& src ) const
	{
		return CEntity3D( m_Pos + src.m_Pos , m_Rot + src.m_Rot , m_Sc + src.m_Sc );
	}

	/// <summary>
	/// �ߋ��̑S���i�g�p����ꍇ�͐ݒ肷��B�ʏ�P�t���[���O�B����ƌ��݂̒l�ɂ��R���W����������s���j
	/// </summary>
    CEntity3D *Before;

	/// <summary>
	/// �l���ߋ��̏��ɕۑ�����
	/// </summary>
    void Save(){ *Before = *this; }

	/// <summary>
	/// �쐬
	/// </summary>
	static CEntity3DPtr Create(
		const CBasicVector3 &pos = CBasicVector3( 0.0f , 0.0f , 0.0f ) , 
		const CBasicAngleVector3 &rot = CBasicAngleVector3( 0 , 0.0f , 0.0f , 0.0f ) ,
		const CBasicVector3 &sc = CBasicVector3( 1.0f , 1.0f , 1.0f ) )
	{
		
		return CEntity3DPtr( NEW__( CEntity3D , CEntity3D( pos , rot , sc ) ) );
	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
    CEntity3D() : Before( skyNull ) , m_IsUpdate( skyTrue ){ Reset(); }
    CEntity3D( 
		const CBasicVector3 &pos , 
		const CBasicAngleVector3 &rot = CBasicAngleVector3( 0 , 0.0f , 0.0f , 0.0f ) ,
		const CBasicVector3 &sc = CBasicVector3( 1.0f , 1.0f , 1.0f ) )
		: Before( skyNull ) , m_Pos( pos ) , m_Sc( sc ) , m_Rot( rot ) , m_IsUpdate( skyTrue ){ m_LocalMatrix.Identity(); }

	//---System�Ŏg�p
    //���擾�֐������O�̂��ߒl���X�V���ꂽ���Ƃɂ��Ă����B
	inline CBasicVector3 &GetScUnSafe(){ m_IsUpdate = skyTrue; return m_Sc; }
	inline dectype       &GetScUnSafe_X(){ m_IsUpdate = skyTrue; return m_Sc.X; }
	inline dectype       &GetScUnSafe_Y(){ m_IsUpdate = skyTrue; return m_Sc.Y; }
	inline dectype       &GetScUnSafe_Z(){ m_IsUpdate = skyTrue; return m_Sc.Z; }
	inline CBasicAngleVector3 &GetRotUnSafe(){ m_IsUpdate = skyTrue; return m_Rot; }
	inline CBasicAngle        &GetRotUnSafe_X(){ m_IsUpdate = skyTrue; return m_Rot.X(); }
	inline CBasicAngle        &GetRotUnSafe_Y(){ m_IsUpdate = skyTrue; return m_Rot.Y(); }
	inline CBasicAngle        &GetRotUnSafe_Z(){ m_IsUpdate = skyTrue; return m_Rot.Z(); }
	inline CBasicVector3 &GetPosUnSafe(){ m_IsUpdate = skyTrue; return m_Pos; }
	inline dectype       &GetPosUnSafe_X(){ m_IsUpdate = skyTrue; return m_Pos.X; }
	inline dectype       &GetPosUnSafe_Y(){ m_IsUpdate = skyTrue; return m_Pos.Y; }
	inline dectype       &GetPosUnSafe_Z(){ m_IsUpdate = skyTrue; return m_Pos.Z; }

private:

	/// <summary>
	/// ���[�J���}�g���b�N�X
	/// </summary>
	CMatrix4x3	m_LocalMatrix;

	/// <summary>
	/// �X�V�t���O
	/// </summary>
	skyBool         m_IsUpdate;

	/// <summary>
	/// �g��k��
	/// </summary>
	CBasicVector3	m_Sc;

	/// <summary>
	/// ��]
	/// </summary>
	CBasicAngleVector3	m_Rot;

	/// <summary>
	/// �ʒu
	/// </summary>
	CBasicVector3	m_Pos;
};

} } }
