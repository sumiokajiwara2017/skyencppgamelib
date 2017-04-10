#pragma once

namespace sky { namespace lib { namespace math {

class CEulerAngles;
class CRotationMatrix;
class CQuaternion;

class CMatrix4x3;
typedef CList< u32 , CMatrix4x3 >	    CMatrix4x3List;	                   //リスト定義
typedef CVectorPrimitive< CMatrix4x3 >	CMatrix4x3Array;	               //配列定義
typedef CHash< CMatrix4x3 >	            CMatrix4x3Hash;	                   //名前配列定義

/// <summary>
/// ３Ｄ処理用４Ｘ３行列クラス
/// 
/// 行列の構成
/// 
/// このクラスの目的は、その結果が正しく見えるまで、行列にプラス記号やマイナス記号をつけたり
/// 転置したりしていらいらすることなく、座標変換が行えるようにすることである。
/// もちろん内部表現の仕様は重要である。
/// それは、このファイル内の実装を正しくするためでなく、行列変数への直接アクセスが時々必要になったり、
/// 最適化のためになるからである。このため、こｎ行列の約束事をここに書いておく
/// 
///               | m11 m12 m13 |
///     [ x y z ] | m21 m22 m23 | = [ x' y' z' ]
///               | m31 m32 m33 |
///               | tx  ty  tz  | 
///               
/// 線形代数のルールに厳密に従うと、この乗算は実際には未定義となる
/// これを回避するには、入力ベクトルと出力ベクトルを、４番目の座標１を持つものとして考える
/// また、線形代数のルールにより、技術的に４×３行列の逆行列を求めることはできないので、
/// 一番右端の列に[ 0 0 0 1 ]があるものと仮定する。
/// これを以下に示す。
/// 
///                 | m11 m12 m13 0 |
///     [ x y z 1 ] | m21 m22 m23 0 | = [ x' y' z' 1 ]
///                 | m31 m32 m33 0 |
///                 | tx  ty  tz  1 |
///
/// </summary>
class CMatrix4x3
{

public:

    /// <summary>
    /// プロパティ
    /// </summary>
	union 
	{
		struct 
		{
			dectype	M11, M12, M13, M14;
			dectype	M21, M22, M23, M24;
			dectype	M31, M32, M33, M34;
			dectype	Tx ,  Ty,  Tz,  Tw;
		};
		dectype m[ 4 ][ 4 ];
	};

	/// <summary>
	/// =演算子オーバーライド（要素の代入）
	/// </summary>
	inline CMatrix4x3& operator =( const CMatrix4x3& v );

	/// <summary>
	///+演算子オーバーライド（要素の代入）
	/// </summary>
	inline CMatrix4x3  operator + ( const CMatrix4x3& src ) const;

	/// <summary>
	///-演算子オーバーライド（要素の代入）
	/// </summary>
	inline CMatrix4x3  operator - ( const CMatrix4x3& src ) const;

    /// <summary>
    /// コンストラクタ
    /// </summary>
	inline CMatrix4x3();
	inline CMatrix4x3( const dectype mat[ 16 ] );
	inline CMatrix4x3( dectype m11 , dectype m12 , dectype m13 , dectype m14 ,
                dectype m21 , dectype m22 , dectype m23 , dectype m24 , 
                dectype m31 , dectype m32 , dectype m33 , dectype m34 , 
                dectype m41 , dectype m42 , dectype m43 , dectype m44 );

    /// <summary>
    /// 恒等行列を設定する
    /// </summary>
	inline CMatrix4x3& Identity();

    /// <summary>
    /// 行列の平行移動部分を直接アクセスする
    /// 最後の行を[0,0,0]に設定し線形変換部分（３×３の部分）をそのまま残すことで、
    /// この行列の平行移動部分を取り除きます。
    /// </summary>
	inline CMatrix4x3& ZeroTranslation();

    /// <summary>
    /// 行列の平行移動部分を直接アクセスする
    /// 行列の平行移動部分を指定された値に設定し、３×３の部分は恒等行列にします。（つまり純粋な平行移動行列になる）
    /// </summary>
	inline CMatrix4x3& SetupTranslation( const CBasicVector3 &d );

    /// <summary>
    /// 行列の平行移動部分を上書きする
    /// 行列の平行移動部分を指定された値に設定し、３×３の部分は変更しません。
    /// </summary>
	inline CMatrix4x3& SetTranslation( const CBasicVector3 &d );

