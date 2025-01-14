// ClientShareService.cpp : implementation file
//

#include "stdafx.h"
#include "ClientConf.h"
#include "ClientShareService.h"
#include "ClientUploadThread.h"
#include "ClientSendClientThread.h"

int CClientShareService::m_iCountInstance;
// CClientShareService

IMPLEMENT_DYNCREATE(CClientShareService, CWinThread)

CClientShareService::CClientShareService()
{
	++CClientShareService::m_iCountInstance;
	for (int i=0; i< MAX_PORT; ++i)
	{
		m_aPortUsing[i] = NULL;
	}
	m_QueueSend.clear();
	m_QueueUpload.clear();
	m_QueueDownload.clear();
	m_SemaQueueDownload = new CSemaphore(1, 1);
	m_SemaQueueUpload = new CSemaphore(1, 1);
	m_SemaQueueSend = new CSemaphore(1, 1);
}

CClientShareService::~CClientShareService()
{
	--CClientShareService::m_iCountInstance;
	// dọn rác 
	int iItem = m_ListViewDown->GetItemCount();
	for (int i=0; i<iItem; ++i)
	{
		CString st = m_ListViewDown->GetItemText(i, 1);
		if (st.Find(_T("Download"))==-1)//kiem tra cac truong hop down
		{
			if (st.Find(_T("Upload"))==-1)
			{
				//truong hop send
				LVITEM lvItem;
				lvItem.mask = LVIF_PARAM;
				lvItem.iItem = i;
				lvItem.iSubItem = 0;
				m_ListViewDown->GetItem(&lvItem);
				CClientSendClientThread* t = (CClientSendClientThread*) lvItem.lParam;
				if (t!=NULL)
				{
					//hoan tra port;
					RecoverPort(t);
					CClientSendClientThread::HardDelete(&t);
				}
				continue;
				//m_ListViewDown->DeleteItem(i);
			}
			// truong hop upload
			LVITEM lvItem;
			lvItem.mask = LVIF_PARAM;
			lvItem.iItem = i;
			lvItem.iSubItem = 0;
			m_ListViewDown->GetItem(&lvItem);
			CClientUploadThread* t = (CClientUploadThread*) lvItem.lParam;
			if (t!=NULL)
			{
				CClientUploadThread::HardDelete(&t);
			}
			continue;
			//m_ListViewDown->DeleteItem(i);
		}
		LVITEM lvItem;
		lvItem.mask = LVIF_PARAM;
		lvItem.iItem = i;
		lvItem.iSubItem = 0;
		m_ListViewDown->GetItem(&lvItem);
		CClientShareTranport * t = (CClientShareTranport*) lvItem.lParam;
		if (t!=NULL)
		{
			CClientShareTranport::HardDelete(&t);
		}
		//m_ListViewDown->DeleteItem(nItem);
	}
	delete m_SemaQueueDownload;
	delete m_SemaQueueUpload;
	delete m_SemaQueueSend;
}

BOOL CClientShareService::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CClientShareService::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CClientShareService, CWinThread)
END_MESSAGE_MAP()

