#!/bin/bash



# Tetris Game

# 10.21.2003 xhchen<[email]xhchen@winbond.com.tw[/email]>



#APP declaration

APP_NAME="${0##*[\\/]}"

APP_VERSION="1.0"





#顏色定義

cRed=1

cGreen=2

cYellow=3

cBlue=4

cFuchsia=5

cCyan=6

cWhite=7

colorTable=($cRed $cGreen $cYellow $cBlue $cFuchsia $cCyan $cWhite)



#位置和大小

iLeft=3

iTop=2

((iTrayLeft = iLeft + 2))

((iTrayTop = iTop + 1))

((iTrayWidth = 10))

((iTrayHeight = 15))



#顏色設置

cBorder=$cGreen

cScore=$cFuchsia

cScoreValue=$cCyan



#控制信號

#改遊戲使用兩個進程，一個用於接收輸入，一個用於遊戲流程和顯示介面;

#當前者接收到上下左右等按鍵時，通過向後者發送signal的方式通知後者。

sigRotate=25

sigLeft=26

sigRight=27

sigDown=28

sigAllDown=29

sigExit=30



#七中不同的方塊的定義

#通過旋轉，每種方塊的顯示的樣式可能有幾種

box0=(0 0 0 1 1 0 1 1)

box1=(0 2 1 2 2 2 3 2 1 0 1 1 1 2 1 3)

box2=(0 0 0 1 1 1 1 2 0 1 1 0 1 1 2 0)

box3=(0 1 0 2 1 0 1 1 0 0 1 0 1 1 2 1)

box4=(0 1 0 2 1 1 2 1 1 0 1 1 1 2 2 2 0 1 1 1 2 0 2 1 0 0 1 0 1 1 1 2)

box5=(0 1 1 1 2 1 2 2 1 0 1 1 1 2 2 0 0 0 0 1 1 1 2 1 0 2 1 0 1 1 1 2)

box6=(0 1 1 1 1 2 2 1 1 0 1 1 1 2 2 1 0 1 1 0 1 1 2 1 0 1 1 0 1 1 1 2)

#所有其中方塊的定義都放到box變數中

box=(${box0[@]} ${box1[@]} ${box2[@]} ${box3[@]} ${box4[@]} ${box5[@]} ${box6[@]})

#各種方塊旋轉後可能的樣式數目

countBox=(1 2 2 2 4 4 4)

#各種方塊再box陣列中的偏移

offsetBox=(0 1 3 5 7 11 15)



#每提高一個速度級需要積累的分數

iScoreEachLevel=50	#be greater than 7



#運行時資料

sig=0		#接收到的signal

iScore=0	#總分

iLevel=0	#速度級

boxNew=()	#新下落的方塊的位置定義

cBoxNew=0	#新下落的方塊的顏色

iBoxNewType=0	#新下落的方塊的種類

iBoxNewRotate=0	#新下落的方塊的旋轉角度

boxCur=()	#當前方塊的位置定義

cBoxCur=0	#當前方塊的顏色

iBoxCurType=0	#當前方塊的種類

iBoxCurRotate=0	#當前方塊的旋轉角度

boxCurX=-1	#當前方塊的x座標位置

boxCurY=-1	#當前方塊的y座標位置

iMap=()		#背景方塊圖表



#初始化所有背景方塊為-1, 表示沒有方塊

for ((i = 0; i < iTrayHeight * iTrayWidth; i++)); do iMap[$i]=-1; done





#接收輸入的進程的主函數

function RunAsKeyReceiver()

