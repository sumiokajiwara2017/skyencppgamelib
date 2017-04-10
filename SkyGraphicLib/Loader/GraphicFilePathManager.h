#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// �O���t�B�b�N�t�@�C���̃p�X���Ǘ�����
/// </summary>
class CGraphicFilePathManager : public base::SkyObject
{

public:

	/// <summary>
	/// ������
	/// </summary>
	static void    Initialize();

	/// <summary>
	/// ������
	/// </summary>
	static void    Dispose();

	/// <summary>
	/// ���[�h�b�n�k�k�`�c�`�t�@�C�����[�g�p�X�ݒ�^�擾�^����
	/// </summary>
	static void SetColladaFilePathRoot( const hash::CHashStringPtr &path = hash::CHashStringPtr() );
	static const hash::CHashStringPtr &GetColladaFilePathRoot();
	static skyWString* ConcatColladaFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath );
	static skyMString* ConcatColladaFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath );
	static void ClearColladaFilePathRoot();

	/// <summary>
	/// ���[�h�e�a�w�t�@�C�����[�g�p�X�ݒ�^�擾�^����
	/// </summary>
	static void SetFbxFilePathRoot( const hash::CHashStringPtr &path = hash::CHashStringPtr() );
	static const hash::CHashStringPtr &GetFbxFilePathRoot();
	static skyWString* ConcatFbxFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath );
	static skyMString* ConcatFbxFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath );
	static void ClearFbxFilePathRoot();

	/// <summary>
	/// ���[�h�w�t�@�C�����[�g�p�X�ݒ�^�擾�^����
	/// </summary>
	static void SetXFilePathRoot( const hash::CHashStringPtr &path = hash::CHashStringPtr() );
	static const hash::CHashStringPtr &GetXFilePathRoot();
	static skyWString* ConcatXFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath );
	static skyMString* ConcatXFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath );
	static void ClearXFilePathRoot();

	/// <summary>
	/// ���[�h�I�u�W�F�t�@�C�����[�g�p�X�ݒ�^�擾�^����
	/// </summary>
	static void SetObjFilePathRoot( const hash::CHashStringPtr &path = hash::CHashStringPtr() );
	static const hash::CHashStringPtr &GetObjFilePathRoot();
	static skyWString* ConcatObjFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath );
	static skyMString* ConcatObjFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath );
	static void ClearObjFilePathRoot();

	/// <summary>
	/// ���[�h�}�e���A���t�@�C���@�C�����[�g�p�X�ݒ�^�擾�^����
	/// </summary>
	static void SetMtlFilePathRoot( const hash::CHashStringPtr &path = hash::CHashStringPtr() );
	static const hash::CHashStringPtr &GetMtlFilePathRoot();
	static skyWString* ConcatMtlFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath );
	static skyMString* ConcatMtlFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath );
	static void ClearMtlFilePathRoot();

	/// <summary>
	/// ���[�h�e�N�X�`���t�@�C���@�C�����[�g�p�X�ݒ�^�擾�^����
	/// </summary>
	static void SetTextureFilePathRoot( const hash::CHashStringPtr &path = hash::CHashStringPtr() );
	static const hash::CHashStringPtr &GetTextureFilePathRoot();
	static skyWString* ConcatTextureFilePathRoot( skyWString* destBuf , u32 bufSize , const skyWString *pPath );
	static skyMString* ConcatTextureFilePathRoot( skyMString* destBuf , u32 bufSize , const skyMString *pPath );
	static void ClearTextureFilePathRoot();
};

} } }

#define CGraphicFilePathManagerInitialize_() sky::lib::graphic::CGraphicFilePathManager::Initialize()
#define CGraphicFilePathManager_	         sky::lib::graphic::CGraphicFilePathManager
#define CGraphicFilePathManagerDispose_()    sky::lib::graphic::CGraphicFilePathManager::Dispose()
