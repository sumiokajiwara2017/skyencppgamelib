#pragma once

namespace sky { namespace lib {

/// <summary>
/// RTTI���������邽�߂̃N���X
/// </summary>
/*
�g����

-------HogeBase.h----------------

//�ŏ�ʂ̐e�N���X
class HogeBase
{
    SkyTypeDefRootRTTI( HogeBase ); �������錾���遚
};

-------HogeBase.cpp--------------

SkyImplementRootRTTI( HogeBase ); �������錾���遚


-------Hoge.h--------------------

//�q�N���X
class Hoge : public HogeBase
{

SkyTypeDefRTTI; �������錾���遚

};

-------Hoge.cpp------------------

SkyImplementRTTI( Hoge , HogeBase ); �������錾���遚

*/
class CRTTI
{

public:

        /// <summary>
        /// �R���X�g���N�^
        /// </summary>
    CRTTI ( const skyMString* pName, const CRTTI* pBaseRTTI );

        /// <summary>
        /// ���O�擾
        /// </summary>
    inline const skyMString* GetName() const { return m_pName; }

        /// <summary>
        /// �e�N���X���擾
        /// </summary>
    inline const CRTTI* GetBaseRTTI() const { return m_pBaseRTTI; }

        /// <summary>
        /// ���O�R�s�[
        /// </summary>
    skyBool CopyName( skyMString* pNameBuffer , u32 maxSize ) const;

protected:

    const skyMString* m_pName;

    const CRTTI* m_pBaseRTTI;
};

} }

/// <summary>
/// �ŏ�ʃN���X��RTTI�錾������}�N��
/// </summary>
#define SkyTypeDefRootRTTI( classname )                                           \
    public:                                                                       \
        static const sky::lib::CRTTI ms_RTTI;                                   \
        virtual const sky::lib::CRTTI* GetRTTI() const                      \
       {                                                                          \
                   return &ms_RTTI;                                                       \
           }                                                                          \
       static skyBool IsExactKindOf( const sky::lib::CRTTI* pRTTI ,         \
           const classname* pObject )                                             \
       {                                                                          \
           if ( !pObject )                                                        \
           {                                                                      \
               return skyFalse;                                                   \
           }                                                                      \
           return pObject->IsExactKindOf( pRTTI );                                \
        }                                                                         \
        skyBool IsExactKindOf( const sky::lib::CRTTI* pRTTI ) const         \
        {                                                                         \
            return ( GetRTTI() == pRTTI );                                        \
        }                                                                         \
        static skyBool IsKindOf ( const sky::lib::CRTTI* pRTTI ,            \
            const classname* pObject )                                            \
        {                                                                         \
            if ( !pObject )                                                       \
            {                                                                     \
                return skyFalse;                                                  \
            }                                                                     \
            return pObject->IsKindOf( pRTTI );                                    \
        }                                                                         \
        skyBool IsKindOf( const sky::lib::CRTTI* pRTTI ) const              \
        {                                                                         \
            const sky::lib::CRTTI* pTmp = GetRTTI();                        \
            while ( pTmp )                                                        \
            {                                                                     \
                if ( pTmp == pRTTI )                                              \
                {                                                                 \
                    return skyTrue;                                               \
                }                                                                 \
                pTmp = pTmp->GetBaseRTTI();                                       \
            }                                                                     \
            return skyFalse;                                                      \
        }                                                                         \
        static classname* DynamicCast( const sky::lib::CRTTI* pRTTI ,       \
            const classname* pObject )                                            \
        {                                                                         \
            if ( !pObject )                                                       \
            {                                                                     \
                return skyNull;                                                   \
            }                                                                     \
            return pObject->DynamicCast( pRTTI );                                 \
        }                                                                         \
        classname* DynamicCast( const sky::lib::CRTTI* pRTTI ) const        \
        {                                                                         \
            return ( IsKindOf( pRTTI ) ? ( classname* ) this : 0 );               \
        }

/// <summary>
/// �q�N���X��RTTI�錾������}�N��
/// </summary>
#define SkyTypeDefRTTI                                                            \
    public:                                                                       \
        static const sky::lib::CRTTI ms_RTTI;                               \
        virtual const sky::lib::CRTTI* GetRTTI() const { return &ms_RTTI; }

/// <summary>
/// �ŏ�ʃN���X��RTTI����������}�N��
/// </summary>
#define SkyImplementRootRTTI( rootclassname )                                     \
    const sky::lib::CRTTI rootclassname::ms_RTTI( #rootclassname , 0 )

/// <summary>
/// �q�N���X��RTTI����������}�N��
/// </summary>
#define SkyImplementRTTI( classname , baseclassname )                             \
    const sky::lib::CRTTI classname::ms_RTTI( #classname , &baseclassname::ms_RTTI )

/// <summary>
/// ���s���^�`�F�b�N�}�N��
/// </summary>
#define SkyIsExactKindOf( classname , pObject )                                  \
    classname::IsExactKindOf( &classname::ms_RTTI , pObject )

/// <summary>
/// ���s���^�`�F�b�N�}�N��
/// </summary>
#define SkyIsKindOf( classname , pObject )                                       \
    classname::IsKindOf( &classname::ms_RTTI , pObject )

/// <summary>
/// �R���p�C�����^�L���X�g�}�N��
/// </summary>
#define SkyStaticCast( classname , pObject )                                     \
    ( ( classname * ) pObject )

/// <summary>
/// �R���p�C�����^�L���X�g�}�N���i�f�o�b�O���s���A�T�[�g�j
/// </summary>
#ifdef SW_SKYLIB_DEBUG_ON
    #define SkyVerifyStaticCast( classname , pObject )                           \
        ( ( classname * ) classname::VerifyStaticCastDebug(                      \
            &classname::ms_RTTI , pObject ) )
#else
    #define SkyVerifyStaticCast( classname, pObject) ( ( classname * ) ( pObject ) )
#endif

/// <summary>
/// ���s���^�L���X�g�}�N���i���s�̏ꍇskyNull��Ԃ��j
/// </summary>
#define SkyDynamicCast( classname , pObject )                                    \
    ( ( classname * ) classname::DynamicCast( &classname::ms_RTTI , pObject ) )
