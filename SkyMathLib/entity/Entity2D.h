#pragma once

namespace sky { namespace lib { namespace math {

class CEntity2D;
typedef SmartPointer< CEntity2D >       CEntity2DPtr; //�X�}�[�g�|�C���^�^��`
typedef kfm::CFCurve< CEntity2D >       CEntity2DFCurve;
typedef kfm::CFCurvePlayer< CEntity2D > CEntity2DFCurvePlayer;

/// <summary>
/// �񎟌����W���N���X
/// CJoint�ɖ�����D��ꂽ������B�ߋ��̈�Y�ł��B
/// </summary>
class CEntity2D : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
    virtual ~CEntity2D(){ SAFE_DELETE( Before ); }

	/// <summary>
	/// �g��k��
	/// </summary>
	inline void SetSc( const CBasicVector2 &sc )
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
	inline void AddSc( const CBasicVector2 &sc )
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
	inline const CBasicVector2 &GetSc(){ return m_Sc; }
	inline const dectype        GetSc_X(){ return m_Sc.X; }
	inline const dectype        GetSc_Y(){ return m_Sc.Y; }

	/// <summary>
	/// ��]
	/// </summary>
	inline void SetRot( CBasicAngle angle )
	{
		m_IsUpdate = skyTrue;
		m_Rot = angle;
	}
	inline void AddRot( CBasicAngle angle )
	{
		m_IsUpdate = skyTrue;
		m_Rot += angle;
	}
	inline const CBasicAngle &GetRot(){ return m_Rot.Rot(); }

	/// <summary>
	/// �ʒu
	/// </summary>
	inline void SetPos( const CBasicVector2 &pos )
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
	inline void AddPos( const CBasicVector2 &pos )
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
	inline const CBasicVector2 &GetPos(){ return m_Pos; }
	inline const dectype        GetPos_X(){ return m_Pos.X; }
	inline const dectype        GetPos_Y(){ return m_Pos.Y; }

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
			m_LocalMatrix.SetupScale( CBasicVector3( m_Sc.X , m_Sc.Y , 0.0f ) );

			//��]�i�������׍����j
			CMatrix4x3 rotMat;
			m_Rot.ToMatrix( rotMat );
			m_LocalMatrix *= rotMat;

			//�ړ��ݒ�i�������גႢ�j
			m_LocalMatrix.SetTranslation( CBasicVector3( m_Pos.X , m_Pos.Y , 0.0f ) );

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

		m_Pos = CBasicVector2( 0.0f , 0.0f );
		m_Sc  = CBasicVector2( 1.0f , 1.0f );
		m_Rot = 0.0f;
	}

	/// <summary>
	/// ������Z�q�I�[�o�[���C�h
	/// </summary>
	virtual CEntity2D& operator =( const CEntity2D& src )
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
	skyBool operator ==( const CEntity2D& src ) const
	{
		return ( 
		m_Pos      == src.m_Pos &&
		m_Sc       == src.m_Sc  &&
		m_Rot      == src.m_Rot );
	}

	/// <summary>
	/// *���Z�q�I�[�o�[���C�h
	/// </summary>
	CEntity2D operator *( dectype src ) const
	{
		return CEntity2D( m_Pos * src , m_Rot * src , m_Sc * src );
	}
	CEntity2D operator *( const CEntity2D &src ) const
	{
		return CEntity2D( m_Pos * src.m_Pos , m_Rot * src.m_Rot , m_Sc * src.m_Sc );
	}

	/// <summary>
	/// -���Z�q�I�[�o�[���C�h
	/// </summary>
	CEntity2D operator -( const CEntity2D& src ) const
	{
		return CEntity2D( m_Pos - src.m_Pos , m_Rot - src.m_Rot , m_Sc - src.m_Sc );
	}

	/// <summary>
	/// +���Z�q�I�[�o�[���C�h
	/// </summary>
	CEntity2D operator +( const CEntity2D& src ) const
	{
		return CEntity2D( m_Pos + src.m_Pos , m_Rot + src.m_Rot , m_Sc + src.m_Sc );
	}

