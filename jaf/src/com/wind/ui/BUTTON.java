package com.wind.ui;

import java.lang.annotation.*;

/**
  * annotate request method, it can be called anywhere in class.
  * 
  * @author jluo
  * @date 2015-4-1
  */
 @Target(ElementType.METHOD)
 @Retention(RetentionPolicy.SOURCE)
 @Documented
 public @interface BUTTON {
    /*
	 *The name of POST must be same with RECV
	 *@return
	 */
	 int value() default 0;
	 
 }