    /// <summary>
    /// 行列の平行移動部分に加算する
    /// 行列の平行移動部分を指定された値に設定し、３×３の部分は変更しません。
    /// </summary>
	inline CMatrix4x3& AddTranslation( const CBasicVector3 &d );

    /// <summary>
    /// ３軸の移動情報を取得する
    /// </summary>
	inline dectype GetTranslation( eAxis axis ) const;

    /// <summary>
    /// 親の基準座標内のローカルの基準座標系の位置と方向を指定し、
    /// ローカル→親への座標変換を実行する行列をセットアップする
    /// 
    /// これの最もよくある使い方は、オブジェクト行列→ワールド行列の作成である
    /// 例として、この場合の座標変換は簡単である
    /// 最初に、オブジェクト空間から慣性空間へ回転し、その後、ワールド空間へ平行移動する
    /// 
    /// 方向は、オイラー角がRotationMatrixのいずれかで指定できる。
    /// 
    /// </summary>
	CMatrix4x3& SetupLocalToParent( const CBasicVector3 &pos , const CEulerAngles &orient );
	CMatrix4x3& SetupLocalToParent( const CBasicVector3 &pos , const CRotationMatrix &orient );

    /// <summary>
    /// 親の基準座標系内のローカルの基準座標系の位置と方向を指定し、
    /// 親→ローカルの座標変換を実行する行列をセットアップする
    /// 
    /// これの最もよくある使い方は、ワールド行列→オブジェクト行列の作成である。
    /// この座標変換を実行するには、通常、最初にワールドから慣性空間へ座標変換し、
    /// その後、慣性空間からオブジェクト空間へ回転する
    /// しかしながら、ここの４×３行列は、常に最後に平行移動を行う
    /// このため、２つの行列ＴとＲの作成を考え、Ｍ＝ＴＲという形で連結する
    /// 
    /// 方向は、オイラー角かRotationMatrixのいずれかで指定できる
    /// </summary>
	CMatrix4x3& SetupParentToLocal( const CBasicVector3 &pos , const CEulerAngles &orient );
	CMatrix4x3& SetupParentToLocal( const CBasicVector3 &pos , const CRotationMatrix &orient );

    /// <summary>
    /// 基本軸の周りの回転を実行する行列をセットアップする
    /// 
    /// 回転の軸は、以下のように１をベースにしたインデックスを用いて指定される
    /// 
    /// eAxis_x→Ｘ軸に関する回転
    /// eAxis_y→Ｙ軸に関する回転
    /// eAxis_z→Ｚ軸に関する回転
    /// 
    /// thetaは回転の量（ラジアン）、左手のルールで正の回転を定義する
    /// 
    /// 平行移動部分はリセットされる
    /// 
    /// </summary>
	inline CMatrix4x3& SetupRotate( eAxis axis , dectype theta );

    /// <summary>
    /// 任意の軸の周りの回転を実行する行列をセットアップする
    /// 
	/// 回転の掛け合わせ順分種類がある。
    /// 
    /// 平行移動部分はリセットされる
    /// 
    /// </summary>
	inline CMatrix4x3& SetupRotateXYZ( const CBasicVector3 &vec );
	inline CMatrix4x3& SetupRotateXZY( const CBasicVector3 &vec );
	inline CMatrix4x3& SetupRotateYXZ( const CBasicVector3 &vec );
	inline CMatrix4x3& SetupRotateYZX( const CBasicVector3 &vec );
	inline CMatrix4x3& SetupRotateZXY( const CBasicVector3 &vec );
	inline CMatrix4x3& SetupRotateZYX( const CBasicVector3 &vec );

    /// <summary>
    /// 任意の軸の周りの回転を実行する行列をセットアップする
    /// 回転の軸は、原点を通らなければならない
    /// 
    /// axisは回転の軸を定義し、単位ベクトルでなければならい
    /// 
    /// thetaは回転の量（ラジアン）、左手のルールで正の回転を定義する
    /// 
    /// 平行移動部分はリセットされる
    /// 
    /// </summary>
	inline CMatrix4x3& SetupRotate( const CBasicVector3 &axis , dectype theta );

    /// <summary>
    /// 回転行列を元にセットアップする
    /// </summary>
	CMatrix4x3& SetupRotate( const CRotationMatrix &rotmat );

    /// <summary>
    /// クオータニオンを元にセットアップする
    /// </summary>
	CMatrix4x3& SetupRotate( const CQuaternion &q );

