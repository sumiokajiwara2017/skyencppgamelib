#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// RGBA�F��ԃN���X
/// RGB�̐F��Ԃɉ����āA�A���t�@�`�����l�����F����ɍl��������B����́A���߁i�����x�j��\��������̂ł���i�����ɂ͂���͐F��Ԃł͂Ȃ��Ƃ��l������j�B
/// </summary>
template< typename T = colortype >
class CRGBA : public base::SkyObject
{
public:

	/// <summary>
	/// �����o�ϐ�
	/// </summary>
    T r, g, b, a;

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
    CRGBA (T lr = 0.0f, T lg = 0.0f, T lb = 0.0f, T la = 0.0f);

	/// <summary>
	/// rgb�e�v�f�l��1.0f�ȓ��ɕ␳
	/// </summary>
    void Clamp();

	/// <summary>
	/// rgb�e�v�f�̂����ő�̒l����Ɋe�v�f�l��1.0f�ȓ��ɕ␳
	/// </summary>
    void Scale();

	/// <summary>
	/// 4�o�C�g�\����rgba�l�ɕϊ�
	/// </summary>
    void GetAs( CRGBA_4byte< T >& rgba ) const;
    u32  GetAs() const;
    void GetAsARGB( CRGBA_4byte< T >& argb ) const;
    u32  GetAsARGB() const;
    void GetAsABGR( CRGBA_4byte< T >& argb ) const;
    u32  GetAsABGR() const;

	/// <summary>
	/// �z��Ƀf�[�^���R�s�[����
	/// </summary>
	void Copyf32Array3RGB( f32 *dest ) const;
	void Copyf32Array4RGBA( f32 *dest ) const;

	/// <summary>
	/// ������Z�q�̃I�[�o�[���C�h
	/// </summary>
    CRGBA< T >& operator= ( T value );
    CRGBA< T >& operator= ( const CRGBA< T >& rgba );

	/// <summary>
	/// ��r���Z�q�̃I�[�o�[���C�h
	/// </summary>
    skyBool operator== ( const CRGBA< T >& rgba ) const;
    skyBool operator!= ( const CRGBA< T >& rgba ) const;

	/// <summary>
	/// �Z�p���Z�q�̃I�[�o�[���C�h
	/// </summary>
    CRGBA< T > operator+ ( const CRGBA< T >& rgba ) const;
    CRGBA< T > operator- ( const CRGBA< T >& rgba ) const;
    CRGBA< T > operator* ( T value )			const;
    CRGBA< T > operator* ( const CRGBA< T >& rgba ) const;
    CRGBA< T > operator/ ( T value )			const;
    CRGBA< T > operator/ ( const CRGBA< T >& rgba ) const;
    CRGBA< T > operator- ()					const;

    CRGBA< T >& operator+= ( const CRGBA< T >& rgba );
    CRGBA< T >& operator-= ( const CRGBA< T >& rgba );
    CRGBA< T >& operator*= ( T value );
    CRGBA< T >& operator*= ( const CRGBA< T >& rgba );
    CRGBA< T >& operator/= ( T value );
    CRGBA< T >& operator/= ( const CRGBA< T >& rgba );
};

/// <summary>
/// ��{RGBA�^
/// </summary>
typedef CRGBA< colortype >	CBasicRGBA;
typedef CBasicRGBA			CColor;
typedef CVectorPrimitive< CColor > CVector_CColor;

/// <summary>
/// �F�萔
/// </summary>
static const CBasicRGBA CColor_White            ( 1.0f , 1.0f , 1.0f , 1.0f );
static const CBasicRGBA CColor_Black            ( 0.0f , 0.0f , 0.0f , 1.0f ); 
static const CBasicRGBA CColor_Red              ( 1.0f , 0.0f , 0.0f , 1.0f ); 
static const CBasicRGBA CColor_Green            ( 0.0f , 1.0f , 0.0f , 1.0f ); 
static const CBasicRGBA CColor_Blue             ( 0.0f , 0.0f , 1.0f , 1.0f ); 
static const CBasicRGBA CColor_DeepBlue         ( 0.0f , 0.125f, 0.3f, 1.0f ); 
static const CBasicRGBA CColor_Tomato           ( 1.0f , 0.38f , 0.27f , 1.0f ); 
static const CBasicRGBA CColor_Orangered        ( 1.0f , 0.27f , 0.0f , 1.0f ); 
static const CBasicRGBA CColor_Sandybrown       ( 0.95f, 0.64f , 0.37f, 1.0f ); 
static const CBasicRGBA CColor_Yellow           ( 1.0f , 1.0f  , 0.0f , 1.0f ); 
static const CBasicRGBA CColor_Violetw          ( 0.93f, 0.50f , 0.93f, 1.0f ); 
static const CBasicRGBA CColor_Silver           ( 0.75f , 0.75f , 0.75f , 1.0f ); 
static const CBasicRGBA CColor_Dimgray          ( 0.41f , 0.41f , 0.41f , 1.0f ); 
static const CBasicRGBA CColor_Lightseagreen    ( 0.12f , 0.69f , 0.66f , 1.0f ); 
static const CBasicRGBA CColor_Powderblue       ( 0.69f , 0.87f , 0.90f , 1.0f ); 
static const CBasicRGBA CColor_Mistyrose        ( 0.88f , 0.89f , 0.88f , 1.0f ); 
static const CBasicRGBA CColor_Lightsteelblue   ( 0.69f , 0.76f , 0.87f , 1.0f ); 
static const CBasicRGBA CColor_Wheat            ( 0.96f , 0.87f , 0.70f , 1.0f ); 
static const CBasicRGBA CColor_Gold             ( 1.00f , 0.84f , 0.00f , 1.0f ); 

} } }