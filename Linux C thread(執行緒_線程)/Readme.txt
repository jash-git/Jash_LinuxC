01.example.c-最簡單的thread範例，重點在於如何建立和等待thread結束
02.sum_by_thread.c-重點在於參數傳入thread中+利用全域變數達到thread和主程式資料共用
03.thread_example.c-重點在於利用全域變數達到thread和主程式資料共用+利用pthread_mutex_t達到thread之間的互鎖功能(實作兩個thread一起處理同一份工作)
04.jash_thread.cpp-證明pthread_mutex_t可以同時用在多個thread之間的互鎖功能，但是可能會有同一筆資料重覆運算浪費CPU
05.jash_thread01.cpp-將所有thread函數整合成一個，另外用pthread_self函數來分別是哪一個thread在工作
06.jash_thread02.cpp-成功傳遞變數到thread函數，原來要透過 sleep(1);//延遲線程建立速度,否則無法正確傳遞變數
07.jash_thread03.cpp-成功傳遞變數到thread函數，原來要透過 sleep(1);//延遲線程建立速度,否則無法正確傳遞變數+把for迴圈改成while迴圈防堵重複執行問題

