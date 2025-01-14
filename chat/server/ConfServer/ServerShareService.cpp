// ServerShareService.cpp : implementation file
//

#include "stdafx.h"
#include "ServerConfDlg.h"
#include "ServerShareService.h"

int CServerShareService::m_iCountInstance;
// CServerShareService

IMPLEMENT_DYNCREATE(CServerShareService, CWinThread)

CServerShareService::CServerShareService()
{
	++CServerShareService::m_iCountInstance;
	m_SemaQueueDownload = new CSemaphore(1, 1);
	m_SemaQueueShare = new CSemaphore(1, 1);
	for (int i=0; i< MAX_PORT; ++i)
	{
		m_aPortUsing[i] = NULL;
	}
}

CServerShareService::~CServerShareService()
{
	--CServerShareService::m_iCountInstance;
	delete m_SemaQueueDownload;
	delete m_SemaQueueShare;
	for (int i=0; i< m_ListShareThread.size(); ++i)
	{
		CServerShareThread::HardDelete(&m_ListShareThread[i]);
	}
	for (int i=0; i< m_ListDownThread.size(); ++i)
	{
		CServerDownloadThread::HardDelete(&m_ListDownThread[i]);
	}
}

BOOL CServerShareService::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CServerShareService::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CServerShareService, CWinThread)
END_MESSAGE_MAP()

