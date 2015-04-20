package com.wind.login;

import android.os.Parcel;
import android.os.Parcelable;

public class RegisterRequest implements Parcelable{
	public static class Param implements Parcelable {
		public String phoneNumber;
		public String verifyCodeMD5;

		public Param() {

		}

		protected Param(Parcel in) {
			phoneNumber = in.readString();
			verifyCodeMD5 = in.readString();
		}

		@Override
		public int describeContents() {
			return 0;
		}

		@Override
		public void writeToParcel(Parcel dest, int flags) {
			dest.writeString(phoneNumber);
			dest.writeString(verifyCodeMD5);
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
    public RegisterRequest() {
    	
    }
    
    protected RegisterRequest(Parcel in) {
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
    public static final Parcelable.Creator<RegisterRequest> CREATOR = new Parcelable.Creator<RegisterRequest>() {
        @Override
        public RegisterRequest createFromParcel(Parcel in) {
            return new RegisterRequest(in);
        }

        @Override
        public RegisterRequest[] newArray(int size) {
            return new RegisterRequest[size];
        }
    };

}
