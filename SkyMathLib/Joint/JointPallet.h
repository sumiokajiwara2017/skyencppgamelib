#pragma once

namespace sky { namespace lib { namespace math {

#define BONE_PALLET_DEFAULT_SIZE ( 64 )

class CJointPallet;
typedef SmartPointer< CJointPallet >	CJointPalletPtr;					//スマートポインタ版定義

/// <summary>
/// ジョイント配列クラス
/// ジョイントを配列で管理するのは頂点ウェイトのため『だけ』。モーションの実際の動きとは関係がない。
/// このクラスは数学ライブラリに有りながら描画ライブラリでしか使用しないという特殊なクラス。
/// </summary>
class CJointPallet : public base::SkyRefObject
{
	friend class CMotionController;

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	inline virtual ~CJointPallet(){};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	inline CJointPallet();

	/// <summary>
	/// ジョイント数取得
	/// </summary>
	inline u32 GetJointNum();

	/// <summary>
	/// クリアする
	/// </summary>
	inline void Clear();

	/// <summary>
	/// スキン変換行列配列の先頭ポインタを返す（そのままシェーダーに渡す）
	/// </summary>
	inline math::CMatrix4x3Array &GetSkinTransformMatrixArray();

	/// <summary>
	/// ジョイントを追加／設定／取得する
	/// </summary>
	inline void  AddJoint( const CJointPtr &spJoint );
	inline void  SetJoint( u32 no , const CJointPtr &spJoint );
	inline const CJointPtr &GetJoint( u32 no );
	inline CJointPtr GetJoint( const hash::CHashStringPtr &name );

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CJointPalletPtr Create()
	{
		CJointPalletPtr spJoint( NEW__( CJointPallet , CJointPallet() ) );
		return spJoint;
	}

private:

	/// <summary>
	/// ジョイント情報
	/// </summary>
	struct CJointData_
	{
		/// <summary>
		/// ジョイント情報
		/// </summary>
		CJointPtr         m_spJoint;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		CJointData_(){}

		/// <summary>
		/// 代入演算子オーバーライド
		/// </summary>
		virtual CJointData_& operator =( const CJointData_& src )
		{
			m_spJoint         = src.m_spJoint;
			return *this;
		}
	};
	typedef CVector< CJointData_ > CJointDataPallet;
	typedef CHash< CJointData_ >   CJointDataHash;

	/// <summary>
	/// ジョイント名前Hash
	/// </summary>
	CJointDataHash m_JointDataHash;

	/// <summary>
	/// ジョイント配列
	/// </summary>
	CJointDataPallet m_JointPallet;

	/// <summary>
	/// スキン変換行列配列(計算バッファ。モーションでもIKでも正しく描画されるためにここを更新してください）
	/// </summary>
	math::CMatrix4x3Array m_SkinTransforms;
};

} } }


#define CJointPalletCreate_()	  sky::lib::math::CJointPallet::Create()

#include "JointPallet.inl"