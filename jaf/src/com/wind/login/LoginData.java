package com.wind.login;

import android.os.Parcel;
import android.os.Parcelable;

public class LoginData implements Parcelable  {
	public static class Result implements Parcelable{
		public static class ServerNode implements Parcelable {
			public int port;
			public String host;
			public String name;

			public ServerNode() {
			}

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
		public static class UserInfo implements Parcelable {
        	public int eid;
        	public String pDomainId;
        	public String userPhone;
        	public int userType;
        	public String userName;
        	public int userID;
        	public int eDomainId;
        	public UserInfo() {
        	}
        
			protected UserInfo(Parcel in) {
				eid = in.readInt();
				pDomainId = in.readString();
				userPhone = in.readString();
				userType = in.readInt();
				userName = in.readString();
				userID = in.readInt();
				eDomainId = in.readInt();
			}

			@Override
			public int describeContents() {
				return 0;
			}

			@Override
			public void writeToParcel(Parcel dest, int flags) {
				dest.writeInt(eid);
				dest.writeString(pDomainId);
				dest.writeString(userPhone);
				dest.writeInt(userType);
				dest.writeString(userName);
				dest.writeInt(userID);
				dest.writeInt(eDomainId);
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
		public UserInfo userInfo = new UserInfo();
		public String IMUserAccount;
	    public String IMPassword;
	    public ServerNode serverNodes[];
	    public long serverTime;
		public String sessionID;
		
	    public Result(){
        }
        
		protected Result(Parcel in) {
			userInfo = (UserInfo) in.readValue(UserInfo.class.getClassLoader());
			IMUserAccount = in.readString();
			IMPassword = in.readString();
			serverNodes = (ServerNode[]) in.readArray(ServerNode[].class.getClassLoader());
			serverTime = in.readLong();
			sessionID = in.readString();
		}

		@Override
		public int describeContents() {
			return 0;
		}

		@Override
		public void writeToParcel(Parcel dest, int flags) {
			dest.writeValue(userInfo);
			dest.writeString(IMUserAccount);
			dest.writeString(IMPassword);
			dest.writeArray(serverNodes);
			dest.writeLong(serverTime);
			dest.writeString(sessionID);
		}

		@SuppressWarnings("unused")
		public static final Parcelable.Creator<Result> CREATOR = new Parcelable.Creator<Result>() {
			@Override
			public Result createFromParcel(Parcel in) {
				return new Result(in);
			}

			@Override
			public Result[] newArray(int size) {
				return new Result[size];
			}
		};
	};
	public int errCode;
	public String type;
	public Result results = new Result();
	public int status;
	
	public LoginData() {
		
	}
        
    protected LoginData(Parcel in) {
    	errCode = in.readInt();
    	type = in.readString();
        results = (Result) in.readValue(Result.class.getClassLoader());
        status = in.readInt();
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
    	dest.writeString(type);
        dest.writeValue(results);
        dest.writeInt(status);
        dest.writeInt(errCode);
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