{

	local pidDisplayer key aKey sig cESC sTTY



	pidDisplayer=$1

	aKey=(0 0 0)



	cESC=`echo -ne "\033"`

	cSpace=`echo -ne "\040"`



	#保存終端屬性。在read -s讀取終端鍵時，終端的屬性會被暫時改變。

	#如果在read -s時程式被不幸殺掉，可能會導致終端混亂，

	#需要在程式退出時恢復終端屬性。

	sTTY=`stty -g`



	#捕捉退出信號

	trap "MyExit;" INT TERM

	trap "MyExitNoSub;" $sigExit



	#隱藏游標

	echo -ne "\033[?25l"





	while :

	do

		#讀取輸入。注-s不回顯，-n讀到一個字元立即返回

		read -s -n 1 key



		aKey[0]=${aKey[1]}

		aKey[1]=${aKey[2]}

		aKey[2]=$key

		sig=0



		#判斷輸入了何種鍵

		if [[ $key == $cESC && ${aKey[1]} == $cESC ]]

		then

			#ESC鍵

			MyExit

		elif [[ ${aKey[0]} == $cESC && ${aKey[1]} == "[" ]]

		then

			if [[ $key == "A" ]]; then sig=$sigRotate	#<向上鍵>

			elif [[ $key == "B" ]]; then sig=$sigDown	#<向下鍵>

			elif [[ $key == "D" ]]; then sig=$sigLeft	#<向左鍵>

			elif [[ $key == "C" ]]; then sig=$sigRight	#<向右鍵>

			fi

		elif [[ $key == "W" || $key == "w" ]]; then sig=$sigRotate	#W, w

		elif [[ $key == "S" || $key == "s" ]]; then sig=$sigDown	#S, s

		elif [[ $key == "A" || $key == "a" ]]; then sig=$sigLeft	#A, a

		elif [[ $key == "D" || $key == "d" ]]; then sig=$sigRight	#D, d

		elif [[ "[$key]" == "[]" ]]; then sig=$sigAllDown	#空白鍵

		elif [[ $key == "Q" || $key == "q" ]]			#Q, q

		then

			MyExit

		fi



		if [[ $sig != 0 ]]

		then

			#向另一進程發送消息

			kill -$sig $pidDisplayer

		fi

	done

}



#退出前的恢復

function MyExitNoSub()

{

	local y



	#恢復終端屬性

	stty $sTTY

	((y = iTop + iTrayHeight + 4))



	#顯示游標

	echo -e "\033[?25h\033[${y};0H"

	exit

}





function MyExit()

{

	#通知顯示進程需要退出

	kill -$sigExit $pidDisplayer



	MyExitNoSub

}





#處理顯示和遊戲流程的主函數

function RunAsDisplayer()

{

	local sigThis

	InitDraw



	#掛載各種信號的處理函數

	trap "sig=$sigRotate;" $sigRotate

	trap "sig=$sigLeft;" $sigLeft

	trap "sig=$sigRight;" $sigRight

	trap "sig=$sigDown;" $sigDown

	trap "sig=$sigAllDown;" $sigAllDown

	trap "ShowExit;" $sigExit



	while :

	do

		#根據當前的速度級iLevel不同，設定相應的迴圈的次數

		for ((i = 0; i < 21 - iLevel; i++))

		do

			sleep 0.02

			sigThis=$sig

			sig=0



			#根據sig變數判斷是否接受到相應的信號

			if ((sigThis == sigRotate)); then BoxRotate;	#旋轉

			elif ((sigThis == sigLeft)); then BoxLeft;	#左移一列

			elif ((sigThis == sigRight)); then BoxRight;	#右移一列

			elif ((sigThis == sigDown)); then BoxDown;	#下落一行

			elif ((sigThis == sigAllDown)); then BoxAllDown;	#下落到底

			fi

		done

		#kill -$sigDown $$

		BoxDown	#下落一行

	done

}





#BoxMove(y, x), 測試是否可以把移動中的方塊移到(x, y)的位置, 返回0則可以, 1不可以

function BoxMove()

{

	local j i x y xTest yTest

	yTest=$1

	xTest=$2

	for ((j = 0; j < 8; j += 2))

	do

		((i = j + 1))

		((y = ${boxCur[$j]} + yTest))

		((x = ${boxCur[$i]} + xTest))

		if (( y < 0 || y >= iTrayHeight || x < 0 || x >= iTrayWidth))

		then

			#撞到牆壁了

			return 1

		fi

		if ((${iMap[y * iTrayWidth + x]} != -1 ))

		then

			#撞到其他已經存在的方塊了

			return 1

		fi

	done

	return 0;

}





