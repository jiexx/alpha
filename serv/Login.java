package com.jiexx.serv;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.*;
 
import javax.servlet.ServletException;
import javax.servlet.ServletInputStream;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;



public class LoginServlet extends HttpServlet {
	
    public void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
    	String result = "", ;
    	connect();
		try {
			BufferedReader br = new BufferedReader(new InputStreamReader((ServletInputStream) request.getInputStream(), "utf-8"));
			StringBuffer sb =new StringBuffer("");
			String temp;
			LoginCommandAck lca;
			while((temp=br.readLine())!=null){
				sb.append(temp);
			}
			br.close();
			LoginCommand lc = (LoginCommand)Utils.fromStr( sb.toString() );
			Db.connect();
			if( Db.insert("insert into auth(username)values("+lc.username+")") > 0 ) {
				Random random = new Random();
				lca.result = 1;
				lca.key = String.valueOf(Math.abs(random.nextInt())%1000000);
				result = Utils.toStr(lca);
			}else {
				throws Exception("");
			}
		} catch (Exception e) {
			LoginCommandAck lca;
			lca.result = 1;
			lca.key = "";
			result = Utils.toStr(lca);
		} finally {
			Db.close();
			PrintWriter pw = response.getWriter();
			pw.write(result);
			pw.flush();
			pw.close();
		}
    }
}