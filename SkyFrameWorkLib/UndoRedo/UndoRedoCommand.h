#pragma once

namespace sky { namespace lib { namespace framework {

/// <summary>
/// UndoRedoコマンド
/// このクラスを継承して作成されるクラスは必ずSkyTypeDefRTTI定義を行い、スマートポインタで扱ってください。
/// </summary>
class CUndoRedoCommand : public base::SkyRefObject
{
    SkyTypeDefRTTI;

public:

    /// <summary>
    /// デストラクタ
    /// </summary>
    virtual ~CUndoRedoCommand(){};

    /// <summary>
    /// 実行
    /// </summary>
    virtual void Execute() = 0;

    /// <summary>
    /// Undo
    /// </summary>
    virtual void Undo() = 0;

    /// <summary>
    /// Redo
    /// </summary>
    virtual void Redo() = 0;

protected:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    CUndoRedoCommand(){};

};

} } }