#將當前移動中的方塊放到背景方塊中去,

#並計算新的分數和速度級。(即一次方塊落到底部)

function Box2Map()

{

	local j i x y xp yp line



	#將當前移動中的方塊放到背景方塊中去

	for ((j = 0; j < 8; j += 2))

	do

		((i = j + 1))

		((y = ${boxCur[$j]} + boxCurY))

		((x = ${boxCur[$i]} + boxCurX))

		((i = y * iTrayWidth + x))

		iMap[$i]=$cBoxCur

	done



	#消去可被消去的行

	line=0

	for ((j = 0; j < iTrayWidth * iTrayHeight; j += iTrayWidth))

	do

		for ((i = j + iTrayWidth - 1; i >= j; i--))

		do

			if ((${iMap[$i]} == -1)); then break; fi

		done

		if ((i >= j)); then continue; fi



		((line++))

		for ((i = j - 1; i >= 0; i--))

		do

			((x = i + iTrayWidth))

			iMap[$x]=${iMap[$i]}

		done

		for ((i = 0; i < iTrayWidth; i++))

		do

			iMap[$i]=-1

		done

	done



	if ((line == 0)); then return; fi



	#根據消去的行數line計算分數和速度級

	((x = iLeft + iTrayWidth * 2 + 7))

	((y = iTop + 11))

	((iScore += line * 2 - 1))

	#顯示新的分數

	echo -ne "\033[1m\033[3${cScoreValue}m\033[${y};${x}H${iScore}         "

	if ((iScore % iScoreEachLevel < line * 2 - 1))

	then

		if ((iLevel < 20))

		then

			((iLevel++))

			((y = iTop + 14))

			#顯示新的速度級

			echo -ne "\033[3${cScoreValue}m\033[${y};${x}H${iLevel}        "

		fi

	fi

	echo -ne "\033[0m"





	#重新顯示背景方塊

	for ((y = 0; y < iTrayHeight; y++))

	do

		((yp = y + iTrayTop + 1))

		((xp = iTrayLeft + 1))

		((i = y * iTrayWidth))

		echo -ne "\033[${yp};${xp}H"

		for ((x = 0; x < iTrayWidth; x++))

		do

			((j = i + x))

			if ((${iMap[$j]} == -1))

			then

				echo -ne "  "

			else

				echo -ne "\033[1m\033[7m\033[3${iMap[$j]}m\033[4${iMap[$j]}m[]\033[0m"

			fi

		done

	done

}





#下落一行

function BoxDown()

{

	local y s

	((y = boxCurY + 1))	#新的y座標

	if BoxMove $y $boxCurX	#測試是否可以下落一行

	then

		s="`DrawCurBox 0`"	#將舊的方塊抹去

		((boxCurY = y))

		s="$s`DrawCurBox 1`"	#顯示新的下落後方塊

		echo -ne $s

	else

		#走到這兒, 如果不能下落了

		Box2Map		#將當前移動中的方塊貼到背景方塊中

		RandomBox	#產生新的方塊

	fi

}



#左移一列

function BoxLeft()

{

	local x s

	((x = boxCurX - 1))

	if BoxMove $boxCurY $x

	then

		s=`DrawCurBox 0`

		((boxCurX = x))

		s=$s`DrawCurBox 1`

		echo -ne $s

	fi

}



#右移一列

function BoxRight()

{

	local x s

	((x = boxCurX + 1))

	if BoxMove $boxCurY $x

	then

		s=`DrawCurBox 0`

		((boxCurX = x))

		s=$s`DrawCurBox 1`

		echo -ne $s

	fi

}





#下落到底

function BoxAllDown()

{

	local k j i x y iDown s

	iDown=$iTrayHeight



	#計算一共需要下落多少行

	for ((j = 0; j < 8; j += 2))

	do

		((i = j + 1))

		((y = ${boxCur[$j]} + boxCurY))

		((x = ${boxCur[$i]} + boxCurX))

		for ((k = y + 1; k < iTrayHeight; k++))

		do

			((i = k * iTrayWidth + x))

			if (( ${iMap[$i]} != -1)); then break; fi

		done

		((k -= y + 1))

		if (( $iDown > $k )); then iDown=$k; fi

	done



	s=`DrawCurBox 0`	#將舊的方塊抹去

	((boxCurY += iDown))

	s=$s`DrawCurBox 1`	#顯示新的下落後的方塊

	echo -ne $s

	Box2Map		#將當前移動中的方塊貼到背景方塊中

	RandomBox	#產生新的方塊

}





