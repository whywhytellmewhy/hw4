# hw4
Created on 110.6.3 at home.

# Q1: How to setup and run your program？
A1: 
Part 1：當使用Xbee傳輸RPC指令(伴隨著d_1、d_2值)給車車時，車車計算要往停車格方向前進(若一開始朝東方)或後退(若一開始朝西方)多少距離(即以不同速度前進一秒)，接著以一個輪子向後退、另一個輪子幾乎不動
的方式，來達成轉彎90度，接著再倒車即可成功進入停車格；  
Part 2：先在Openmv IDE上篩選出較下方的端點位在螢幕中間下方的位置的直線(因為將車車放置在職銜上並沿著其方向行走時，理論上直線的底端位置(較靠近車車)會落在鏡頭視野中間正下方處)，這樣一來就可以篩選掉其他偵測到的直線。接著將直線的x_1、x_2、y_1、y_2的數據以uART訊號傳送給車車，車車接收到後可計算出要轉的角度及順/逆時針方向，接著轉向所對應的角度與方向後，即可與直線方向平行。最後再往前衝即可沿著直線方向前進；  
Part 3：先用Openmv IDE偵測出ApriTag的位置以及鏡頭與其中心點的距離及與Tag面之間的視角差，轉動 "90度-視角差" 後，即可與Tag面平行並朝向Tag面所在的方向，接著往前走原鏡頭與Tag中心點的距離，即可到達Tag正前方，最後再將車車轉向90度即可直接面朝Tag。然後再以Ping測量它們之間的距離傳送到screen並print出即可。  

# Q2: What are the results？
A2:
Part 1: 車車成功以倒車方式進入停車區；  
Part 2: 車車成功轉向面朝直線方向(與直線方向平行)，然後沿著直線方向前進；  
Part 3: 車車保持與Tag面之間的垂直距離，行走到正對著Tag面，並在螢幕上print出用Ping測量到的車車與Tag之間的(垂直)距離。