    /// <summary>
    /// 回転行列を作成する
    /// </summary>
	const CRotationMatrix &CreateRotateMatrix( CRotationMatrix &r ) const;

    /// <summary>
    /// ３軸の向きを取得する(恒等行列の場合Ｘ軸は( 1.0f , 0.0f , 0.0f ) Ｙ軸は( 0.0f , 1.0f , 0.0f ) Ｚ軸は( 0.0f , 0.0f , 1.0f )
	/// isNormalizeExec:正規化するかしないか
    /// </summary>
	inline void    GetDirection( eAxis axis , CBasicVector3 &dest , skyBool isNormalizeExec = skyTrue ) const;

    /// <summary>
    /// 
    /// 各軸でスケーリングを実行する行列をセットアップする
    /// kだけ均等にスケーリングする場合は、CBasicVector3(k,k,k)の形式のベクトルを用いる
    /// 
    /// 平行移動部分はリセットされる
    /// 
    /// </summary>
	inline CMatrix4x3& SetupScale( const CBasicVector3 &sc );

    /// <summary>
    /// 
    /// 各軸でスケーリングを実行する行列をセットアップする
    /// kだけ均等にスケーリングする場合は、CBasicVector3(k,k,k)の形式のベクトルを用いる
    /// 
    /// </summary>
	inline CMatrix4x3& SetScale( const CBasicVector3 &sc );

    /// <summary>
    /// ３軸の拡大縮小情報を取得する
    /// </summary>
	inline dectype       GetScale( eAxis axis ) const;
	inline CBasicVector3 GetScale() const;

    /// <summary>
    /// 
    /// 任意の軸に沿ったスケーリングを実行する行列をセットアップする
    /// 
    /// axisは単位ベクトルで指定される
    /// 
    /// 平行移動部分はリセットされる
    /// 
    /// </summary>
	inline CMatrix4x3& SetupScaleAlongAxis( const CBasicVector3 &axis , dectype k );

    /// <summary>
    /// せん断を実行する行列をセットアップする
    /// 
    /// せん断の種類は、１をベースにした軸（axis）のインデックスで指定される
    /// この行列で点を座標変換した場合の効果は、以下の擬似コードで示したとおりである。
    /// 
    /// axis == eAxis_x → y += s*x , z += t*x
    /// axis == eAxis_y → x += s*y, z += t*y
    /// axis == eAxis_z → x += s*z, y += t*z
    /// 
    /// 平行移動部分はリセットされる
    /// 
    /// 
    /// </summary>
	inline CMatrix4x3& SetupShear( eAxis axis , dectype s , dectype t );

    /// <summary>
    /// 原点を通る面上への投影を実行する行列をセットアップする
    /// この面は、単位ベクトルｎに垂直である
    /// </summary>
	inline CMatrix4x3& SetupProject( const CBasicVector3 &n );
	inline CMatrix4x3& SetupProject( const CBasicPlane &p , const CBasicVector3 &lightPos ); //lightPosは照らしたい物体からのオフセット値としてみます。

    /// <summary>
    /// 基本軸に平行な面に関するリフレクションを実行する行列をセットアップする
    /// 
    /// axisは１をベースにしたインデックスで、投影される面を指定する
    /// 
    /// eAxis_x → 面ｘ＝ｋに関するリフレクション
    /// eAxis_y → 面ｙ＝ｋに関するリフレクション
    /// eAxis_z → 面ｚ＝ｋに関するリフレクション
    /// 
    /// 平行移動は適切に設定される（平行移動は、ｋ ！＝ ０の場合にだけ起こるからである）
    /// 
    /// </summary>
	inline CMatrix4x3& SetupReflect( eAxis axis , dectype k = 0.0f );

    /// <summary>
    /// 
    /// 原点を通る任意の平面に関するリフレクションを実行する行列をセットアップする
    /// 単位ベクトルｎは、この面に垂直である
    /// 
    /// 平行移動部分はリセットされる
    /// 
    /// </summary>
	inline CMatrix4x3& SetupReflect( const CBasicVector3 &n );

    /// <summary>
    /// 
    /// ワールド座標テクスチャ座標変換行列作成
    /// 
    /// </summary>
	inline CMatrix4x3& SetupPosToUv();

    /// <summary>
    /// View行列を作成する
    /// </summary>
	inline CMatrix4x3& SetupViewLH( const CBasicVector3 &eye , const CBasicVector3 &tag , const CBasicVector3 &up );
	inline CMatrix4x3& SetupViewRH( const CBasicVector3 &eye , const CBasicVector3 &tag , const CBasicVector3 &up );