#旋轉方塊

function BoxRotate()

{

	local iCount iTestRotate boxTest j i s

	iCount=${countBox[$iBoxCurType]}	#當前的方塊經旋轉可以產生的樣式的數目



	#計算旋轉後的新的樣式

	((iTestRotate = iBoxCurRotate + 1))

	if ((iTestRotate >= iCount))

	then

		((iTestRotate = 0))

	fi



	#更新到新的樣式, 保存老的樣式(但不顯示)

	for ((j = 0, i = (${offsetBox[$iBoxCurType]} + $iTestRotate) * 8; j < 8; j++, i++))

	do

		boxTest[$j]=${boxCur[$j]}

		boxCur[$j]=${box[$i]}

	done



	if BoxMove $boxCurY $boxCurX	#測試旋轉後是否有空間放的下

	then

		#抹去舊的方塊

		for ((j = 0; j < 8; j++))

		do

			boxCur[$j]=${boxTest[$j]}

		done

		s=`DrawCurBox 0`



		#畫上新的方塊

		for ((j = 0, i = (${offsetBox[$iBoxCurType]} + $iTestRotate) * 8; j < 8; j++, i++))

		do

			boxCur[$j]=${box[$i]}

		done

		s=$s`DrawCurBox 1`

		echo -ne $s

		iBoxCurRotate=$iTestRotate

	else

		#不能旋轉，還是繼續使用老的樣式

		for ((j = 0; j < 8; j++))

		do

			boxCur[$j]=${boxTest[$j]}

		done

	fi

}





#DrawCurBox(bDraw), 繪製當前移動中的方塊, bDraw為1, 畫上, bDraw為0, 抹去方塊。

function DrawCurBox()

{

	local i j t bDraw sBox s

	bDraw=$1



	s=""

	if (( bDraw == 0 ))

	then

		sBox="\040\040"

	else

		sBox="[]"

		s=$s"\033[1m\033[7m\033[3${cBoxCur}m\033[4${cBoxCur}m"

	fi



	for ((j = 0; j < 8; j += 2))

	do

		((i = iTrayTop + 1 + ${boxCur[$j]} + boxCurY))

		((t = iTrayLeft + 1 + 2 * (boxCurX + ${boxCur[$j + 1]})))

		#\033[y;xH, 游標到(x, y)處

		s=$s"\033[${i};${t}H${sBox}"

	done

	s=$s"\033[0m"

	echo -n $s

}





#更新新的方塊

function RandomBox()

