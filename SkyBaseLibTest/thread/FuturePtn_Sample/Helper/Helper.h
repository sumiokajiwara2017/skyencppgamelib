/// <summary>
/// �z�X�g�N���X
/// </summary>
class Helper : public sky::lib::thread::ThreadWin32
{

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Helper( const skyWString *pStr , DataSmPtr& data );

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Helper( void );

	/// <summary>
	/// ���s�֐�
	/// </summary>
	virtual void ThreadMain( void );

	/// <summary>
	/// �������
	/// </summary>
	void Slowly( void );

protected:

	DataSmPtr				m_Data;
	sky::lib::WString		m_Str;

};