CClientShareService * CClientShareService::Create( CListCtrl * m_List )
{
	if (CClientShareService::m_iCountInstance>0)
	{
		return NULL;
	}
	CClientShareService * t = (CClientShareService *) AfxBeginThread(RUNTIME_CLASS(CClientShareService), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	t->m_ListViewDown = m_List;
	t->InitListViewColumn();
	t->ResumeThread();
	return t;
}

void CClientShareService::InitListViewColumn()
{
	m_ListViewDown->SetExtendedStyle(LVS_EX_FULLROWSELECT);//|LVS_SINGLESEL);
	int nColumnCount = m_ListViewDown->GetHeaderCtrl()->GetItemCount();
	for (int i=0;i < nColumnCount;i++)
	{
		m_ListViewDown->DeleteColumn(0);
	}
	LVCOLUMN lvColumn;

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 220;
	lvColumn.pszText = _T("Filename");
	m_ListViewDown->InsertColumn(0, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 100;
	lvColumn.pszText = _T("Status");
	m_ListViewDown->InsertColumn(1, &lvColumn);

	lvColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 80;
	lvColumn.pszText = _T("%");
	m_ListViewDown->InsertColumn(2, &lvColumn);


}

void CClientShareService::Download( CString stFilePath, CString stAddress, int iPort )
{
	QueueItem t;
	t.iPort = iPort;
	t.stAddress = stAddress;
	t.stFilePath = stFilePath;
	m_SemaQueueDownload->Lock();
	m_QueueDownload.push_back(t);
	m_SemaQueueDownload->Unlock();
}

int CClientShareService::Run()
{
	while (TRUE)
	{
		// dọn rác bỏ slot đã down xong và update %
		int iItem = m_ListViewDown->GetItemCount();
		for (int i=0; i<iItem; ++i)
		{
			CString st = m_ListViewDown->GetItemText(i, 1);
			if (st.Find(_T("Download"))==-1)//kiem tra cac truong hop down hay up
			{
				if (st.Find(_T("Upload"))==-1)
				{
					//truong hop send
					LVITEM lvItem;
					lvItem.mask = LVIF_PARAM;
					lvItem.iItem = i;
					lvItem.iSubItem = 0;
					m_ListViewDown->GetItem(&lvItem);
					CClientSendClientThread* t = (CClientSendClientThread*) lvItem.lParam;
					if (t==NULL)
					{
						continue;
					}
					m_ListViewDown->SetItemText(i, 2, t->GetPercent().GetString());
					DWORD iExitCode = t->GetExitCode();
					if (iExitCode == STILL_ACTIVE)
					{
						continue;
					}
					if (iExitCode == 0)
					{
						lvItem.stateMask = LVIF_TEXT;
						lvItem.iSubItem = 1;
						lvItem.pszText = _T("Send false");
						m_ListViewDown->SetItem(&lvItem);
						lvItem.mask = LVIF_PARAM;
						lvItem.iSubItem = 0;
						lvItem.lParam = 0;
						m_ListViewDown->SetItem(&lvItem);
						CClientSendClientThread::HardDelete(&t);
						continue;
					}
					if (iExitCode == 1)
					{
						lvItem.mask = LVIF_TEXT;
						lvItem.iSubItem = 1;
						lvItem.pszText = _T("Send finsh");
						m_ListViewDown->SetItem(&lvItem);
						lvItem.mask = LVIF_PARAM;
						lvItem.iSubItem = 0;
						lvItem.lParam = 0;
						m_ListViewDown->SetItem(&lvItem);
						CClientSendClientThread::HardDelete(&t);
						continue;
					}
					continue;
				}
				//truong hop up
				LVITEM lvItem;
				lvItem.mask = LVIF_PARAM;
				lvItem.iItem = i;
				lvItem.iSubItem = 0;
				m_ListViewDown->GetItem(&lvItem);
				CClientUploadThread * t = (CClientUploadThread*) lvItem.lParam;
				if (t==NULL)
				{
					continue;
				}
				m_ListViewDown->SetItemText(i, 2, t->GetPercent().GetString());
				DWORD iExitCode = t->GetExitCode();
				if (iExitCode == STILL_ACTIVE)
				{
					continue;
				}
				if (iExitCode == 0)
				{
					lvItem.stateMask = LVIF_TEXT;
					lvItem.iSubItem = 1;
					lvItem.pszText = _T("Upload false");
					m_ListViewDown->SetItem(&lvItem);
					lvItem.mask = LVIF_PARAM;
					lvItem.iSubItem = 0;
					lvItem.lParam = 0;
					m_ListViewDown->SetItem(&lvItem);
					CClientUploadThread::HardDelete(&t);
					continue;
				}
				if (iExitCode == 1)
				{
					lvItem.mask = LVIF_TEXT;
					lvItem.iSubItem = 1;
					lvItem.pszText = _T("Upload finsh");
					m_ListViewDown->SetItem(&lvItem);
					lvItem.mask = LVIF_PARAM;
					lvItem.iSubItem = 0;
					lvItem.lParam = 0;
					m_ListViewDown->SetItem(&lvItem);
					CClientUploadThread::HardDelete(&t);
					continue;
				}
				continue;
			}
			//truong hop down
			LVITEM lvItem;
			lvItem.mask = LVIF_PARAM;
			lvItem.iItem = i;
			lvItem.iSubItem = 0;
			m_ListViewDown->GetItem(&lvItem);
			CClientShareTranport * t = (CClientShareTranport*) lvItem.lParam;
			if (t==NULL)
			{
				continue;
			}
			m_ListViewDown->SetItemText(i, 2, t->GetPercent().GetString());
			DWORD iExitCode = t->GetExitCode();
			if (iExitCode == STILL_ACTIVE)
			{
				continue;
			}
			if (iExitCode == 0)
			{
				lvItem.stateMask = LVIF_TEXT;
				lvItem.iSubItem = 1;
				lvItem.pszText = _T("Download false");
				m_ListViewDown->SetItem(&lvItem);
				lvItem.mask = LVIF_PARAM;
				lvItem.iSubItem = 0;
				lvItem.lParam = 0;
				m_ListViewDown->SetItem(&lvItem);
				CClientShareTranport::HardDelete(&t);
				continue;
			}
			if (iExitCode == 1)
			{
				lvItem.mask = LVIF_TEXT;
				lvItem.iSubItem = 1;
				lvItem.pszText = _T("Download finsh");
				m_ListViewDown->SetItem(&lvItem);
				lvItem.mask = LVIF_PARAM;
				lvItem.iSubItem = 0;
				lvItem.lParam = 0;
				m_ListViewDown->SetItem(&lvItem);
				CClientShareTranport::HardDelete(&t);
				continue;
			}
		}

		m_SemaQueueUpload->Lock();
		for (int i = 0; i<m_QueueUpload.size(); ++i)
		{
			//thử new xem còn slot không
			CClientUploadThread * t = CClientUploadThread::CreateCClientUploadThread(m_QueueUpload[i].stFilePath, m_QueueUpload[i].stAddress, m_QueueUpload[i].iPort);
			if (t != NULL)
			{
				LVITEM lvItem;
				int nItem;
				lvItem.mask = LVIF_TEXT|LVIF_PARAM;
				lvItem.iItem = m_ListViewDown->GetItemCount();
				lvItem.iSubItem = 0;
				lvItem.lParam = (LPARAM)t;
				lvItem.pszText = (LPWSTR)m_QueueUpload[i].stFilePath.GetString();
				nItem = m_ListViewDown->InsertItem(&lvItem);
				m_ListViewDown->SetItemText(nItem, 1, _T("Upload"));

				m_QueueUpload.erase(m_QueueUpload.begin() + i);
				--i;
				t->ResumeThread();
			}
			else
			{
				break;
			}
		}
		m_SemaQueueUpload->Unlock();

		m_SemaQueueDownload->Lock();
		for (int i = 0; i<m_QueueDownload.size(); ++i)
		{
			//thử new xem còn slot không
			CClientShareTranport * t = CClientShareTranport::Create(m_QueueDownload[i].stAddress, m_QueueDownload[i].iPort, m_QueueDownload[i].stFilePath);
			if (t != NULL)
			{
				LVITEM lvItem;
				int nItem;
				lvItem.mask = LVIF_TEXT|LVIF_PARAM;
				lvItem.iItem = m_ListViewDown->GetItemCount();
				lvItem.iSubItem = 0;
				lvItem.lParam = (LPARAM)t;
				lvItem.pszText = (LPWSTR)m_QueueDownload[i].stFilePath.GetString();
				nItem = m_ListViewDown->InsertItem(&lvItem);
				m_ListViewDown->SetItemText(nItem, 1, _T("Download"));

				m_QueueDownload.erase(m_QueueDownload.begin() + i);
				--i;
				t->ResumeThread();
			}
			else
			{
				break;
			}
		}
		m_SemaQueueDownload->Unlock();

		m_SemaQueueSend->Lock();
		for (int i = 0; i<m_QueueSend.size(); ++i)
		{
			//thử new xem còn slot không
			CClientSendClientThread * t = CClientSendClientThread::CreateCClientSendClientThread(m_QueueSend[i].stFilePath, m_QueueSend[i].iPort);
			if (t != NULL)
			{
				LVITEM lvItem;
				int nItem;
				lvItem.mask = LVIF_TEXT|LVIF_PARAM;
				lvItem.iItem = m_ListViewDown->GetItemCount();
				lvItem.iSubItem = 0;
				lvItem.lParam = (LPARAM)t;
				lvItem.pszText = (LPWSTR)m_QueueSend[i].stFilePath.GetString();
				nItem = m_ListViewDown->InsertItem(&lvItem);
				m_ListViewDown->SetItemText(nItem, 1, _T("Send"));
				m_aPortUsing[m_QueueSend[i].iPort - PORT_BASE] = t;
				m_QueueSend.erase(m_QueueSend.begin() + i);
				--i;
				t->ResumeThread();
			}
			else
			{
				break;
			}
		}
		m_SemaQueueSend->Unlock();

		Sleep(200);
	}
}

void CClientShareService::HardDelete( CClientShareService ** t )
{
	TerminateThread((*t)->m_hThread, 0);
	delete (*t);
	*t = NULL;
}

void CClientShareService::OnDeleteCancel()
{
	POSITION pos = m_ListViewDown->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		//TRACE0("No items were selected!\n");
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = m_ListViewDown->GetNextSelectedItem(pos);
			//TRACE1("Item %d was selected!\n", nItem);
			CString st = m_ListViewDown->GetItemText(nItem, 1);
			if (st.Find(_T("Download"))==-1)//kiem tra cac truong hop down
			{
				if (st.Find(_T("Upload"))==-1)
				{
					//truong hop send
					LVITEM lvItem;
					lvItem.mask = LVIF_PARAM;
					lvItem.iItem = nItem;
					lvItem.iSubItem = 0;
					m_ListViewDown->GetItem(&lvItem);
					CClientSendClientThread* t = (CClientSendClientThread*) lvItem.lParam;
					if (t!=NULL)
					{
						//hoan tra port;
						RecoverPort(t);
						CClientSendClientThread::HardDelete(&t);
					}
					m_ListViewDown->DeleteItem(nItem);
					continue;
				}
				// truong hop upload
				LVITEM lvItem;
				lvItem.mask = LVIF_PARAM;
				lvItem.iItem = nItem;
				lvItem.iSubItem = 0;
				m_ListViewDown->GetItem(&lvItem);
				CClientUploadThread* t = (CClientUploadThread*) lvItem.lParam;
				if (t!=NULL)
				{
					CClientUploadThread::HardDelete(&t);
				}
				m_ListViewDown->DeleteItem(nItem);
				continue;
			}
			// truong hop down
			LVITEM lvItem;
			lvItem.mask = LVIF_PARAM;
			lvItem.iItem = nItem;
			lvItem.iSubItem = 0;
			m_ListViewDown->GetItem(&lvItem);
			CClientShareTranport * t = (CClientShareTranport*) lvItem.lParam;
			if (t!=NULL)
			{
				CClientShareTranport::HardDelete(&t);
			}
			m_ListViewDown->DeleteItem(nItem);
		}
	}
}

