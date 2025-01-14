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

	// server mess code:
	static TCHAR *MC_USER_LOGGED_IN;		// server thong bao co 1 client dang nhap thanh cong
	static TCHAR *MC_USERNAME_LIST;			// server gui danh sach client cho ca phong
	static TCHAR *MC_USER_LOGGEDOUT;		// server thong bao user thoat ra
	static TCHAR *MC_UPLOAD_READY;			// server cho phep client upload 1 file public
	static TCHAR *MC_PUBLIC_FILE_OFFER;		// server de nghi client nhan file share public
	static TCHAR *MC_PRIVATE_FILE_DOWNLOAD;	// server de nghi client save file share private

	// mess code chung:	
	static TCHAR *MC_PRIVATE_FILE_OFFER;	// client de nghi client ben kia nhan file private

	// client message code:
	static TCHAR *MC_USERNAME_REQUEST;		// client yeu cau dang nhap bang mot username
	static TCHAR *MC_PUBLIC_CHAT;			// public chat message	
	static TCHAR *MC_PRIVATE_CHAT;			// private chat message
	static TCHAR *MC_UPLOAD_REQUEST;		// client yeu cau duoc upload 1 file public
	static TCHAR *MC_PRIVATE_FILE_ACCEPT;	// client chap nhan de nghi share file private tu client khac	
	static TCHAR *MC_PRIVATE_FILE_DECLINE;	// client ko dong y nhan file share private

	// error code:
	static TCHAR *EC_USERNAME_EXISTED;		// bao loi username bi trung
	static TCHAR *EC_USERNAME_LOGGEDOUT;	// bao loi remote user da thoat trong khi dang chat private
	static TCHAR *EC_FILE_NOTFOUND;			// bao loi ko tim ra file

	// lay doan chuoi dau tien ra khoi message, dong thoi cung DELETE chuoi do trong chuoi message luon (!!!!)
	static CString PeelMess(CString *cstrMess);

	// ghep nhieu chuoi thanh mot chuoi theo dinh dang message
	static CString JoinMess(TCHAR *cstrMess[], int count);

	// 2 ham PeelMess va JoinMess co the tuy bien duoc ki tu phan cach cac thanh phan trong message
};
