#pragma once

namespace sky { namespace lib { namespace math {

/// <summary>
/// 拡張ＩＤ最大数
/// </summary>
const u32 COLLISION_EXTRAID_NUM = 8;
const s32 COLLISION_KIND_ID_ALL = -1;

/// <summary>
/// コリジョンデータ
/// </summary>
class CollisionData : public base::SkyRefObject
{

friend class CollisionManager;

public:

	/// <summary>
	/// コンストラクタ
	/// 衝突判定実行判定デリゲートを指定しない（デフォルト引数）場合はisStopSameKindCollisionの判定外のすべてのコリジョンデータとの衝突判定を行います
	/// kindId = -1 はすべての種別という意味で予約されているのでユーザーは設定しないこと。
	/// </summary>
	CollisionData();
	CollisionData( s32 kindId , skyBool isStopSameKindCollision , const CGeometryData* pGeometry , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate = ICollisionCBDelegateObjectPtr() , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject = IExecDetectionDelegateObjectPtr() );
	CollisionData( s32 kindId , skyBool isStopSameKindCollision , const IGeometoryGetDelegateObjectPtr &spGeometoryGetDelegate , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate = ICollisionCBDelegateObjectPtr() , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject = IExecDetectionDelegateObjectPtr() );

	/// <summary>
	/// 初期化
	/// 衝突判定実行判定デリゲートを指定しない（デフォルト引数）場合はisStopSameKindCollisionの判定外のすべてのコリジョンデータとの衝突判定を行います
	/// </summary>
	void Initialize( s32 kindId , skyBool isStopSameKindCollision , const CGeometryData* pGeometry , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate = ICollisionCBDelegateObjectPtr() , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject = IExecDetectionDelegateObjectPtr() );
	void Initialize( s32 kindId , skyBool isStopSameKindCollision , const IGeometoryGetDelegateObjectPtr &spGeometoryGetDelegate , const ICollisionCBDelegateObjectPtr &spCollisionCBDelegate = ICollisionCBDelegateObjectPtr() , const IExecDetectionDelegateObjectPtr &spExecDetectionDelegateObject = IExecDetectionDelegateObjectPtr() );

	/// <summary>
	/// コリジョン情報取得デリゲート設定
	/// 設定しなおすことでコリジョンを途中で変化させることも出来る
	/// </summary>
	void SetGeometoryGetDelegate( const IGeometoryGetDelegateObjectPtr &spDelegate );

	/// <summary>
	/// 衝突判定実行判定デリゲート設定
	/// 設定しなおすことで衝突判定を行う組み合わせを変化させることができる
	/// </summary>
	void SetExecDetectionDelegate( const IExecDetectionDelegateObjectPtr &spDelegate );

	/// <summary>
	/// コリジョン発生コールバックデリゲート設定
	/// 設定しなおすことでコリジョン発生の通知先変化させることも出来る
	/// </summary>
	void SetCollisionCBDelegate( const ICollisionCBDelegateObjectPtr &spDelegate );

	/// <summary>
	/// コリジョン判定停止設
	/// </summary>
	void SetStop( skyBool isStop );

	/// <summary>
	/// 同じ属性同士の衝突判定停止設定
	/// </summary>
	void SetStopSameKindCollision( skyBool isStop );

	/// <summary>
	/// 種別ＩＤの取得／設定
	/// </summary>
	s32  GetKindId() const;
	void SetKindId( s32 id );

	/// <summary>
	/// 拡張ＩＤ取得／設定
	/// </summary>
	s32  GetExtraId( u32 index ) const;
	void SetExtraId( u32 index , s32 id );

	/// <summary>
	/// 拡張データ取得／設定
	/// </summary>
	const CommonSmartPointer &GetExtraData() const;
	void  SetExtraData( const CommonSmartPointer &spExtraData );
	
	/// <summary>
	/// タグビットの取得、クリア
	/// </summary>
	u32     GetTagBit();
	void    TagBitOn( u32 tagBit );
	void    TagBitOff( u32 tagBit );
	skyBool IsTagBitOn( u32 tagBit );
	void    ClearTagBit();
	
	/// <summary>
	/// 衝突応答データを受け取る
	/// </summary>
	skyBool IsHitResultGet();
	void    SetHitResultSet( skyBool isHitResult );

	/// <summary>
	/// ジオメトリ取得
	/// </summary>
	const CGeometryData* GetGeometry();

private:

	/// <summary>
	/// タグ（タグはビット値として扱われるので、代入時はビット演算をする）
	/// </summary>
	u32 m_TagBit;

	/// <summary>
	/// コリジョン判定停止フラグ
	/// このフラグがＯＮの場合は衝突判定は行われない。カメラの視野角に入っていない等の理由で描画剃る必要がない場合は
	/// このフラグを立てておくこと。
	/// </summary>
	skyBool m_IsStop;

	/// <summary>
	/// 同種別ＩＤ衝突判定停止フラグ
	/// </summary>
	skyBool m_IsStopSameKindCollision;

	/// <summary>
	/// 種別ＩＤ（プレイヤー・敵・地形・ギミック等の種別ＩＤ）
	/// </summary>
	s32 m_KindID;

	/// <summary>
	/// 幾何形状データポインタ
	/// </summary>
	const CGeometryData* m_pGeometry;

	/// <summary>
	/// コリジョン情報取得デリゲート（幾何形状データポインタが存在しない場合このデリゲートを呼ぶ）
	/// CollisionManagerはこのデリゲートをコールすることでCGeometryData*を受け取り衝突判定を行う
	/// </summary>
	IGeometoryGetDelegateObjectPtr m_spGeometoryGetDelegate;

	/// <summary>
	/// 衝突判定実行判定デリゲート
	/// CollisionManagerはこのデリゲートの戻り値からで実際に衝突判定を実行するかどうか判定する
	/// </summary>
	IExecDetectionDelegateObjectPtr m_spExecDetectionDelegate;

	/// <summary>
	/// コリジョン発生コールバックデリゲート
	/// CollisionManagerはこのデリゲートをコールすることで衝突があったことを通知する
	/// </summary>
	ICollisionCBDelegateObjectPtr m_spCollisionCBDelegate;

	/// <summary>
	/// 拡張ＩＤ(アプリケーション毎に好きなデータを格納できる）
	/// </summary>
	s32 m_ExtraId[ COLLISION_EXTRAID_NUM ];

	/// <summary>
	/// 拡張データ(SkyRefObjectを継承しているデータなら何でも入るポインタ。アプリケーション毎に好きなデータを格納できる）
	/// </summary>
	CommonSmartPointer m_spExtraData;

	/// <summary>
	/// 削除フラグ
	/// </summary>
	skyBool m_IsDelete;

	/// <summary>
	/// ヒット結果データを受け取るかフラグ
	/// </summary>
	skyBool m_IsHitResultGet;

};

} } }