    /// <summary>
    /// Projection行列を作成する
    /// </summary>
	inline CMatrix4x3& SetupProjectionLH( dectype fovY , dectype width , dectype height , dectype zn , dectype zf );
	inline CMatrix4x3& SetupProjectionRH( dectype fovY , dectype width , dectype height , dectype zn , dectype zf );
	inline CMatrix4x3& SetupProjectionLH( dectype fovY , dectype aspect , dectype zn , dectype zf );
	inline CMatrix4x3& SetupProjectionRH( dectype fovY , dectype aspect , dectype zn , dectype zf );
	inline CMatrix4x3& SetupProjectionOffsetCenterLH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf ); //ビューポートの中心を変える時など 
	inline CMatrix4x3& SetupProjectionOffsetCenterRH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf ); //ビューポートの中心を変える時など 

    /// <summary>
    /// 正射影行列を作成する
    /// </summary>
	inline CMatrix4x3& SetupOrthoMatrixLH( dectype width , dectype height , dectype zn , dectype zf );
	inline CMatrix4x3& SetupOrthoMatrixRH( dectype width , dectype height , dectype zn , dectype zf );
	inline CMatrix4x3& SetupOrthoMatrixOffsetCenterLH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf );//ビューポートの中心を変える時など
	inline CMatrix4x3& SetupOrthoMatrixOffsetCenterRH( dectype left , dectype right , dectype bottom , dectype top , dectype zn , dectype zf );//ビューポートの中心を変える時など
	
    /// <summary>
    /// テクスチャ座標変換行列を作成する
    /// </summary>
	inline CMatrix4x3& SetupTexturePositionMatrix();

    /// <summary>
    /// 行をベクトルとして取得
    /// </summary>
	inline CBasicVector3 &GetCol0( CBasicVector3 &dest ) const;
	inline CBasicVector3 &GetCol1( CBasicVector3 &dest ) const;
	inline CBasicVector3 &GetCol2( CBasicVector3 &dest ) const;
	inline CBasicVector3 &GetCol3( CBasicVector3 &dest ) const;

    /// <summary>
    /// 行をベクトルとして設定
    /// </summary>
	inline CMatrix4x3& SetCol0( CBasicVector3 &src , dectype w );
	inline CMatrix4x3& SetCol1( CBasicVector3 &src , dectype w );
	inline CMatrix4x3& SetCol2( CBasicVector3 &src , dectype w );
	inline CMatrix4x3& SetCol3( CBasicVector3 &src , dectype w );

    /// <summary>
    /// 配列にコピーする
    /// </summary>
	template< class T >
	void CopyArray( T *dest ) const;

    /// <summary>
    /// プリントする
    /// </summary>
	void DebugPrint() const;

    /// <summary>
    /// f32[16]の配列を単位行列に初期化する
    /// </summary>
	static void IdentityArray( f32 *dest );
};

/// <summary>
/// ３次元ZEROベクトル
/// </summary>
static const CMatrix4x3 CMatrix4x3_Identity( 1.0f , 0.0f , 0.0f , 0.0f ,
	                                         0.0f , 1.0f , 0.0f , 0.0f ,
	                                         0.0f , 0.0f , 1.0f , 0.0f ,
	                                         0.0f , 0.0f , 0.0f , 1.0f );

} } }

/// <summary>
/// *演算子のオーバーライド
/// 
/// 点を座標変換する
/// これは、紙の上の線形代数記法に似せてベクトルクラスを使えるようにする
/// </summary>
inline sky::lib::math::CBasicVector3	operator*( const sky::lib::math::CBasicVector3 &p , const sky::lib::math::CMatrix4x3 &m );
inline sky::lib::math::CMatrix4x3		operator*( const sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CMatrix4x3 &m2);
inline sky::lib::math::CBasicVector3	&operator*=( sky::lib::math::CBasicVector3 &p , const sky::lib::math::CMatrix4x3 &m );
inline sky::lib::math::CMatrix4x3      &operator*=( sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CMatrix4x3 &m2 );

sky::lib::math::CMatrix4x3		operator*( const sky::lib::math::CRotationMatrix &m1 , const sky::lib::math::CMatrix4x3 &m2 );
sky::lib::math::CMatrix4x3		operator*( const sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CRotationMatrix &m2 );
sky::lib::math::CBasicVector3	operator*( const sky::lib::math::CBasicVector3 &p , const sky::lib::math::CRotationMatrix &m );
sky::lib::math::CBasicVector3	&operator*=( sky::lib::math::CBasicVector3 &p , const sky::lib::math::CRotationMatrix &m );
sky::lib::math::CMatrix4x3		&operator*=( sky::lib::math::CMatrix4x3 &m1 , const sky::lib::math::CRotationMatrix &m2 );

