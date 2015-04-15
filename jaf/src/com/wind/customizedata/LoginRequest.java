package com.wind.customizedata;

import android.os.Parcel;
import android.os.Parcelable;


public class LoginRequest implements Parcelable {
	
	public static class Phone implements Parcelable {
		public String phoneNumber;
		public Phone() {
	    }
	
	    protected Phone(Parcel in) {
	        phoneNumber = in.readString();
	    }
	
	    @Override
	    public int describeContents() {
	        return 0;
	    }
	
	    @Override
	    public void writeToParcel(Parcel dest, int flags) {
	        dest.writeString(phoneNumber);
	    }
	
	    @SuppressWarnings("unused")
	    public static final Parcelable.Creator<Phone> CREATOR = new Parcelable.Creator<Phone>() {
	        @Override
	        public Phone createFromParcel(Parcel in) {
	            return new Phone(in);
	        }
	
	        @Override
	        public Phone[] newArray(int size) {
	            return new Phone[size];
	        }
	    };
	}
	public Phone params = new Phone();
	public LoginRequest() {
	}

    protected LoginRequest(Parcel in) {
        params = (Phone) in.readValue(Phone.class.getClassLoader());
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeValue(params);
    }

    @SuppressWarnings("unused")
    public static final Parcelable.Creator<LoginRequest> CREATOR = new Parcelable.Creator<LoginRequest>() {
        @Override
        public LoginRequest createFromParcel(Parcel in) {
            return new LoginRequest(in);
        }

        @Override
        public LoginRequest[] newArray(int size) {
            return new LoginRequest[size];
        }
    };
}