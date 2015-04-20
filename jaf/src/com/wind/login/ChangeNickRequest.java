package com.wind.login;

import android.os.Parcel;
import android.os.Parcelable;

public class ChangeNickRequest implements Parcelable {
	public static class Param implements Parcelable {
		public String uid;
		public String nickname;

		public Param() {
		}

		protected Param(Parcel in) {
			uid = in.readString();
			nickname = in.readString();
		}

		@Override
		public int describeContents() {
			return 0;
		}

		@Override
		public void writeToParcel(Parcel dest, int flags) {
			dest.writeString(uid);
			dest.writeString(nickname);
		}

		@SuppressWarnings("unused")
		public static final Parcelable.Creator<Param> CREATOR = new Parcelable.Creator<Param>() {
			@Override
			public Param createFromParcel(Parcel in) {
				return new Param(in);
			}

			@Override
			public Param[] newArray(int size) {
				return new Param[size];
			}
		};
	}
	public int appType;
	public Param params = new Param();
	public ChangeNickRequest() {
		
	}
	
    protected ChangeNickRequest(Parcel in) {
        appType = in.readInt();
        params = (Param) in.readValue(Param.class.getClassLoader());
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(appType);
        dest.writeValue(params);
    }

    @SuppressWarnings("unused")
    public static final Parcelable.Creator<ChangeNickRequest> CREATOR = new Parcelable.Creator<ChangeNickRequest>() {
        @Override
        public ChangeNickRequest createFromParcel(Parcel in) {
            return new ChangeNickRequest(in);
        }

        @Override
        public ChangeNickRequest[] newArray(int size) {
            return new ChangeNickRequest[size];
        }
    };

}
