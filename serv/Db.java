package com.jiexx.serv;

import java.sql.*;

public class Db {
	public static String url = "jdbc:mysql://localhost/vpn";
    public static String username = "vpn";
    public static String password = "aA@686868";
    public static Connection con;
    public static Statement stmt;
    public static ResultSet rs;
    public static void connect() {
        try {
            Class.forName("com.mysql.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        try {
            con = DriverManager.getConnection(url, username, password);
            stmt = con.createStatement();
        } catch(SQLException e) {
        	e.printStackTrace();
        }
    }
	public static void close() {
		stmt.close();
        con.close();
	}
    public static int insert(String sql) {
        try {
            stmt.clearBatch();
            stmt.addBatch(sql);
            stmt.executeBatch();
        }catch (Exception e) {
        	e.printStackTrace();
			return -1;
        }
		return 1;
    }
    public static void update(String sql) {
        try {
            stmt.executeUpdate(sql);
        }catch (Exception e) {
        	e.printStackTrace();
        }
    }
    public static void select(String sql) {
        try {
            rs = stmt.executeQuery(sql);
            ResultSetMetaData meta_data = rs.getMetaData();//col name
            for (int i_col = 1; i_col <= meta_data.getColumnCount(); i_col++) {
                System.out.print(meta_data.getColumnLabel(i_col) + "   ");
            }
            while (rs.next()) {
                for (int i_col = 1; i_col <= meta_data.getColumnCount(); i_col++) {
                    System.out.print(rs.getString(i_col) + "  ");
                }
                System.out.println();
            }
            rs.close();
        }catch (Exception e) {
        	e.printStackTrace();
        }
    }
}