CServerShareService * CServerShareService::CreateCServerShareService(CDialog * m_Window)
{
	if (CServerShareService::m_iCountInstance >= 1)
	{
		return NULL;
	}
	CServerShareService * t = (CServerShareService*) AfxBeginThread(RUNTIME_CLASS(CServerShareService), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	t->m_ParentWindow = m_Window;
	t->ResumeThread();
	return t;
}

void CServerShareService::HardDelete( CServerShareService ** t )
{
	TerminateThread((*t)->m_hThread, 0);
	delete (*t);
	*t = NULL;
}

int CServerShareService::Run()
{
	while (TRUE)
	{
		//don gac
		for (int i=0; i<m_ListShareThread.size(); ++i)
		{
			if (m_ListShareThread[i] == NULL)
			{
				m_ListShareThread.erase(m_ListShareThread.begin()+i);
				--i;
			}
			else if (m_ListShareThread[i]->GetTimeEdle() > MAX_TIMEOUT)
			{
				RecoverPort(m_ListShareThread[i]);
				CServerShareThread::HardDelete(&m_ListShareThread[i]);
				m_ListShareThread.erase(m_ListShareThread.begin()+i);
				--i;
			}
		}
		// tim file nao down xong thi cho vào đợi share
		for (int i=0; i<m_ListDownThread.size(); ++i)
		{
			if (m_ListDownThread[i] == NULL)
			{
				m_ListDownThread.erase(m_ListDownThread.begin()+i);
				--i;
			}
			else if (m_ListDownThread[i]->GetExitCode() != STILL_ACTIVE)
			{
				if (m_ListDownThread[i]->GetExitCode() == 0)
				{
					RecoverPort(m_ListDownThread[i]);
					CServerDownloadThread::HardDelete(&m_ListDownThread[i]);
					m_ListDownThread.erase(m_ListDownThread.begin()+i);
					--i;
				}
				else if (m_ListDownThread[i]->GetExitCode() == 1)
				{
					QueueItem t;
					t.m_stFilePath = m_ListDownThread[i]->GetFilePath();
					t.m_bIsPublicShare = m_ListDownThread[i]->IsPublicShare();
					t.m_stReceiver = m_ListDownThread[i]->GetReceiver();
					t.m_stSender = m_ListDownThread[i]->GetSender();
					m_SemaQueueShare->Lock();
					m_QueueShare.push_back(t);
					m_SemaQueueShare->Unlock();
					RecoverPort(m_ListDownThread[i]);
					CServerDownloadThread::HardDelete(&m_ListDownThread[i]);
					m_ListDownThread.erase(m_ListDownThread.begin()+i);
					--i;
				}
			}
		}
		//kiem tra hang doi share va share
		m_SemaQueueShare->Lock();
		for (int i=0; i<m_QueueShare.size(); ++i)
		{
			int port = FindPortIndex();
			CFile f;
			f.Open( m_QueueShare[i].m_stFilePath, CFile::modeRead);
			ULONGLONG lgFileSize = f.GetLength();
			f.Close();
			CServerShareThread * t = CServerShareThread::CreateCServerShareThread(PORT_BASE+port, m_QueueShare[i].m_stFilePath);
			if (t != NULL)
			{
				m_aPortUsing[port] = t;

				CString cstrFileSize;
				cstrFileSize.Format(L"%d", lgFileSize);
				CString cstrPort;
				cstrPort.Format(L"%d", PORT_BASE+port);
				if (m_QueueShare[i].m_bIsPublicShare)
				{
					//share public gởi lời mời đến client khác
					((CServerConfDlg*)m_ParentWindow)->AnnouncePubFile( &cstrFileSize, &cstrPort, &m_QueueShare[i].m_stFilePath);
				} 
				else
				{
					//share private gởi tin đến người đc share connect vô cstrPort
					//chương trình 9 phải đảm bảo ánh xạ "m_stFilePath" - (lúc này nó chỉ là tên file tạm trong thư mục hiện hành) - với username cần gởi để chỉ send cho 1 username
					((CServerConfDlg*)m_ParentWindow)->AnnouncePrivFile( &cstrFileSize, &cstrPort, &m_QueueShare[i].m_stFilePath, &m_QueueShare[i].m_stSender, &m_QueueShare[i].m_stReceiver);
				}
				

				m_QueueShare.erase(m_QueueShare.begin()+i);
				--i;
				m_ListShareThread.push_back(t);
				t->ResumeThread();

				//////////////////////////////////////////////////////////////////////////
				//gọi 1 hàm bên ct chat yc gởi đề nghị serfile đến các client khác 
				//m_ParentWindow->abc();
				//////////////////////////////////////////////////////////////////////////
			}
			else //neu khong con slot buoc fai xoa file nao co timeout cao nhat
			{
				int iMax = 0;
				int jMax = -1;
				for (int j = 0; j<m_ListShareThread.size(); ++j)
				{
					if (m_ListShareThread[j]->GetTimeEdle()>iMax)
					{
						iMax = m_ListShareThread[j]->GetTimeEdle();
						jMax = j;
					}
				}
				if (jMax < 0) break;
				RecoverPort(m_ListShareThread[jMax]);
				CServerShareThread::HardDelete(&m_ListShareThread[jMax]);
				m_ListShareThread.erase(m_ListShareThread.begin()+jMax);
				--i;
			}
		}
		m_SemaQueueShare->Unlock();
		//kiem tra hang doi xin upfile va nhan file up len
		m_SemaQueueDownload->Lock();
		for (int i=0; i<m_QueueDownload.size(); ++i)
		{
			CServerDownloadThread * t = CServerDownloadThread::CreateCServerDownloadThread(m_QueueDownload[i].m_stFilePath, m_QueueDownload[i].m_iPort, m_QueueDownload[i].m_bIsPublicShare);
			if (m_QueueDownload[i].m_bIsPublicShare)
			{
				t->SetReceiver(_T(""));
				t->SetSender(_T(""));
			} 
			else
			{
				t->SetReceiver(m_QueueDownload[i].m_stReceiver);
				t->SetSender(m_QueueDownload[i].m_stSender);
			}
			if (t != NULL)
			{
				m_aPortUsing[m_QueueDownload[i].m_iPort - PORT_BASE] = t;
				m_QueueDownload.erase(m_QueueDownload.begin()+i);
				--i;
				m_ListDownThread.push_back(t);
				t->ResumeThread();
			}
		}
		m_SemaQueueDownload->Unlock();
		Sleep(200);
	}
}

void CServerShareService::RecoverPort( void * p )
{
	for (int i=0; i< MAX_PORT; ++i)
	{
		if (m_aPortUsing[i] == p)
		{
			m_aPortUsing[i] = NULL;
			break;
		}
	}
}

int CServerShareService::FindPortIndex()
{
	for (int i=0; i< MAX_PORT; ++i)
	{
		if (m_aPortUsing[i] == NULL)
		{
			return i;
		}
	}

	return -1;		//!!aakhoi them o day
}

int CServerShareService::OnClientNeedShareFilePublic( CString stFileName )
{
	m_SemaQueueDownload->Lock();
	QueueItem t;
	t.m_stFilePath = stFileName;
	t.m_iPort = FindPortIndex();
	m_aPortUsing[t.m_iPort] = &t;
	t.m_iPort += PORT_BASE;
	t.m_bIsPublicShare = TRUE;
	m_QueueDownload.push_back(t);
	m_SemaQueueDownload->Unlock();
	return t.m_iPort;
}

int CServerShareService::OnClientNeedShareFilePrivate( CString stFileName, CString stSender, CString stReceiver )
{
	m_SemaQueueDownload->Lock();
	QueueItem t;
	t.m_stFilePath = stFileName;
	t.m_iPort = FindPortIndex();
	m_aPortUsing[t.m_iPort] = &t;
	t.m_iPort += PORT_BASE;

	t.m_bIsPublicShare = FALSE;
	t.m_stSender = stSender;
	t.m_stReceiver = stReceiver;

	m_QueueDownload.push_back(t);
	m_SemaQueueDownload->Unlock();
	return t.m_iPort;
}


// CServerShareService message handlers

void InitStaticMemberServer()
{
	CServerShareService::m_iCountInstance = 0;
	CServerShareThread::m_iCountInstance = 0;
	CServerDownloadThread::m_iCountInstance = 0;
}
