#pragma once

namespace sky { namespace lib { namespace kfm {

static const u32 CFCURVE_DEFAULT_KEY_NUM = 16;

/// <summary>
/// �t�@���N�V�����J�[�u��{�N���X
/// </summary>
class CFCurveBase : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CFCurveBase();

	/// <summary>
	/// ���ԕ��i�o�^����Ă���L�[��Time�̍��v�j
	/// </summary>
	inline updateTime GetTimeRange() const;

	/// <summary>
	/// �V�l�Z�o���C�x���g�擾
	/// </summary>
	inline delegate::CDelegateHandler &GetColValueEventHndl();

	/// <summary>
	/// �L�[�̃N���A
	/// </summary>
	virtual void ClearKey() = 0;

protected:

	/// <summary>
	/// �t�@���N�V�����J�[�u����������܂ł̍��v����
	/// </summary>
	updateTime m_TimeRange;

	/// <summary>
	/// �V�l�Z�o���C�x���g�n���h��
	/// </summary>
	delegate::CDelegateHandler  m_ColValueEventHndl;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	inline CFCurveBase();
};

/// <summary>
/// �t�@���N�V�����J�[�u
/// </summary>
template < class T >
class CFCurve : public CFCurveBase
{
	/// <summary>
	/// �\�[�g���W�b�N��`
	/// </summary>
	class SortPred
	{
	public:
		skyBool operator()( const CFKey< T > *key1 , const CFKey< T > *key2 )
		{
			return key1->GetTime() < key2->GetTime();
		}
	};

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	inline virtual ~CFCurve();

	/// <summary>
	/// �^��`
	/// </summary>
	typedef CVectorPrimitive< CFKey< T > * > CKeyVector;

	/// <summary>
	/// �����l�̐ݒ�
	/// </summary>
	void SetInitValue( T value );
	T&   GetInitValue();

	/// <summary>
	/// �L�[�̒ǉ�
	/// </summary>
	void AddKey( updateTime time , T keyValue , eInterpolateType eType );

	/// <summary>
	/// �L�[�̎擾
	/// </summary>
	skyBool GetKey( u32 index , CFKey< T >* &pDest );

	/// <summary>
	/// �L�[�̍폜
	/// </summary>
	void        DeleteKey( CFKey< T >* &pKey );
	CFKey< T > *DeleteKey( u32 index );

	/// <summary>
	/// �x�[�X�̏������z�֐��̎���
	/// </summary>
	skyBool     ColTimeValue( updateTime time , f32 weight = 1.0f , skyBool isCallBackValue = skyTrue , const CFCurve< T > *pBlendFCurve = skyNull );
	const T&    GetTimeValue() const;
	void        ClearKey();

	/// <summary>
	/// �L�[���X�g�̍œK���i�L�[�ƃL�[�̒l�̊Ԃ��߂��ꍇ�L�[�����炷�j
	/// </summary>
	void Optimization();

	/// <summary>
	/// �R���X�g���N�^
	/// �L�[��������ݒ肳��Ă���ꍇ�A�ŏ��̕�Ԃ͂��̃R���X�g���N�^�ɐݒ肳�ꂽ�����l����L�[�ւ̕�ԂɂȂ�܂��B
	/// </summary>
	CFCurve( T initValue , u32 keyNum = CFCURVE_DEFAULT_KEY_NUM ) : m_InitValue( initValue ) , m_KeyArray( ( s32 )keyNum ){}

//---system

	/// <summary>
	/// �L�[�Ǘ��z��̎Q�Ƃ̎擾�i�f�o�b�O�v�����g���Ɏg�p�j
	/// ���̃��X�g���g�p���ē��e��ύX���Ȃ��ł��������B
	/// </summary>
	CKeyVector &GetKeyArray();

private:

	/// <summary>
	/// �Ǘ��L�[�i�[�z��
	/// </summary>
	CKeyVector m_KeyArray;

	/// <summary>
	/// �����l
	/// </summary>
	T m_InitValue;

	/// <summary>
	/// GetTimeValue()�ōŌ�Ɏ擾���ꂽ�l
	/// </summary>
	T m_TimeValue;
};

//�悭�g�p����^�̃t�@���N�V�����J�[�u��`
typedef CFCurve< dectype >       DecTypeFCurve;

} } }

#include "FCurve.inl"