package com.wind.login;

import android.os.Parcel;
import android.os.Parcelable;

public class ChangeAvatarRequest implements Parcelable {
	public long userId;
	public int imgLen;
	public byte imgcontent[];
	public ChangeAvatarRequest(){
		
	}

    protected ChangeAvatarRequest(Parcel in) {
        userId = in.readInt();
        imgLen = in.readInt();
        imgcontent = new byte[imgLen];
        in.readByteArray(imgcontent);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeLong(userId);
        dest.writeInt(imgLen);
        dest.writeByteArray(imgcontent);
    }

    @SuppressWarnings("unused")
    public static final Parcelable.Creator<ChangeAvatarRequest> CREATOR = new Parcelable.Creator<ChangeAvatarRequest>() {
        @Override
        public ChangeAvatarRequest createFromParcel(Parcel in) {
            return new ChangeAvatarRequest(in);
        }

        @Override
        public ChangeAvatarRequest[] newArray(int size) {
            return new ChangeAvatarRequest[size];
        }
    };
}