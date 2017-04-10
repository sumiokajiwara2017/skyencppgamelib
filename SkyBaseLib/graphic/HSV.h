#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// HSV�F��ԃN���X
/// H:�F�� - �F�̎�ށi�ԁA�A���F�̂悤�ȁj�B0�`360�͈̔́i�A�v���P�[�V�����ɂ���Ă�0�`100%�ɐ��K������邱�Ƃ�����j�B
/// S:�ʓx - �F�̑N�₩���B0�`100%�͈̔́B�h�����x��colorimeric purity�̐F�ʓI�ȗʂƔ�r���āu���x�v�ȂǂƂ������B�F�̍ʓx�̒ቺ�ɂ�āA�D�F���������ɂȂ�A�����񂾐F������A�܂��ʓx�̋t�Ƃ��āudesaturation�v���`����ƗL�v�ł���B
/// V:���x - �F�̖��邳�B0�`100%�͈̔́B
/// </summary>
template< typename T = colortype >
struct CHSV : public base::SkyObject
{

	/// <summary>
	/// �����o�ϐ�
	/// </summary>
	T	h;		//�F���i0.0�`360.0�j
	T	s;		//�ʓx�i0.0�`1.0�j
	T	v;		//���x�i0.0�`1.0�j
		
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CHSV() {}
	CHSV( T fH, T fS, T fV ) :
		h	( fH ),
		s	( fS ),
		v	( fV ){}
	
	/// <summary>
	/// CRGB�l��ݒ�
	/// </summary>
	void FromCRGB( const CRGB< T >& rgb );
		
	/// <summary>
	/// CRGB�l���擾
	/// </summary>
	CRGB< T > ToCRGB() const;

	/// <summary>
	/// �F�����擾
	/// </summary>
	static CRGBA_4byte< T > GetHueColor( T hue );

	/// <summary>
	/// CRGBA_4byte�l��ݒ�
	/// </summary>
	void FromCRGBA_4byte( const CRGBA_4byte< T > &rgba );
		
	/// <summary>
	/// CRGBA_4byte�l���擾
	/// </summary>
	CRGBA_4byte< T > ToCRGBA_4byte() const;
		
	/// <summary>
	/// CRGBA�l��ݒ�
	/// </summary>
	void FromCRGBA( const CRGBA< T > &rgba );
		
	/// <summary>
	/// CRGBA�l���擾
	/// </summary>
	CRGBA< T > ToCRGBA() const;
		
	/// <summary>
	/// ���Z�q�I�[�o�[���C�h
	/// </summary>
	CHSV operator* ( T value ) const
	{
		return HSV( h * value, s * value, v * value );
	}
	CHSV operator+ ( const CHSV& kHSV ) const
	{
		return HSV( h + kHSV.h, s + kHSV.s, v + kHSV.v );
	}
	CHSV operator- ( const CHSV& kHSV ) const
	{
		return HSV( h - kHSV.h, s - kHSV.s, v - kHSV.v );
	}
	skyBool operator== ( const CHSV& kHSV ) const
	{
		//0.00001�ȉ��̍��Ȃ�Γ����F�Ɣ��肷��
		return ( fabs( h - kHSV.h ) + fabs( s - kHSV.s ) + fabs( v - kHSV.v ) ) <= 0.00001f;
	}

};

/// <summary>
/// ��{HSV�^
/// </summary>
typedef CHSV< colortype >	CBasicHSV;

/// <summary>
/// �F�萔
/// </summary>
static const CBasicHSV CHSV_DeepskyBlue          ( 195.0f , 1.0f , 0.5f );
static const CBasicHSV CHSV_Red                  ( 29.0f  , 1.0f , 0.5f );
static const CBasicHSV CHSV_Lawngreen            ( 90.5f  , 1.0f , 0.5f );

} } }