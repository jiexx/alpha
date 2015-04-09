package com.wind.ui;

import java.lang.annotation.*;

/**
  * annotate response method, it will be called after request if result returned by server.
  * 
  * @author jluo
  * @date 2015-4-1
  */
 @Target(ElementType.METHOD)
 @Retention(RetentionPolicy.SOURCE)
 @Documented
 public @interface RECV {
    /*
	 *The name of RECV must be same with POST
	 *@return
	 */
	String value() default "";
 }