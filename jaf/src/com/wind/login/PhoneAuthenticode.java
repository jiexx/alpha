package com.wind.login;

import android.os.Parcel;
import android.os.Parcelable;


public class PhoneAuthenticode implements Parcelable{
	public static class Result implements Parcelable {
		public String code;
		public String verifyCode;
		public int isRegistered;

		public Result() {
		}

		protected Result(Parcel in) {
			code = in.readString();
			verifyCode = in.readString();
			isRegistered = in.readInt();
		}

		@Override
		public int describeContents() {
			return 0;
		}

		@Override
		public void writeToParcel(Parcel dest, int flags) {
			dest.writeString(code);
			dest.writeString(verifyCode);
			dest.writeInt(isRegistered);
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
	}
	public int errCode;
	public Result results = new Result();
	public int status;
    public PhoneAuthenticode() {	
    }

    protected PhoneAuthenticode(Parcel in) {
    	errCode = in.readInt();
        results = (Result) in.readValue(Result.class.getClassLoader());
        status = in.readInt();
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
    	dest.writeInt(errCode);
        dest.writeValue(results);
        dest.writeInt(status);
    }

    @SuppressWarnings("unused")
    public static final Parcelable.Creator<PhoneAuthenticode> CREATOR = new Parcelable.Creator<PhoneAuthenticode>() {
        @Override
        public PhoneAuthenticode createFromParcel(Parcel in) {
            return new PhoneAuthenticode(in);
        }

        @Override
        public PhoneAuthenticode[] newArray(int size) {
            return new PhoneAuthenticode[size];
        }
    };
}
