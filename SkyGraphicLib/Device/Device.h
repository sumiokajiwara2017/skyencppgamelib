#pragma once

//===================================【Common】====================================================>

namespace sky { namespace lib { namespace graphic {

//メンバ変数
struct CDeviceMember;

class CDevice;
typedef CDevice*  CDevicePtr;			//ポインタ型定義

/// <summary>
/// プラットフォーム毎のデバイスクラス
/// </summary>
class CDevice : public base::SkyRefObject
{

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CDevice();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 末期化
	/// </summary>
	void Dispose();

	/// <summary>
	/// 状態確認
	/// </summary>
	skyBool IsInit			() const;
	skyBool IsCreateDevice	() const;

	/// <summary>
	/// 描画初期化
	/// </summary>
	void BeginRendering();

	/// <summary>
	/// 描画末期化
	/// </summary>
	void EndRendering();

	/// <summary>
	/// メンバ変数の取得
	/// </summary>
	inline CDeviceMember* GetMember(){ return m_pMember; }

    /// <summary>
    /// シングルトン生成管理処理群（テンプレートなのでテンプレートのメンバが持てないため、直に記述した）
    /// </summary>
    static void       CreateInstance();
    static CDevicePtr GetInstance();
    static void       DeleteInstance();

private:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CDevice();

    /// <summary>
    /// 自身の参照
    /// </summary>
    static CDevicePtr m_pMySelf;

	/// <summary>
	/// メンバ変数群(pimpl)
	/// </summary>
	CDeviceMember*   m_pMember;

	/// <summary>
	/// 状態フラグ
	/// </summary>
	skyBool m_IsInit;
};

} } }

//インスタンスアクセス文字列
#define CDeviceCreate_()	sky::lib::graphic::CDevice::CreateInstance()
#define CDevice_			sky::lib::graphic::CDevice::GetInstance()
#define CDeviceDelete_()	sky::lib::graphic::CDevice::DeleteInstance()

//===================================【DirectX】===================================================>

#ifdef SKYLIB_RENDER_DIRECTX11

namespace sky { namespace lib { namespace graphic {

//型整形
typedef SmartPointerCom< ID3D11Device >          D3DDevicePtr;
typedef SmartPointerCom< ID3D11DeviceContext >   D3DDeviceInsContextPtr;
typedef SmartPointerCom< IDXGIDevice1 >          DXGIDevice1Ptr;
typedef SmartPointerCom< IDXGIAdapter >          DXGIAdapterPtr;
typedef SmartPointerCom< IDXGIFactory >          DXGIFactoryPtr;

struct CDeviceMember
{
	/// <summary>
	/// デバイスの参照
	/// </summary>
	D3DDevicePtr            m_spD3DDevice;

	/// <summary>
	/// デバイスコンテキスト
	/// </summary>
	D3DDeviceInsContextPtr  m_spD3DDeviceContext;

	/// <summary>
	/// ＤＸデバイス
	/// </summary>
	DXGIDevice1Ptr          m_DxDevice1;

	/// <summary>
	/// アダプター
	/// </summary>
    DXGIAdapterPtr          m_spDXGIAdapter;

	/// <summary>
	/// ファクトリー
	/// </summary>
    DXGIFactoryPtr          m_spDXGIFactory;

	/// <summary>
	/// 初期化できたデバイス格納先
	/// </summary>
	D3D_FEATURE_LEVEL       m_FeatureLevel;

	/// <summary>
	/// 頂点シェーダーのバージョンを取得
	/// </summary>
	const skyMString*       GetVSVersion();

	/// <summary>
	/// ピクセルシェーダーのバージョンを取得
	/// </summary>
	const skyMString*       GetPSVersion();
};

} } }

#endif

//===================================【OpenGL】====================================================>

#ifdef SKYLIB_RENDER_OPENGL4

namespace sky { namespace lib { namespace graphic {

struct CDeviceMember
{
	//OpenGLにDeviceという概念は必要なし。
};

} } }

#endif
