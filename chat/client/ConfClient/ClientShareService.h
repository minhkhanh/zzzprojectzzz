#pragma once

#include <vector>
#include "ClientShareTranport.h"
#include <afxmt.h>
using namespace std;

// CClientShareService
#define PORT_BASE 4000
#define MAX_PORT 20
struct QueueItem
{
	CString stFilePath;
	CString stAddress;
	int iPort;
};

class CClientShareService : public CWinThread
{
	DECLARE_DYNCREATE(CClientShareService)

protected:
	CClientShareService();           // protected constructor used by dynamic creation
	virtual ~CClientShareService();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	static int m_iCountInstance;
	static CClientShareService * Create(CListCtrl * m_List);
	void InitListViewColumn();
	void Download(CString stFilePath, CString stAddress, int iPort); // chi goi khi da hoi nguoi dung co muon down ko va lay duoc duong dan
	void OnDeleteCancel(); // chuong tring ngoai fai tu hoi nguoi dung da chac chan chua
	void Upload(CString stFilePath, CString stAddress, int iPort); // goi khi da hoi nguoi dung thong tin, port duoc cap tu server
	int SendFileToClient(CString stFilePath);  //gởi file đến 1 client khác trả về port, ct ngoài tự chuyển port đến máy đích, máy đích sẽ down bình thường như từ server chỉ khác ip
	virtual int Run();
	static void HardDelete(CClientShareService ** t);

private:
	void RecoverPort(void * p);
	int FindPortIndex();

	vector<QueueItem> m_QueueDownload;
	vector<QueueItem> m_QueueUpload;
	vector<QueueItem> m_QueueSend;
	CSemaphore * m_SemaQueueDownload;
	CSemaphore * m_SemaQueueUpload;
	CSemaphore * m_SemaQueueSend;
	CListCtrl * m_ListViewDown;

	void* m_aPortUsing[MAX_PORT];

protected:
	DECLARE_MESSAGE_MAP()
};

void InitStaticMemberClient();
