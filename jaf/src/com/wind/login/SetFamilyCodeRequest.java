package com.wind.login;

import android.os.Parcel;
import android.os.Parcelable;


public class SetFamilyCodeRequest implements Parcelable {
	public static class Param implements Parcelable {
		public String uid;
		public String familyCode;

		public Param() {
		}

		protected Param(Parcel in) {
			uid = in.readString();
			familyCode = in.readString();
		}

		@Override
		public int describeContents() {
			return 0;
		}

		@Override
		public void writeToParcel(Parcel dest, int flags) {
			dest.writeString(uid);
			dest.writeString(familyCode);
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
	public SetFamilyCodeRequest() {
		
	}
	
    protected SetFamilyCodeRequest(Parcel in) {
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
    public static final Parcelable.Creator<SetFamilyCodeRequest> CREATOR = new Parcelable.Creator<SetFamilyCodeRequest>() {
        @Override
        public SetFamilyCodeRequest createFromParcel(Parcel in) {
            return new SetFamilyCodeRequest(in);
        }

        @Override
        public SetFamilyCodeRequest[] newArray(int size) {
            return new SetFamilyCodeRequest[size];
        }
    };

}
