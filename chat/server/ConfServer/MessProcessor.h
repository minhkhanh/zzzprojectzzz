#pragma once

#include <vector>
using namespace std;

#define m_item(IDControl) GetDlgItem(IDControl)

//////////////////////////////////////////////////////////////////////////
/// Lop MessProcessor chua cac thong tin ve message code, error code,
/// ham xu ly chuoi thong diep
//////////////////////////////////////////////////////////////////////////
class MessProcessor
{
public:
	MessProcessor(void);
	~MessProcessor(void);

	// message code:
	static TCHAR *MC_PUBLIC_CHAT;			// public chat message
	static TCHAR *MC_USERNAME_REQUEST;		// yeu cau kiem tra username tu client
	static TCHAR *MC_USER_LOGGED_IN;		// user dang nhap thanh cong
	static TCHAR *MC_USERNAME_LIST;			// server gui danh sach client
	static TCHAR *MC_USER_LOGGEDOUT;		// thong bao user thoat ra
	static TCHAR *MC_PRIVATE_CHAT;			// private chat message
	static TCHAR *MC_UPLOAD_REQUEST;		// yeu cau upload file
	static TCHAR *MC_UPLOAD_READY;			// client dc phep upload
	static TCHAR *MC_PUBLIC_FILE_OFFER;		// thong tin cua file dc share
	static TCHAR *MC_PRIVATE_FILE_OFFER;	// client de nghi client ben kia nhan file private
	static TCHAR *MC_PRIVATE_FILE_ACCEPT;	// client chap nhan de nghi share file private tu client khac
	static TCHAR *MC_PRIVATE_FILE_DECLINE;	// client ko dong y nhan file share private
	static TCHAR *MC_PRIVATE_FILE_DOWNLOAD;	// server de nghi client save file share private

	// error code:
	static TCHAR *EC_USERNAME_EXISTED;		// bao loi username bi trung
	static TCHAR *EC_USERNAME_LOGGEDOUT;	// bao loi remote user da thoat trong khi dang chat private
	static TCHAR *EC_FILE_NOTFOUND;			// bao loi ko tim ra file

	// lay doan chuoi dau tien ra khoi message
	static CString PeelMess(CString *cstrMess);

	// ghep nhieu chuoi thanh mot chuoi theo dinh dang message
	static CString JoinMess(TCHAR *cstrMess[], int count);
};
