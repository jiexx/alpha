         JCodeModel cm = new JCodeModel(); 
         JType type = cm.parseType("Singleton");									
         File destDir = new File("src"); 
         JDefinedClass dc = cm._class("dw.sample.Singleton"); 							public class Singleton { 
         // 定义静态成员变量
         dc.field(JMod.PRIVATE+ JMod.STATIC, type, "instance"); 							private static Singleton instance; 
         // 定义单例类 Singleton 的构造函数
         dc.constructor(JMod.PRIVATE); 														private Singleton() { } 

         // 生成 Singleton 类的成员方法 getInstanceMethod 
         JMethod getInstanceMethod = dc.method(JMod.PUBLIC+ JMod.STATIC, type, 				public static Singleton getInstance()
"getInstance"); 
		 JBlock getInstanceBody = getInstanceMethod.body(); 								{
         JFieldRef fieldRef = JExpr.ref("instance"); 											
		 JConditional conditionIf = getInstanceBody._if(fieldRef.eq(JExpr 						if(instance== null)
 ._null())); 
		 JBlock thenPart = conditionIf._then(); 												{ 
         thenPart.assign(fieldRef, JExpr._new(type)); 												instance= new Singleton(); }
		 getInstanceBody._return(fieldRef); 														return instance; }
         // 生成 Singleton 类的成员方法 sayHelloMethod 										/* This method will say Hello to the name.*/ 	
         JMethod sayHelloMethod = dc.method(JMod.PUBLIC, cm.parseType("void"), 				public void sayHello(
"sayHello"); 
         // 生成方法级的 javadoc 
         sayHelloMethod.javadoc().add("This method will say Hello to the name."); 	
		 JBlock sayHelloBody = sayHelloMethod.body(); 
         sayHelloMethod.param(cm.parseType("String"), "name"); 										String name) {
         JClass sys = cm.ref("java.lang.System"); 															System.
         JFieldRef ot = sys.staticRef("out"); 																	out.
         JExpression sentance1 = JExpr.lit("Hello ").invoke("concat").arg(JExpr.ref("name")); 							"Hello ".concat(name).concat("1")
         JExpression sentance2 = sentance1.invoke("concat").arg("!"); 
         sayHelloBody.invoke(ot, "println").arg(sentance2); 													println
		 cm.build(destDir); 
	 } 
	 
	 
	 http://www.ibm.com/developerworks/cn/java/j-lo-codemodel/index.html
	 
	 @ACTIVTY(layout.id)
public  MainActivity {


@POST("book")
String request1() {
}
@RECV("book")
void response1( Data d) {
   findViewById().setText(d.name);
}

@POST("login")
String request2() {
}
@RECV("login")
void response2( Data d) {
   logined = true;
}

public void onCreate(Bundle savedInstanceState){
   response2()
}

void click(){
   request1();
   
}

}


public  MainActivity_ extends MainActivity  {
 @Override
    public void onCreate(Bundle savedInstanceState)
    {
         super.onCreate(savedInstanceState);
         setContentView(layout.id);
    }



private String request1() {
    String url = super.request1();
    request1Delegate dl = new request1Delegate ();
    sky.post(url, dl); 
}

public class request1Delegate extends SkyXXXX {
    void onReceive( char[] buf ){
          Message msg = new Message();
          msg.what = "book";
          msg.data = buf;
          handler.sendMessage( handler.obtainMessage() ); 
          
    }
}



private Handler handler= new Handler(){      
    	public void handleMessage(Message msg) {
    	    switch( msg.what) {
            case "book":
                 super.response1(msg.data);
            case "login":
                 super.response2(msg.data);
            }
    	}
     };
}

	 
	 
	 
	 
	 
	 
	 
	 
	 



@ACTIVTY(layout.id)
public  MainActivity {


@POST("book")
String request1() {
}
@RECV("book")
void response1( Data d) {
   findViewById().setText(d.name);
}

@POST("login")
String request2() {
}
@RECV("login")
void response2( Data d) {
   logined = true;
}

public void onCreate(Bundle savedInstanceState){
   response2()
}

void click(){
   request1();
   
}

}


public  MainActivity_ extends MainActivity  {
 @Override
    public void onCreate(Bundle savedInstanceState)
    {
         super.onCreate(savedInstanceState);
         setContentView(layout.id);
    }



private request1( Data d) {
    String url = super.request1();
    Delegate dl = new response1();
    sky.post(url, dl); 
}

public class response1 extends SkyXXXX {
    void onReceive( char[] buf ){
          Message msg = handler.obtainMessage();
          msg.what = "book";
          msg.data = buf;
          handler.sendMessage( handler.obtainMessage() ); 
          
    }
}



private Handler handler= new Handler(){      
    	public void handleMessage(Message msg) {
    	    switch( msg.what) {
            case "book":
                 super.response1(msg.data);
            case "login":
                 super.response2(msg.data);
            }
    	}
     };
}