{

	local i j t



	#更新當前移動的方塊

	iBoxCurType=${iBoxNewType}

	iBoxCurRotate=${iBoxNewRotate}

	cBoxCur=${cBoxNew}

	for ((j = 0; j < ${#boxNew[@]}; j++))

	do

		boxCur[$j]=${boxNew[$j]}

	done





	#顯示當前移動的方塊

	if (( ${#boxCur[@]} == 8 ))

	then

		#計算當前方塊該從頂端哪一行"冒"出來

		for ((j = 0, t = 4; j < 8; j += 2))

		do

			if ((${boxCur[$j]} < t)); then t=${boxCur[$j]}; fi

		done

		((boxCurY = -t))

		for ((j = 1, i = -4, t = 20; j < 8; j += 2))

		do

			if ((${boxCur[$j]} > i)); then i=${boxCur[$j]}; fi

			if ((${boxCur[$j]} < t)); then t=${boxCur[$j]}; fi

		done

		((boxCurX = (iTrayWidth - 1 - i - t) / 2))



		#顯示當前移動的方塊

		echo -ne `DrawCurBox 1`



		#如果方塊一出來就沒處放，Game over!

		if ! BoxMove $boxCurY $boxCurX

		then

			kill -$sigExit ${PPID}

			ShowExit

		fi

	fi







	#清除右邊預顯示的方塊

	for ((j = 0; j < 4; j++))

	do

		((i = iTop + 1 + j))

		((t = iLeft + 2 * iTrayWidth + 7))

		echo -ne "\033[${i};${t}H        "

	done



	#隨機產生新的方塊

	((iBoxNewType = RANDOM % ${#offsetBox[@]}))

	((iBoxNewRotate = RANDOM % ${countBox[$iBoxNewType]}))

	for ((j = 0, i = (${offsetBox[$iBoxNewType]} + $iBoxNewRotate) * 8; j < 8; j++, i++))

	do

		boxNew[$j]=${box[$i]};

	done



	((cBoxNew = ${colorTable[RANDOM % ${#colorTable[@]}]}))



	#顯示右邊預顯示的方塊

	echo -ne "\033[1m\033[7m\033[3${cBoxNew}m\033[4${cBoxNew}m"

	for ((j = 0; j < 8; j += 2))

	do

		((i = iTop + 1 + ${boxNew[$j]}))

		((t = iLeft + 2 * iTrayWidth + 7 + 2 * ${boxNew[$j + 1]}))

		echo -ne "\033[${i};${t}H[]"

	done

	echo -ne "\033[0m"

}





#初始繪製

function InitDraw()

{

	clear

	RandomBox	#隨機產生方塊，這時右邊預顯示視窗中有方快了

	RandomBox	#再隨機產生方塊，右邊預顯示視窗中的方塊被更新，原先的方塊將開始下落

	local i t1 t2 t3



	#顯示邊框

	echo -ne "\033[1m"

	echo -ne "\033[3${cBorder}m\033[4${cBorder}m"



	((t2 = iLeft + 1))

	((t3 = iLeft + iTrayWidth * 2 + 3))

	for ((i = 0; i < iTrayHeight; i++))

	do

		((t1 = i + iTop + 2))

		echo -ne "\033[${t1};${t2}H||"

		echo -ne "\033[${t1};${t3}H||"

	done



	((t2 = iTop + iTrayHeight + 2))

	for ((i = 0; i < iTrayWidth + 2; i++))

	do

		((t1 = i * 2 + iLeft + 1))

		echo -ne "\033[${iTrayTop};${t1}H=="

		echo -ne "\033[${t2};${t1}H=="

	done

	echo -ne "\033[0m"





	#顯示"Score"和"Level"字樣

	echo -ne "\033[1m"

	((t1 = iLeft + iTrayWidth * 2 + 7))

	((t2 = iTop + 10))

	echo -ne "\033[3${cScore}m\033[${t2};${t1}HScore"

	((t2 = iTop + 11))

	echo -ne "\033[3${cScoreValue}m\033[${t2};${t1}H${iScore}"

	((t2 = iTop + 13))

	echo -ne "\033[3${cScore}m\033[${t2};${t1}HLevel"

	((t2 = iTop + 14))

	echo -ne "\033[3${cScoreValue}m\033[${t2};${t1}H${iLevel}"

	echo -ne "\033[0m"

}





#退出時顯示GameOVer!

function ShowExit()

{

	local y

	((y = iTrayHeight + iTrayTop + 3))

	echo -e "\033[${y};0HGameOver!\033[0m"

	exit

}





#顯示用法.

function Usage

{

	cat << EOF

Usage: $APP_NAME

Start tetris game.



  -h, --help              display this help and exit

      --version           output version information and exit

EOF

}





#遊戲主程序在這兒開始.

if [[ "$1" == "-h" || "$1" == "--help" ]]; then

	Usage

elif [[ "$1" == "--version" ]]; then

	echo "$APP_NAME $APP_VERSION"

elif [[ "$1" == "--show" ]]; then

	#當發現具有參數--show時，運行顯示函數

	RunAsDisplayer

else

	bash $0 --show&	#以參數--show將本程式再運行一遍

	RunAsKeyReceiver $!	#以上一行產生的進程的進程號作為參數

fi

