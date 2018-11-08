
#ifdef __cplusplus

#define WMX_EXCEPTION(desc)		WmxException(desc, _T(__FILE__), __LINE__)

// WMX例外クラス
class WmxException {

public:

	// 定数
	enum {
		MAX_MSG_LEN = 1024,		// メッセージの最大文字数
		MAX_FILE_LEN = 256,		// ファイル名の最大文字数
	};

	// デフォルトコンストラクタ
	WmxException() : m_Line(0)
	{
		*m_Msg = _T('\0');
		*m_File = _T('\0');
	}

	// コピーコンストラクタ
	WmxException(const WmxException &e)
	{
		_tcscpy(m_Msg, e.m_Msg);
		_tcscpy(m_File, e.m_File);
		m_Line = e.m_Line;
	}

	// コンストラクタ
	WmxException(const TCHAR *msg, const TCHAR *file, int line) : m_Line(line)
	{
		_tcsncpy(m_Msg, msg, MAX_MSG_LEN);
		_tcsncpy(m_File, file, MAX_FILE_LEN);
		m_Msg[MAX_MSG_LEN - 1] = _T('\0');
		m_File[MAX_FILE_LEN - 1] = _T('\0');
	}

	// コピー演算子のオーバーロード
	virtual WmxException &operator = (const WmxException &e)
	{
		_tcscpy(m_Msg, e.m_Msg);
		_tcscpy(m_File, e.m_File);
		m_Line = e.m_Line;
		return *this;
	}

	// 有効性チェック
	virtual bool IsValid(void) const
	{
		return *m_File != _T('\0') && m_Line > 0;
	}

	// メッセージの取得
	virtual const TCHAR *Message(void) const
	{
		return m_Msg;
	}

	// ファイル名の取得
	virtual const TCHAR *Filename(void) const
	{
		return m_File;
	}

	// 行番号の取得
	virtual int LineNo(void) const
	{
		return m_Line;
	}

private:

	TCHAR	m_Msg[MAX_MSG_LEN];			// メッセージ文字列
	TCHAR	m_File[MAX_FILE_LEN];		// ファイル名文字列
	int		m_Line;						// 行番号

};

#endif