	/// <summary>
	/// �ߋ��̑S���i�ʏ�P�t���[���O�B����ƌ��݂̒l�ɂ��R���W����������s���j
	/// </summary>
	CEntity2D *Before;

	/// <summary>
	/// �R�c�G���e�B�e�B�̏����쐬����
	/// ���̏������g�p����̂�CvertexBuffer�̃Z�b�g�A�b�v������ȏꍇ�Ɍ��肵�����������B
	/// �v�Z�ʂ����炷���߂ɂQ�c�ł���Ă���̂ɑ����i�K�łR�c�ɖ߂�������Ƃ������Ȃ��̂ŁB
	/// </summary>
	CEntity3DPtr Create3D() const
	{
		return CEntity3D::Create
        (
		    CBasicVector3( m_Pos.X , m_Pos.Y , 0.0f ) ,
			CBasicAngleVector3( 0.0f , 0.0f , m_Rot.Rot() ) ,
			CBasicVector3( m_Sc.X , m_Sc.Y , 1.0f )
        );
	}

	/// <summary>
	/// �l���ߋ��̏��ɕۑ�����
	/// </summary>
	void Save(){ *Before = *this; }

	/// <summary>
	/// �쐬
	/// </summary>
	static CEntity2DPtr Create(
		const CBasicVector2 &pos = CBasicVector2( 0 , 0 ) , 
		const dectype rot = 0.0f ,
		const CBasicVector2 &sc = CBasicVector2( 1.0f , 1.0f ) )
	{
		
		return CEntity2DPtr( NEW__( CEntity2D , CEntity2D( pos , rot , sc ) ) );
	}

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
    CEntity2D() : Before( skyNull ) , m_IsUpdate( skyFalse ){ Reset(); }
    CEntity2D( 
		const CBasicVector2 &pos , 
		const CBasicAngleVector2 rot = 0.0f ,
		const CBasicVector2 &sc = CBasicVector2( 1.0f , 1.0f ) )
		: Before( skyNull ) , m_Pos( pos ) , m_Sc( sc ) , m_Rot( rot ) , m_IsUpdate( skyTrue ){ m_LocalMatrix.Identity(); }

	//---System�Ŏg�p
    //���擾�֐������O�̂��ߒl���X�V���ꂽ���Ƃɂ��Ă����B
	inline CBasicVector2      &GetScUnSafe(){ m_IsUpdate = skyTrue; return m_Sc; }
	inline dectype            &GetScUnSafe_X(){ m_IsUpdate = skyTrue; return m_Sc.X; }
	inline dectype            &GetScUnSafe_Y(){ m_IsUpdate = skyTrue; return m_Sc.Y; }
	inline CBasicAngleVector2 &GetRotUnSafe(){ m_IsUpdate = skyTrue; return m_Rot; }
	inline CBasicVector2      &GetPosUnSafe(){ m_IsUpdate = skyTrue; return m_Pos; }
	inline dectype            &GetPosUnSafe_X(){ m_IsUpdate = skyTrue; return m_Pos.X; }
	inline dectype            &GetPosUnSafe_Y(){ m_IsUpdate = skyTrue; return m_Pos.Y; }
	inline void UpdateFlagOn(){ m_IsUpdate = skyTrue; }

private:

	/// <summary>
	/// ���[�J���}�g���b�N�X
	/// </summary>
	CMatrix4x3	m_LocalMatrix;

	/// <summary>
	/// �X�V�t���O
	/// </summary>
	skyBool             m_IsUpdate;

	/// <summary>
	/// �ʒu
	/// </summary>
	CBasicVector2	m_Pos;

	/// <summary>
	/// �g��k��
	/// </summary>
	CBasicVector2	m_Sc;

	/// <summary>
	/// ��]
	/// </summary>
	CBasicAngleVector2	m_Rot;
};

} } }
