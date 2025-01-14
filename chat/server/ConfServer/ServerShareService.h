#pragma once
#include <vector>
#include "ServerShareThread.h"
#include "ServerDownloadThread.h"
using namespace std;
//////////////////////////////////////////////////////////////////////////
//chương trình server chỉ thao tác với duy nhất lớp này
//////////////////////////////////////////////////////////////////////////
//#define MAX_FILE_SHARE 10;
#define MAX_TIMEOUT 1500   // file sao 5 phut ko down se xoa
#define PORT_BASE 5000
#define MAX_PORT MAX_FILE_SHARE+MAX_FILE_UP_TO_SERVER + 20
// CServerShareService
struct QueueItem 
{
	CString m_stFilePath;
	int m_iPort;
	bool m_bIsPublicShare;

	CString m_stSender;		// neu la share public thi = "", nguoc lai share private thi = Sender User
	CString m_stReceiver;	// neu la share public thi = "", nguoc lai share private thi = Receiver User
};

class CServerShareService : public CWinThread
{
	DECLARE_DYNCREATE(CServerShareService)

protected:
	CServerShareService();           // protected constructor used by dynamic creation
	virtual ~CServerShareService();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	static int m_iCountInstance;
	static CServerShareService * CreateCServerShareService(CDialog * m_Window);//gọi đầu tiên để tạo 1 instance duy nhất truyền vào con trỏ this
	virtual int Run();
	static void HardDelete(CServerShareService ** t);
	int OnClientNeedShareFilePublic(CString stFileName);   //truyền vào tên file trả về port mà server sẽ lắng nghe clien truyền file lên
	int OnClientNeedShareFilePrivate(CString stFileName, CString stSender, CString stReceiver);   //truyền vào tên file trả về port mà server sẽ lắng nghe clien truyền file lên
									//dùng khi client cần chia sẻ private
private:
	void RecoverPort(void * p);
	int FindPortIndex();

	vector<CServerShareThread *> m_ListShareThread;
	vector<CServerDownloadThread *> m_ListDownThread;
	vector<QueueItem> m_QueueShare;
	vector<QueueItem> m_QueueDownload;
	void* m_aPortUsing[MAX_PORT];
	CSemaphore * m_SemaQueueShare;
	CSemaphore * m_SemaQueueDownload;

	CDialog * m_ParentWindow;
protected:
	DECLARE_MESSAGE_MAP()
};

void InitStaticMemberServer(); //gọi trước khi sử dụng lớp này
