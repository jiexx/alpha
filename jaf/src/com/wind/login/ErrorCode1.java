package com.wind.login;

import android.os.Parcel;
import android.os.Parcelable;

public class ErrorCode1 implements Parcelable{
	public static class Result implements Parcelable {
		public int uid;

		public Result() {
		}

		protected Result(Parcel in) {
			uid = in.readInt();
		}

		@Override
		public int describeContents() {
			return 0;
		}

		@Override
		public void writeToParcel(Parcel dest, int flags) {
			dest.writeInt(uid);
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
	Result results = new Result();
    public int status;
    public int errCode;
    public ErrorCode1() {
    	
    }
    
    protected ErrorCode1(Parcel in) {
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
        dest.writeValue(results);
        dest.writeInt(status);
        dest.writeInt(errCode);
    }

    @SuppressWarnings("unused")
    public static final Parcelable.Creator<ErrorCode1> CREATOR = new Parcelable.Creator<ErrorCode1>() {
        @Override
        public ErrorCode1 createFromParcel(Parcel in) {
            return new ErrorCode1(in);
        }

        @Override
        public ErrorCode1[] newArray(int size) {
            return new ErrorCode1[size];
        }
    };
}
