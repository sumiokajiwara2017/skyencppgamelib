#pragma once

namespace sky { namespace lib { namespace graphic {

/// <summary>
/// レンダリングステップ制御
/// </summary>
class CRenderStepManager : public base::SkyRefObject
{

//友達
friend class CRenderStep; //友達なので、内部のシーングラフの参照とか見れる。

public:

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CRenderStepManager(){};

	//シングルトン機能を付属
	friend class SingletonPtr< CRenderStepManager >;
    static SingletonPtr< CRenderStepManager > SingletonMember;

	/// <summary>
	/// ステップの登録
	/// </summary>
	void AttachStep( RenderStep_Prio prio , CRenderStepPtr &spStep );

	/// <summary>
	/// ステップの取得
	/// </summary>
	CRenderStepPtr Get( const hash::CHashStringPtr &spHash );

//---------------シーン-------------------------▽

	/// <summary>
	/// シーンの追加
	/// </summary>
	void AttachScene      ( dectype prio , const CScenePtr &spScene );
	void AttachFrontScene ( const CScenePtr &spScene );
	void AttachTailScene  ( const CScenePtr &spScene );

	/// <summary>
	/// シーンの削除
	/// </summary>
	void DetachScene( const CScenePtr &spScene );

	/// <summary>
	/// シーンリストの取得
	/// </summary>
	CScenePtrList &GetSceneList();

	/// <summary>
	/// シーンの全破棄（非同期の場合、描画の終わりにこれを呼び、描画用にクローンされたシーングラフを全破棄する）
	/// </summary>
	void DeleteAllScene();

	/// <summary>
	/// レンダリング
	/// </summary>
	void Rendering( const fps::CFpsController &fps );

//---Static

	/// <summary>
	/// インスタンスの作成
	/// </summary>
	static CRenderStepManagerPtr Create()
	{
		
		CRenderStepManagerPtr spRenderStepManager( NEW__( CRenderStepManager , CRenderStepManager() ) );
		return spRenderStepManager;
	}

private:

	/// <summary>
	/// ステップリスト
	/// </summary>
	CRenderStepPtrList			m_kStepList;

	/// <summary>
	/// ステップHash
	/// </summary>
	CRenderStepPtrHash			m_kStepHash;

	/// <summary>
	/// シーンList
	/// </summary>
	CScenePtrList				m_kSceneList;

	/// <summary>
	/// 名前でのシーングラフの取得
	/// </summary>
	skyBool GetSceneGraph_( const hash::CHashStringPtr &spHash , CSceneGraphNodePtr &spNode );

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CRenderStepManager(): m_kStepHash(C_S32(render_SCENEGRAPH_HASH_SIZE) ){};

};

} } }

//インスタンスアクセス文字列
#define CRenderStepManagerCreate_()	sky::lib::graphic::CRenderStepManager::SingletonMember.CreateInstance()
#define CRenderStepManager_			sky::lib::graphic::CRenderStepManager::SingletonMember.GetInstance()
#define CRenderStepManagerDelete_()	sky::lib::graphic::CRenderStepManager::SingletonMember.DeleteInstance()
