# 改library
首先先進入各part的bbcar/bbcar/bbcar.cpp把turn改成
void BBCar::turn( double speed, double factor ){  
    servo0.set_factor(1);  
    servo1.set_factor(1);  
    servo0.set_speed(speed);  
    servo1.set_speed(speed);  

}  

# part1
這題的部分因為encoder會失效，因此事先量好車速再計算需要行走的時間。  
將車子擺好後輸入/RP/run d1 d2 P  
d1 d2為距離單位是公分，d1, d2的用法跟HW4網站上的圖相反。P 輸入方位，ESWN分別代表東南西北。  
![](https://i.imgur.com/z5AXfjb.png)
# part2
這題的部分要注意的是要把車子兩個輪子的中心點擺在線上才有辦法正確的執行程式。
# part3
這題要注意的是若要執行一定要確保開始的時候要照到Apriltag而最好放離Apriltag遠一點  
不然可能會造成結束的時候離apriltag太近照不到。
