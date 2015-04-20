package com.wind.login;

import android.os.Parcel;
import android.os.Parcelable;


public class ChangeContactRequest implements Parcelable {
	public static class Param implements Parcelable {
		public static class Address implements Parcelable {
			public String addrType;
			public String addrName;
			public String addrLat;
			public String addrLng;

			public Address() {
			}

			protected Address(Parcel in) {
				addrType = in.readString();
				addrName = in.readString();
				addrLat = in.readString();
				addrLng = in.readString();
			}

			@Override
			public int describeContents() {
				return 0;
			}

			@Override
			public void writeToParcel(Parcel dest, int flags) {
				dest.writeString(addrType);
				dest.writeString(addrName);
				dest.writeString(addrLat);
				dest.writeString(addrLng);
			}

			@SuppressWarnings("unused")
			public static final Parcelable.Creator<Address> CREATOR = new Parcelable.Creator<Address>() {
				@Override
				public Address createFromParcel(Parcel in) {
					return new Address(in);
				}

				@Override
				public Address[] newArray(int size) {
					return new Address[size];
				}
			};
		}

		public String uid;
		public Address addressList[];

		public Param() {
		}

		protected Param(Parcel in) {
			uid = in.readString();
			addressList = (Address[]) in.readArray(Address.class.getClassLoader());
		}

		@Override
		public int describeContents() {
			return 0;
		}

		@Override
		public void writeToParcel(Parcel dest, int flags) {
			dest.writeString(uid);
			dest.writeArray(addressList);
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
	public ChangeContactRequest() {
		
	}
		
    protected ChangeContactRequest(Parcel in) {
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
    public static final Parcelable.Creator<ChangeContactRequest> CREATOR = new Parcelable.Creator<ChangeContactRequest>() {
        @Override
        public ChangeContactRequest createFromParcel(Parcel in) {
            return new ChangeContactRequest(in);
        }

        @Override
        public ChangeContactRequest[] newArray(int size) {
            return new ChangeContactRequest[size];
        }
    };
}