void CClientShareService::Upload( CString stFilePath, CString stAddress, int iPort )
{
	QueueItem t;
	t.iPort = iPort;
	t.stAddress = stAddress;
	t.stFilePath = stFilePath;
	m_SemaQueueUpload->Lock();
	m_QueueUpload.push_back(t);
	m_SemaQueueUpload->Unlock();
}

int CClientShareService::SendFileToClient( CString stFilePath )
{
	QueueItem t;
	t.stFilePath = stFilePath;
	t.iPort = FindPortIndex();
	m_aPortUsing[t.iPort] = &t;
	t.iPort += PORT_BASE;
	m_SemaQueueSend->Lock();
	m_QueueSend.push_back(t);
	m_SemaQueueSend->Unlock();
	return t.iPort;
}

void CClientShareService::RecoverPort( void * p )
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

int CClientShareService::FindPortIndex()
{
	for (int i=0; i< MAX_PORT; ++i)
	{
		if (m_aPortUsing[i] == NULL)
		{
			return i;
		}
	}
	return -1;
}


// CClientShareService message handlers

void InitStaticMemberClient()
{
	CClientShareService::m_iCountInstance = 0;
	CClientShareTranport::m_iCountInstance = 0;
	CClientUploadThread::m_iCountInstance = 0;
}
