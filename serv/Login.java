import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.ServletException;

import org.json.JSONObject;
import org.json.JSONTokener;

import java.io.IOException;
import java.io.PrintWriter;

import java.sql.*;

public class LoginServlet extends HttpServlet {
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
    public static void insert(String sql) {
        try {
            stmt.clearBatch();
            stmt.addBatch(sql);
            stmt.executeBatch();
        }catch (Exception e) {
        	e.printStackTrace();
        }
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
    
    public final static Data fromStr( String json ) throws Exception {
		String str = Utils.decodeDES(json.getBytes());
		JSONTokener parser = new JSONTokener( str );
		JSONObject obj = (JSONObject) parser.nextValue();
		if( obj != null ) {
			String cmd = obj.getString("name");
			String val = obj.getString("value");
			if( cmd != null && !cmd.equals("") && val != null && !val.equals("") ) {
				Class<?> clazz = Class.forName(cmd+"Ack");
				if( clazz != null ) {
					Object o = clazz.newInstance();
					Data d = new Data((Command) o);
					d.unserialize(val);
					return d;
				}
			}
		}
		return null;
	}
    public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
    	String result = "";
    	connect();
		try {
			BufferedReader br = new BufferedReader(
					new InputStreamReader((ServletInputStream) request.getInputStream(), "utf-8"));
			StringBuffer sb =new StringBuffer("");
			String temp;
			while((temp=br.readLine())!=null){
				sb.append(temp);
			}
			br.close();
			result = sb.toString();
			
		} catch (Exception e) {
			result = "{err:\"error\"}";
		} finally {
			stmt.close();
	        con.close();
			PrintWriter pw = response.getWriter();
			pw.write(result);
			pw.flush();
			pw.close();
		}
    }
}