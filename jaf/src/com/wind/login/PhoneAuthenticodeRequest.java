package com.wind.login;

import android.os.Parcel;
import android.os.Parcelable;


public class PhoneAuthenticodeRequest implements Parcelable{
	public static class Param implements Parcelable {
		public String phoneNumber;
		public int smsFlag;

		public Param() {
		}

		protected Param(Parcel in) {
			phoneNumber = in.readString();
			smsFlag = in.readInt();
		}

		@Override
		public int describeContents() {
			return 0;
		}

		@Override
		public void writeToParcel(Parcel dest, int flags) {
			dest.writeString(phoneNumber);
			dest.writeInt(smsFlag);
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
    public PhoneAuthenticodeRequest() {	
    }

    protected PhoneAuthenticodeRequest(Parcel in) {
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
    public static final Parcelable.Creator<PhoneAuthenticodeRequest> CREATOR = new Parcelable.Creator<PhoneAuthenticodeRequest>() {
        @Override
        public PhoneAuthenticodeRequest createFromParcel(Parcel in) {
            return new PhoneAuthenticodeRequest(in);
        }

        @Override
        public PhoneAuthenticodeRequest[] newArray(int size) {
            return new PhoneAuthenticodeRequest[size];
        }
    };
}
