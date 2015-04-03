package com.example.jafdemo;

import org.androidannotations.annotations.EActivity;
import org.androidannotations.annotations.ItemClick;
import org.androidannotations.annotations.ItemLongClick;
import org.androidannotations.annotations.ItemSelect;
import org.androidannotations.annotations.res.StringArrayRes;

import android.app.ListActivity;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListAdapter;
import android.widget.Toast;

// Ä¬ÈÏlayout
@EActivity
public class TestActivity extends ListActivity {

	String[] items = {"11111", "222222","33333"};

	private ListAdapter adapter;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, items);
		setListAdapter(adapter);
	}

	@ItemClick
	void listItemClicked(String i) {
		Toast.makeText(this, "click: " + i, Toast.LENGTH_SHORT).show();
	}

	@ItemLongClick
	void listItemLongClicked(String i) {
		Toast.makeText(this, "long click: " + i, Toast.LENGTH_SHORT).show();
	}


}
