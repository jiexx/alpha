package com.wind.customizedata;

import java.util.HashMap;
import java.util.Map;
import android.os.Parcelable;

import com.iwindnet.launch.WindnetLaunch;
import com.wind.book.CommitBookRequest;
import com.wind.book.CountOfBook;
import com.wind.book.CountOfBookRequest;
import com.wind.book.CoverPicOfBookRequest;
import com.wind.book.DetailOfBook;
import com.wind.book.DetailOfBookRequest;
import com.wind.book.ErrorCode2;
import com.wind.book.ListOfBookRequest;
import com.wind.book.ListOfBooks;
import com.wind.book.PublishBookRequest;
import com.wind.borrow.CountOfBookBorrowed;
import com.wind.borrow.CountOfBookBorrowedRequest;
import com.wind.borrow.ListOfBookBorrowed;
import com.wind.borrow.ListOfBookBorrowedRequest;
import com.wind.comment.CommentRequest;
import com.wind.comment.CountOfComment;
import com.wind.comment.CountOfCommentRequest;
import com.wind.comment.ListOfCommentRequest;
import com.wind.comment.ListOfComments;
import com.wind.login.ChangeAvatarRequest;
import com.wind.login.ChangeContactRequest;
import com.wind.login.ChangeNickRequest;
import com.wind.login.ErrorCode1;
import com.wind.login.LoginData;
import com.wind.login.LoginRequest;
import com.wind.login.PhoneAuthenticode;
import com.wind.login.PhoneAuthenticodeRequest;
import com.wind.login.RegisterRequest;
import com.wind.login.SetFamilyCodeRequest;
import com.wind.operate.OperateRequest;
import com.wind.ui.CommunicationData;
import com.wind.ui.NameParcel;

public enum CustomizedDataManager {
	INSTANCE;  
	public class Info {
		public int appClass;
		public int cmdId;
		public Class<?> response;
		public Info( int a, int c, Class<?> r ) {
			appClass = a;
			cmdId = c;
			response = r;
		}
	}
	private Map<Class<?>, Info> table = new HashMap<Class<?>, Info>();
	CustomizedDataManager() {
		table.put(PhoneAuthenticodeRequest.class,	new Info(1005, 602,		PhoneAuthenticode.class) );
		table.put(RegisterRequest .class,			new Info(1005, 603,		ErrorCode1.class) );
		table.put(LoginRequest.class,				new Info(1005, 604,		LoginData.class) );
		table.put(ChangeAvatarRequest.class,		new Info(1005, 605,		ErrorCode1.class) );
		table.put(ChangeNickRequest.class,			new Info(1005, 606,		ErrorCode1.class) );
		table.put(SetFamilyCodeRequest.class,		new Info(1005, 607,		ErrorCode1.class) );
		table.put(ChangeContactRequest.class,		new Info(1005, 608,		ErrorCode1.class) );
		table.put(CountOfBookRequest.class,			new Info(1006, 1101,	CountOfBook.class) );
		table.put(ListOfBookRequest.class,			new Info(1006, 1102,	ListOfBooks.class) );
		table.put(DetailOfBookRequest.class,		new Info(1006, 1103,	DetailOfBook.class) );
		table.put(CommitBookRequest.class,			new Info(1006, 1104,	ErrorCode2.class) );
		table.put(PublishBookRequest.class,			new Info(1006, 1105,	ErrorCode2.class) );
		table.put(CoverPicOfBookRequest .class,		new Info(1006, 1106,	ErrorCode2.class) );
		table.put(CountOfCommentRequest.class,		new Info(1006, 1201,	CountOfComment.class) );
		table.put(ListOfCommentRequest.class,		new Info(1006, 1202,	ListOfComments.class) );
		table.put(CommentRequest .class,			new Info(1006, 1203,	ErrorCode2.class) );
		table.put(CountOfBookBorrowedRequest.class, new Info(1006, 1301,	CountOfBookBorrowed.class) );
		table.put(ListOfBookBorrowedRequest.class,	new Info(1006, 1302,	ListOfBookBorrowed.class) );
		table.put(OperateRequest.class,				new Info(1006, 1401,	ErrorCode2.class) );
	}
	public NameParcel createRequest(Class<?> clazz, Object obj){
		NameParcel cd = null;
		if( table.containsKey(clazz) ) {
			cd = new NameParcel(clazz, (Parcelable)obj);
		}
		return cd;
	}
	public boolean checkResponse(Class<?> clazz) {
		for( Map.Entry<Class<?>, Info> e : table.entrySet() ) {
			if( e.getValue().response.equals(clazz) ) {
				return true;
			}
		}
		return false;
	}
	public NameParcel createResponse(Class<?> clazz, Object obj){
		NameParcel cd = null;
		if( checkResponse( clazz ) ) {
			cd = new NameParcel(clazz, (Parcelable)obj);
		}
		return cd;
	}
	public Info queryClass(CommunicationData cm){
		Class<?> name = cm.name();
		return table.get(name);
	}
	
}
