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
        	public int userID;
        	public String userName;
        	public String userPhone;
        	public int appType;
        	public UserInfo() {
        	}
        
			protected UserInfo(Parcel in) {
				userID = in.readInt();
				userName = in.readString();
				userPhone = in.readString();
				appType = in.readInt();
			}

			@Override
			public int describeContents() {
				return 0;
			}

			@Override
			public void writeToParcel(Parcel dest, int flags) {
				dest.writeInt(userID);
				dest.writeString(userName);
				dest.writeString(userPhone);
				dest.writeInt(appType);
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
		public String sessionID;
    	public long serverTime;
        public UserInfo userInfo = new UserInfo();
	    public ServerNode serverNodes[];
	    public String IMUserAccount;
	    public String IMPassword;
	    public Result(){
        }
        
		protected Result(Parcel in) {
			sessionID = in.readString();
			serverTime = in.readLong();
			userInfo = (UserInfo) in.readValue(UserInfo.class.getClassLoader());
			serverNodes = (ServerNode[]) in.readArray(ServerNode.class.getClassLoader());
			IMUserAccount = in.readString();
			IMPassword = in.readString();
		}

		@Override
		public int describeContents() {
			return 0;
		}

		@Override
		public void writeToParcel(Parcel dest, int flags) {
			dest.writeString(sessionID);
			dest.writeLong(serverTime);
			dest.writeValue(userInfo);
			dest.writeArray(serverNodes);
			dest.writeString(IMUserAccount);
			dest.writeString(IMPassword);
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
	public String type;
	public Result results = new Result();
	public int status;
	public int errCode;
	public LoginData() {
		
	}
        
    protected LoginData(Parcel in) {
    	type = in.readString();
        results = (Result) in.readValue(Result.class.getClassLoader());
        status = in.readInt();
        errCode = in.readInt();
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