/// <summary>
/// 行列の４×４部分の行列式かけ算する（上記*演算子のオーバーライドでは４×３の計算しかしていない。その場合だとビュー行列やプロジェクション行列の計算には使えない）
/// </summary>
inline sky::lib::math::CMatrix4x3 MulMatrix4x4( const sky::lib::math::CMatrix4x3 &m1, const sky::lib::math::CMatrix4x3 &m2 );      //行列のM1 M2 M3 行のw値の計算をはし追っています。カメラの計算とかはこれで十分
inline sky::lib::math::CMatrix4x3 MulMatrix4x4Full( const sky::lib::math::CMatrix4x3 &m1, const sky::lib::math::CMatrix4x3 &m2 );  //全ての要素をちゃんと掛け合わせています。スキンメッシュアニメーションの場合これじゃないとダメ。
inline sky::lib::math::CBasicVector3 MulMatrix4x4( const sky::lib::math::CBasicVector3 &v, const sky::lib::math::CMatrix4x3 &m );   //全ての要素をちゃんと掛け合わせています。スキンメッシュアニメーションの場合これじゃないとダメ。
inline sky::lib::math::CBasicVector4 MulMatrix4x4( const sky::lib::math::CBasicVector4 &v, const sky::lib::math::CMatrix4x3 &m );   //全ての要素をちゃんと掛け合わせています。スキンメッシュアニメーションの場合これじゃないとダメ。

/// <summary>
/// ４ｘ３行列の並行移動部分のみ合成する
/// </summary>
inline sky::lib::math::CMatrix4x3 MulMatrix4x4TransOnly( const sky::lib::math::CMatrix4x3 &m1, const sky::lib::math::CMatrix4x3 &m2 );

/// <summary>
/// 行列の３×３部分の行列式を計算する
/// </summary>
inline dectype	Determinant(const sky::lib::math::CMatrix4x3 &m);
inline dectype	Determinant4x4(const sky::lib::math::CMatrix4x3 &m);

/// <summary>
/// 逆行列を計算する
/// determinantメソッドで除算された古典的な随伴行列を用いる
/// </summary>
inline sky::lib::math::CMatrix4x3 Invert(const sky::lib::math::CMatrix4x3 &m);
inline sky::lib::math::CMatrix4x3 Invert4x4(const sky::lib::math::CMatrix4x3 &m);

/// <summary>
/// 平行移動用ベクトルクラスを返す
/// </summary>
inline sky::lib::math::CBasicVector3	GetTranslationVector(const sky::lib::math::CMatrix4x3 &m);

/// <summary>
/// 親→ローカル座標変換行列（ワールド行列→オブジェクト行列など）を与え
/// オブジェクトの位置を取り出す
/// 
/// この行列は、剛体の座標変換を表しているという前提がある
/// スケーリング、スキュー、ミラーを含まない
///
/// </summary>
inline sky::lib::math::CBasicVector3	GetPositionFromParentToLocalMatrix(const sky::lib::math::CMatrix4x3 &m);

/// <summary>
/// ローカル→親の座標変換行列（例えば、オブジェクト行列→ワールド行列）を与え、
/// オブジェクトの位置を取り出す
/// </summary>
inline sky::lib::math::CBasicVector3	GetPositionFromLocalToParentMatrix(const sky::lib::math::CMatrix4x3 &m);

/// <summary>
/// 行列の補間
/// </summary>
sky::lib::math::CMatrix4x3 Lerp(sky::lib::math::CMatrix4x3 a,sky::lib::math::CMatrix4x3 b,f32 t);

/// <summary>
/// 行列からクオータニオンへ変換
/// </summary>
skyBool TransformRotMatToQuaternion(
    f32 &qx , f32 &qy , f32 &qz , f32 &qw ,
    f32 m11 , f32 m12 , f32 m13 ,
    f32 m21 , f32 m22 , f32 m23 ,
    f32 m31 , f32 m32 , f32 m33
);
skyBool TransformRotMatToQuaternion( sky::lib::math::CQuaternion &dest , const sky::lib::math::CMatrix4x3 &src );

#include "Matrix4x3.inl"