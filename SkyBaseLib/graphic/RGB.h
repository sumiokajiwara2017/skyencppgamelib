#pragma once

namespace sky { namespace lib { namespace graphic {

template< typename T >
class CRGBA_4byte;

/// <summary>
/// RGB�F��ԃN���X
/// RGB�F��Ԃ͐F�̕\���@�̈��ŁA�� (Red)�A�� (Green)�A�� (Blue) �̎O�̌��F�������ĕ��L���F���Č�������@���F�̈��ł���B
/// RGB�͎O���F�̓������ł���B�u���E���ǁiCRT�j��t���f�B�X�v���C�iLCD�j�A�f�W�^���J�����Ȃǂŉ摜�Č��Ɏg���Ă���B
/// </summary>
template< typename T = colortype >
class CRGB : public base::SkyObject
{
public:

	/// <summary>
	/// �����o�ϐ�
	/// </summary>
    T r, g, b;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
    CRGB ( T lr = 0.0f, T lg = 0.0f, T lb = 0.0f );

	/// <summary>
	/// rgb�e�v�f�l��1.0f�ȓ��ɕ␳
	/// </summary>
    void Clamp		();

	/// <summary>
	/// rgb�e�v�f�̂����ő�̒l����Ɋe�v�f�l��1.0f�ȓ��ɕ␳
	/// </summary>
    void Scale		();

	/// <summary>
	/// 4�o�C�g�\����rgba�l�ɕϊ�
	/// </summary>
    void GetAs		( CRGBA_4byte< T >& rgba ) const;

	/// <summary>
	/// ������Z�q�̃I�[�o�[���C�h
	/// </summary>
    CRGB< T >& operator=	( T value );

	/// <summary>
	/// ��r���Z�q�̃I�[�o�[���C�h
	/// </summary>
    skyBool operator==	( const CRGB< T >& rgb )		const;
    skyBool operator!=	( const CRGB< T >& rgb )		const;

	/// <summary>
	/// �Z�p���Z�q�̃I�[�o�[���C�h
	/// </summary>
    CRGB< T > operator+	( const CRGB< T >& rgb )		const;
    CRGB< T > operator-	( const CRGB< T >& rgb )		const;
    CRGB< T > operator*	( T value )						const;
    CRGB< T > operator*	( const CRGB< T >& rgb )		const;
    CRGB< T > operator/	( T value )						const;
    CRGB< T > operator/	( const CRGB< T >& rgb )		const;
    CRGB< T > operator-	()								const;
    CRGB< T >& operator+= ( const CRGB< T >& rgb );
    CRGB< T >& operator-= ( const CRGB< T >& rgb );
    CRGB< T >& operator*= ( T value );
    CRGB< T >& operator*= ( const CRGB< T >& rgb );
    CRGB< T >& operator/= ( T value );
    CRGB< T >& operator/= ( const CRGB< T >& rgb );

};

/// <summary>
/// ��{RGB�^
/// </summary>
typedef CRGB< colortype >	CBasicRGB;

/// <summary>
/// �F�萔
/// </summary>
static const CBasicRGB RGB_White        ( 1.0f , 1.0f , 1.0f );
static const CBasicRGB RGB_Black        ( 0.0f , 0.0f , 0.0f ); 
static const CBasicRGB RGB_Red          ( 1.0f , 0.0f , 0.0f ); 
static const CBasicRGB RGB_Green        ( 0.0f , 1.0f , 0.0f ); 
static const CBasicRGB RGB_Blue         ( 0.0f , 0.0f , 1.0f ); 
static const CBasicRGB RGB_Tomato       ( 1.0f , 0.38f , 0.27f ); 
static const CBasicRGB RGB_Orangered    ( 1.0f , 0.27f , 0.0f ); 
static const CBasicRGB RGB_Sandybrown   ( 0.95f , 0.64f , 0.37f ); 
static const CBasicRGB RGB_Yellow       ( 1.0f , 1.0f , 0.0f ); 
static const CBasicRGB RGB_Violetw      ( 0.93f , 0.50f , 0.93f ); 
static const CBasicRGB RGB_Silver       ( 0.75f , 0.75f , 0.75f ); 

} } }