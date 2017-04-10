#pragma once

namespace sky { namespace lib { namespace comp {

/// <summary>
/// ���������k�̂��߂�zlib���g�p���Ă��܂�������
/// �f�t�H���g���k���[�h(fopen�̃��[�h + ���k���x�� + ���k���� )
///
/// �yfopen�̃��[�h�z
/// rb : �o�C�i���ǂݍ��݃��[�h
/// wb : �o�C�i���������݃��[�h
///
/// �y���k���x���z
/// 0 : ���k�Ȃ�
/// 1 : �������x�d��
/// 9 : ���k�����d��
////
/// �y���k���x���z
/// f : �t�B���^ p
/// h : �n�t�}���������̂�
/// R : ���������O�X
///
/// �𓚌�̃t�@�C���T�C�Y�����O�Ɏ����@�����̂Ƃ��날��܂���B
/// �f�[�^���𓚂���e�A�v�������ɂ����āA�ő�l�����ꂼ�ꉼ�肵�Ďg���Ă��������B
/// </summary>
static const skyMString *DEFAULT_WRITE_GZ_MODE = "wb6f";
static const skyMString *DEFAULT_READ_GZ_MODE = "rb";

/// <summary>
/// Zlib���[�e�B���e�B�֐��Q
/// </summary>
namespace CCompressUtil
{
	/// <summary>
	/// �w��̃f�[�^�����k
	/// </summary>
	skyBool CompressFile( const u8 *pSrcData , u32 srcSize , const hash::CHashStringPtr &pDestPath , const skyMString *pWriteOpenMode = DEFAULT_WRITE_GZ_MODE );
	skyBool CompressData( const u8 *pSrcData , u32 srcSize , u8* pDestData , u32 &destSize );

	/// <summary>
	/// �w��̈��k�f�[�^����
	/// </summary>
	skyBool UnCompressFile( const u8 *pDestData , u32 destBufSize , u32 &destReadSize , const hash::CHashStringPtr &pSrcPath , const skyMString *pReadOpenMode = DEFAULT_READ_GZ_MODE );
	skyBool UnCompressData( const u8 *pSrcData , u32 srcSize , u8* pDestData , u32 &destSize );
}

#define CCompressUtil_					sky::lib::comp::CCompressUtil

} } }