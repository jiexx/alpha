package com.wind.customizedata;

import android.os.Parcel;
import android.os.Parcelable;

public class LoginData implements Parcelable {
	public int errCode;
	public int status;
	public LoginResult results;
	
	public static class LoginResult implements Parcelable {
		public String IMUserAccount;
		public String IMPassword;
		public int resumeImportMode;
		public long serverTime;
		public String sessionID;
		public int pushMessageMode;
		public UserInfo userInfo;
		public ServerNode[] serverNodes;
	
	    protected LoginResult(Parcel in) {
	        IMUserAccount = in.readString();
	        IMPassword = in.readString();
	        resumeImportMode = in.readInt();
	        serverTime = in.readLong();
	        sessionID = in.readString();
	        pushMessageMode = in.readInt();
	        userInfo = (UserInfo) in.readValue(UserInfo.class.getClassLoader());
	        serverNodes = (ServerNode[]) in.readArray(ServerNode.class.getClassLoader());
	    }
	
	    @Override
	    public int describeContents() {
	        return 0;
	    }
	
	    @Override
	    public void writeToParcel(Parcel dest, int flags) {
	        dest.writeString(IMUserAccount);
	        dest.writeString(IMPassword);
	        dest.writeInt(resumeImportMode);
	        dest.writeLong(serverTime);
	        dest.writeString(sessionID);
	        dest.writeInt(pushMessageMode);
	        dest.writeValue(userInfo);
	        dest.writeArray(serverNodes);
	    }
	
	    @SuppressWarnings("unused")
	    public static final Parcelable.Creator<LoginResult> CREATOR = new Parcelable.Creator<LoginResult>() {
	        @Override
	        public LoginResult createFromParcel(Parcel in) {
	            return new LoginResult(in);
	        }
	
	        @Override
	        public LoginResult[] newArray(int size) {
	            return new LoginResult[size];
	        }
	    };
	}
		
	public static class UserInfo implements Parcelable {
		public int pDomainId;
		public int userID;
		public String eid;
		public String eDomainId;
		public String userName;
		public String userPhone;
		public int userType;
	
	    protected UserInfo(Parcel in) {
	        pDomainId = in.readInt();
	        userID = in.readInt();
	        eid = in.readString();
	        eDomainId = in.readString();
	        userName = in.readString();
	        userPhone = in.readString();
	        userType = in.readInt();
	    }
	
	    @Override
	    public int describeContents() {
	        return 0;
	    }
	
	    @Override
	    public void writeToParcel(Parcel dest, int flags) {
	        dest.writeInt(pDomainId);
	        dest.writeInt(userID);
	        dest.writeString(eid);
	        dest.writeString(eDomainId);
	        dest.writeString(userName);
	        dest.writeString(userPhone);
	        dest.writeInt(userType);
	    }
	
	    @SuppressWarnings("unused")
	    public static final Parcelable.Creator<UserInfo> CREATOR = new Parcelable.Creator<UserInfo>() {
	        @Override
	        public UserInfo createFromParcel(Parcel in) {
	            return new UserInfo(in);
	        }
	
	        @Override
	        public UserInfo[] newArray(int size) {
	            return new UserInfo[size];
	        }
	    };
	}
	
	public static class ServerNode implements Parcelable {
		public int port;
		public String host;
		public String name;
	
	    protected ServerNode(Parcel in) {
	        port = in.readInt();
	        host = in.readString();
	        name = in.readString();
	    }
	
	    @Override
	    public int describeContents() {
	        return 0;
	    }
	
	    @Override
	    public void writeToParcel(Parcel dest, int flags) {
	        dest.writeInt(port);
	        dest.writeString(host);
	        dest.writeString(name);
	    }
	
	    @SuppressWarnings("unused")
	    public static final Parcelable.Creator<ServerNode> CREATOR = new Parcelable.Creator<ServerNode>() {
	        @Override
	        public ServerNode createFromParcel(Parcel in) {
	            return new ServerNode(in);
	        }
	
	        @Override
	        public ServerNode[] newArray(int size) {
	            return new ServerNode[size];
	        }
	    };
	}

    protected LoginData(Parcel in) {
        errCode = in.readInt();
        status = in.readInt();
        results = (LoginResult) in.readValue(LoginResult.class.getClassLoader());
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(errCode);
        dest.writeInt(status);
        dest.writeValue(results);
    }

    @SuppressWarnings("unused")
    public static final Parcelable.Creator<LoginData> CREATOR = new Parcelable.Creator<LoginData>() {
        @Override
        public LoginData createFromParcel(Parcel in) {
            return new LoginData(in);
        }

        @Override
        public LoginData[] newArray(int size) {
            return new LoginData[size];
        }
    };
}
