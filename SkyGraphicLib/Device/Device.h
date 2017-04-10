#pragma once

//===================================�yCommon�z====================================================>

namespace sky { namespace lib { namespace graphic {

//�����o�ϐ�
struct CDeviceMember;

class CDevice;
typedef CDevice*  CDevicePtr;			//�|�C���^�^��`

/// <summary>
/// �v���b�g�t�H�[�����̃f�o�C�X�N���X
/// </summary>
class CDevice : public base::SkyRefObject
{

public:

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CDevice();

	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ������
	/// </summary>
	void Dispose();

	/// <summary>
	/// ��Ԋm�F
	/// </summary>
	skyBool IsInit			() const;
	skyBool IsCreateDevice	() const;

	/// <summary>
	/// �`�揉����
	/// </summary>
	void BeginRendering();

	/// <summary>
	/// �`�斖����
	/// </summary>
	void EndRendering();

	/// <summary>
	/// �����o�ϐ��̎擾
	/// </summary>
	inline CDeviceMember* GetMember(){ return m_pMember; }

    /// <summary>
    /// �V���O���g�������Ǘ������Q�i�e���v���[�g�Ȃ̂Ńe���v���[�g�̃����o�����ĂȂ����߁A���ɋL�q�����j
    /// </summary>
    static void       CreateInstance();
    static CDevicePtr GetInstance();
    static void       DeleteInstance();

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CDevice();

    /// <summary>
    /// ���g�̎Q��
    /// </summary>
    static CDevicePtr m_pMySelf;

	/// <summary>
	/// �����o�ϐ��Q(pimpl)
	/// </summary>
	CDeviceMember*   m_pMember;

	/// <summary>
	/// ��ԃt���O
	/// </summary>
	skyBool m_IsInit;
};

} } }

//�C���X�^���X�A�N�Z�X������
#define CDeviceCreate_()	sky::lib::graphic::CDevice::CreateInstance()
#define CDevice_			sky::lib::graphic::CDevice::GetInstance()
#define CDeviceDelete_()	sky::lib::graphic::CDevice::DeleteInstance()

//===================================�yDirectX�z===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//�^���`
typedef SmartPointerCom< ID3D11Device >          D3DDevicePtr;
typedef SmartPointerCom< ID3D11DeviceContext >   D3DDeviceInsContextPtr;
typedef SmartPointerCom< IDXGIDevice1 >          DXGIDevice1Ptr;
typedef SmartPointerCom< IDXGIAdapter >          DXGIAdapterPtr;
typedef SmartPointerCom< IDXGIFactory >          DXGIFactoryPtr;

struct CDeviceMember
{
	/// <summary>
	/// �f�o�C�X�̎Q��
	/// </summary>
	D3DDevicePtr            m_spD3DDevice;

	/// <summary>
	/// �f�o�C�X�R���e�L�X�g
	/// </summary>
	D3DDeviceInsContextPtr  m_spD3DDeviceContext;

	/// <summary>
	/// �c�w�f�o�C�X
	/// </summary>
	DXGIDevice1Ptr          m_DxDevice1;

	/// <summary>
	/// �A�_�v�^�[
	/// </summary>
    DXGIAdapterPtr          m_spDXGIAdapter;

	/// <summary>
	/// �t�@�N�g���[
	/// </summary>
    DXGIFactoryPtr          m_spDXGIFactory;

	/// <summary>
	/// �������ł����f�o�C�X�i�[��
	/// </summary>
	D3D_FEATURE_LEVEL       m_FeatureLevel;

	/// <summary>
	/// ���_�V�F�[�_�[�̃o�[�W�������擾
	/// </summary>
	const skyMString*       GetVSVersion();

	/// <summary>
	/// �s�N�Z���V�F�[�_�[�̃o�[�W�������擾
	/// </summary>
	const skyMString*       GetPSVersion();
};

} } }

#endif

//===================================�yOpenGL�z====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

struct CDeviceMember
{
	//OpenGL��Device�Ƃ����T�O�͕K�v�Ȃ��B
};

} } }

#endif
