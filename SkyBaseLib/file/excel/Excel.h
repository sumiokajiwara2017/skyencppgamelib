#pragma once

#ifdef SKYLIB_COMPILER_VC10

//tlbのファイル名だけです。tlbまでのパスはincludeディレクトリの指定で行ってください。
#ifdef _DEBUG
#import "../../../../../skycslib/dlls/Debug/skycslib.tlb" raw_interfaces_only //COMクラスを呼び出す際のヘッダーのようなもの。これはnamespaceの外に居ないとnamespaceの影響をウケル 
#else
#import "../../../../../skycslib/dlls/Release/skycslib.tlb" raw_interfaces_only //COMクラスを呼び出す際のヘッダーのようなもの。これはnamespaceの外に居ないとnamespaceの影響をウケル 
#endif

namespace sky { namespace lib { namespace file {

//-------------------------------< インターフェース >----------------------------------------//

class IExcel;
typedef SmartPointer2< IExcel >	IExcelPtr;

/// <summary>
/// Excel操作クラス
/// </summary>
class IExcel
{

public:

    /// <summary>
    /// プロパティ
    /// <summary>
    virtual s32 getRow() = 0;
    virtual void setRow( s32 row ) = 0;
    virtual s32 getCol() = 0;
    virtual void setCol( s32 col ) = 0;

    /// <summary>
    /// ワークブックを開く
    /// <summary>
    virtual void Open( skyString *filePath ) = 0;

    /// <summary>
    /// 閉じる
    /// <summary>
    virtual void Close() = 0;

    /// <summary>
    /// シート名の存在チェック
    /// <summary>
    virtual skyBool IsSheet( skyString *sheetName ) = 0;

    /// <summary>
    /// 文字列でシート選択
    /// <summary>
    virtual void SelSheet( skyString *sheetName ) = 0;

    /// <summary>
    /// シート選択
    /// <summary>
    virtual void SelSheet(s32 sheetNo ) = 0;

    /// <summary>
    /// セル選択
    /// <summary>
    virtual void SelCell(s32 rowNo, s32 colNo) = 0;

    /// <summary>
    /// セルに入っている文字列が指定の文字列と同じかどうか判定する
    /// <summary>
    virtual skyBool EqualsCell( skyString *key) = 0;

    /// <summary>
    /// セルに入っている文字列に指定の文字が入っているかチェック
    /// <summary>
    virtual skyBool IndexOfCell( skyString *key) = 0;

    /// <summary>
    /// セルにデータが存在するか判定する
    /// <summary>
    virtual skyBool IsCellEmpty() = 0;

    /// <summary>
    /// 列選択
    /// <summary>
    virtual void NextCol() = 0;

    /// <summary>
    /// 行選択
    /// <summary>
    virtual void NextRow() = 0;

    /// <summary>
    /// 値を文字列で取得
    /// <summary>
    virtual const skyString *GetValue_String() = 0;

    /// <summary>
    /// 値を文字列で取得
    /// <summary>
    virtual s32 GetValue_Int() = 0;

    /// <summary>
    /// 値を文字列で取得
    /// <summary>
    virtual void SetValue_String( skyString *value) = 0;

    /// <summary>
    /// 値を文字列で取得
    /// <summary>
    virtual void SetValue_Int(s32 value) = 0;

    /// <summary>
    /// 保存
    /// <summary>
    virtual void Save() = 0;

    /// <summary>
    /// 別名保存
    /// <summary>
    virtual void SaveAs( skyString *filePath ) = 0;

	/// <summary>
	/// インスタンスを返す
	/// </summary>
	static IExcelPtr						CreateInstance	();
};

//-------------------------------------< 実装 >---------------------------------------------//

class CExcelWin32 : public IExcel
{

//インターフェースのみアクセス可能
friend class IExcel;

public:

	/// <summary>
	///  インタフェース純粋仮想関数の実装（説明はインタフェースの定義を見てください）
	/// </summary>
    s32 getRow			();
    void setRow			( s32 row );
    s32 getCol			();
    void setCol			( s32 col );
    void Open			( skyString *filePath);
    void Close			();
    skyBool IsSheet		( skyString *sheetName);
    void SelSheet		( skyString *sheetName);
    void SelSheet		(s32 sheetNo);
    void SelCell		(s32 rowNo, s32 colNo);
    skyBool EqualsCell	( skyString *key);
    skyBool IndexOfCell	( skyString *key);
    skyBool IsCellEmpty	();
    void NextCol		();
    void NextRow		();
    const skyString *GetValue_String();
    s32 GetValue_Int();
    void SetValue_String( skyString *value);
    void SetValue_Int(s32 value);
    void Save();
    void SaveAs( skyString *filePath );

private:

	skycslib::IExcelPtr	m_pComExcel;

	//各機能封印
	CExcelWin32			();									//コンストラクタ
	CExcelWin32			( const CExcelWin32& ){};					//コピーコンストラクタ
	virtual ~CExcelWin32	();								//デストラクタ
	CExcelWin32 &operator=	(const CExcelWin32& ){ return *this; };	//代入演算子オーバーロード

};

} } }

#endif
