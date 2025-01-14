#include "StdAfx.h"
#include "ServerComm.h"

ServerComm::ServerComm(void)
{
}

ServerComm::ServerComm( SOCKET sock )
{
	this->sockClient = sock;
}

ServerComm::~ServerComm(void)
{
	//closesocket(sockClient);
}

int ServerComm::Send( TCHAR *strMessage )
{
	int iCharNum = wcslen(strMessage);
	send(sockClient, (char*)&iCharNum, sizeof(int), 0);							//!!send message size di truoc, ben nhan phai co co che phan biet size message
	return send(sockClient, (char*)strMessage, iCharNum * sizeof(TCHAR), 0);	// gui message di
}

void ServerComm::SetInfo( SOCKET sock , CString cstrLocalName)
{
	this->sockClient = sock;
	this->cstrLocalUsername = cstrLocalName;
}

int ServerComm::SendPublicMess(CString *cstrMessContent )
{
	TCHAR *carrStr[] = {MessProcessor::MC_PUBLIC_CHAT, cstrMessContent->GetBuffer()};
	CString cstrMess = MessProcessor::JoinMess(carrStr, 2);

	return this->Send(cstrMess.GetBuffer());
}

int ServerComm::SendLogInErr(CString *cstrMessBody )
{
	TCHAR *arrStr[] = {MessProcessor::EC_USERNAME_EXISTED, cstrMessBody->GetBuffer()};
	CString cstrErrMess = MessProcessor::JoinMess(arrStr, 2);

	return this->Send(cstrErrMess.GetBuffer());
}

int ServerComm::SendLoggedInMess(CString *cstrUsername, CString *cstrMessBody)
{
	TCHAR *arrStr[] = {MessProcessor::MC_USER_LOGGED_IN, cstrUsername->GetBuffer(), cstrMessBody->GetBuffer()};
	CString cstrMess = MessProcessor::JoinMess(arrStr, 3);

	return this->Send(cstrMess.GetBuffer());
}

int ServerComm::SendLoggedOutMess(CString *cstrUsername, CString *cstrMessBody)
{
	TCHAR *carrStr[] = {MessProcessor::MC_USER_LOGGEDOUT, cstrUsername->GetBuffer(), cstrMessBody->GetBuffer()};
	CString cstrLoggedOutMess = MessProcessor::JoinMess(carrStr, 3);

	return this->Send(cstrLoggedOutMess.GetBuffer());
}

void ServerComm::End()
{
	// gracefull ending
	shutdown(sockClient, SD_BOTH);
	closesocket(sockClient);
}

int ServerComm::SendUserLv( CListCtrl *lvUser )
{
	CString cstrUserList = MessProcessor::MC_USERNAME_LIST;

	// tao chuoi co chua danh sach username:
	int i = 0;
	for ( ; i < lvUser->GetItemCount(); ++i)
		cstrUserList = cstrUserList + L" " + lvUser->GetItemText(i, 0);

	return this->Send(cstrUserList.GetBuffer());
}

int ServerComm::SendPrivMessErr( CString *cstrSender, CString *cstrMess )
{
	TCHAR *arrStr[] = {MessProcessor::EC_USERNAME_LOGGEDOUT, cstrSender->GetBuffer(), cstrMess->GetBuffer()};
	CString cstrErrMess = MessProcessor::JoinMess(arrStr, 3);

	return Send(cstrErrMess.GetBuffer());
}

int ServerComm::SendPrivMess( CString *cstrSender, CString *cstrMessBody )
{
	TCHAR *arrStr[] = {MessProcessor::MC_PRIVATE_CHAT, cstrSender->GetBuffer(), cstrMessBody->GetBuffer()};
	CString cstrPrivMess = MessProcessor::JoinMess(arrStr, 3);

	return this->Send(cstrPrivMess.GetBuffer());
}

int ServerComm::SendUploadReadyMess(CString *cstrPort )
{
	TCHAR *arrStr[] = {MessProcessor::MC_UPLOAD_READY, cstrPort->GetBuffer()};
	CString cstrMess = MessProcessor::JoinMess(arrStr, 2);

	return this->Send(cstrMess.GetBuffer());
}

int ServerComm::SendDownloadOffer( CString *cstrFileSize, CString *cstrPort, CString *cstrFileName )
{
	TCHAR *arrStr[] = {MessProcessor::MC_PUBLIC_FILE_OFFER, cstrFileSize->GetBuffer(), cstrPort->GetBuffer(), cstrFileName->GetBuffer()};
	CString cstrMess = MessProcessor::JoinMess(arrStr, 4);

	return this->Send(cstrMess.GetBuffer());
}

int ServerComm::SendPrivateFileOffer( CString *cstrSender, CString *cstrMessContent )
{
	TCHAR *arrStr[] = {MessProcessor::MC_PRIVATE_FILE_OFFER, cstrSender->GetBuffer(), cstrMessContent->GetBuffer()};
	CString cstrMess = MessProcessor::JoinMess(arrStr, 3);

	return this->Send(cstrMess.GetBuffer());
}

int ServerComm::SendPrivFileAccept( CString *cstrSender, CString *cstrServerPort )
{
	TCHAR *arrStr[] = {MessProcessor::MC_PRIVATE_FILE_ACCEPT, cstrSender->GetBuffer(), cstrServerPort->GetBuffer()};
	CString cstrMess = MessProcessor::JoinMess(arrStr, 3);

	return this->Send(cstrMess.GetBuffer());
}

int ServerComm::SendPrivFileDownload( CString *cstrSender, CString *cstrFileName, CString *cstrFileSize, CString *cstrPort )
{
	TCHAR *arrStr[] = {MessProcessor::MC_PRIVATE_FILE_DOWNLOAD, cstrSender->GetBuffer(), cstrFileSize->GetBuffer(), cstrPort->GetBuffer(), cstrFileName->GetBuffer()};
	CString cstrMess = MessProcessor::JoinMess(arrStr, 5);

	return this->Send(cstrMess.GetBuffer());
}