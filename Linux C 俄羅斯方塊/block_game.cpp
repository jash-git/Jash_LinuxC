/*
出處：http://blog.csdn.net/simonjay2007/article/details/7345078
g++ block_game.cpp -o block_game -lpthread
*/
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <time.h>

#include<termios.h>
#include<fcntl.h>


#define TABLE_SIZE 20
#define BLOCK_SIZE 4
#define SLEEP_TIME 500

using namespace std;

struct grid{int x; int y;};        //座標

/////////////////////Block 類//////////////////////
class Block
{
public:
	enum direct{UP, DOWN, LEFT, RIGHT};                 //定義方向
	grid g[BLOCK_SIZE];                                 //方塊的座標信息

	void def_block(grid g1, grid g2, grid g3, grid g4); //定義方塊
	void rotate();                                      //旋轉方塊
	void move(int dir);                                 //移動方塊
	void set_cen(grid g);                               //設置方塊旋轉中心
	grid get_cen();                                     //獲取方塊旋轉中心
	void set_type(int t);								//設置方塊種類
	int get_type();										//獲取方塊種類
	void back();										//旋轉還原
	void creat_block(int x, int y);                     //隨機生成方塊

private:
	grid center;                                        //方塊旋轉中心
	int type;                                           //方塊類型
	

};

void Block::def_block(grid g1, grid g2, grid g3, grid g4) {
	g[0]=g1; g[1]=g2; g[2]=g3; g[3]=g4;
}

void Block::rotate() {
	int x, y, i=0;

	for(i; i<=3; i++) {
		x=g[i].x-center.x; y=g[i].y-center.y;
		g[i].x=center.x+y; g[i].y=center.y-x;
	}
}

void Block::move(int dir) {
	int d=dir, i=0;

	switch(d) {
	case UP: { 
		for(i; i<=3; i++) g[i].y++;
		center.y++; break;
			 }
	case DOWN: {
		for(i; i<=3; i++) g[i].y--;
		center.y--; break;
			   }
	case LEFT: {
		for(i; i<=3; i++) g[i].x--;
		center.x--; break;
			   }
	case RIGHT: {
		for(i; i<=3; i++) g[i].x++;
		center.x++; break;
				}
	}
}

void Block::set_cen(grid g) {
	center=g;
}

grid Block::get_cen() {
	return center;
}

void Block::set_type(int t) {
	type=t;
}

int Block::get_type() {
	return type;
}

void Block::back() {
	int x, y, i=0;

	for(i; i<=3; i++) {
		x=g[i].x-center.x; y=g[i].y-center.y;
		g[i].x=center.x-y; g[i].y=center.y+x;
	}
}

void Block::creat_block(int x, int y) {   //隨機創建方塊
	int ran;
	grid g[BLOCK_SIZE];

	
	ran=1+rand()%7;
	switch(ran) {
	//L
	case 1: {
		g[0].x=x/2; g[0].y=y-3;
		g[1].x=g[0].x; g[1].y=g[0].y+1;
		g[2].x=g[0].x; g[2].y=g[0].y+2;
		g[3].x=g[0].x+1; g[3].y=g[0].y; 
		set_cen(g[0]); set_type(1); break;
			}
	//反L
	case 2: {
		g[0].x=x/2; g[0].y=y-3;
		g[1].x=g[0].x; g[1].y=g[0].y+1;
		g[2].x=g[0].x; g[2].y=g[0].y+2;
		g[3].x=g[0].x-1; g[3].y=g[0].y; 
		set_cen(g[0]); set_type(2); break;
			}
	//Z
	case 3: {
		g[0].x=x/2; g[0].y=y-2;
		g[1].x=g[0].x; g[1].y=g[0].y+1;
		g[2].x=g[0].x+1; g[2].y=g[0].y+1;
		g[3].x=g[0].x-1; g[3].y=g[0].y; 
		set_cen(g[0]); set_type(3); break;
			}
	//反Z
	case 4: {
		g[0].x=x/2; g[0].y=y-2;
		g[1].x=g[0].x; g[1].y=g[0].y+1;
		g[2].x=g[0].x+1; g[2].y=g[0].y+1;
		g[3].x=g[0].x-1; g[3].y=g[0].y; 
		set_cen(g[0]); set_type(4); break;
			}
	//田
	case 5: {
		g[0].x=x/2; g[0].y=y-2;
		g[1].x=g[0].x; g[1].y=g[0].y+1;
		g[2].x=g[0].x+1; g[2].y=g[0].y+1;
		g[3].x=g[0].x+1; g[3].y=g[0].y; 
		set_cen(g[0]); set_type(5); break;
			}
	//1
	case 6: {
		g[0].x=x/2; g[0].y=y-3;
		g[1].x=g[0].x; g[1].y=g[0].y+1;
		g[2].x=g[0].x; g[2].y=g[0].y+2;
		g[3].x=g[0].x; g[3].y=g[0].y-1; 
		set_cen(g[0]); set_type(6); break;
			}
	//山
	case 7: {
		g[0].x=x/2; g[0].y=y-2;
		g[1].x=g[0].x; g[1].y=g[0].y+1;
		g[2].x=g[0].x-1; g[2].y=g[0].y;
		g[3].x=g[0].x+1; g[3].y=g[0].y; 
		set_cen(g[0]); set_type(7); break;
			}
	default: ;
	}
	def_block(g[0], g[1], g[2], g[3]);
}


/////////////////////////////////////////

////////////////////Table 類//////////////////////
class Table
{
public:
	
	Table() {                         //構造棋盤
		height=20; width=10; count=0;
		init_table();
	}
	Table(int x, int y);
	int set_block(Block bl);          //安設方塊
	void clr_block(Block bl);         //清除方塊
	int clr_line(int y);			  //消行
	int get_h();                      //獲取棋盤高度
	int get_w();					  //獲取棋盤寬度
	int if_full(int y);				  //判定是否滿行
	int get_table(int x, int y);      //獲取棋盤上點資訊
	void paint();					  //繪製棋盤
	void move_line(int y);            //整行下移
	void set_count(int c);            //記錄得分
	int get_count();                  //獲取得分

private:
	int table[TABLE_SIZE][TABLE_SIZE];//棋盤
	int height, width;                //棋盤的高和寬
	int count;                        //得分

	void init_table();                //棋盤初始化

};

void Table::init_table() {
	int i=0, j=0;

	for(i; i<width; i++) {
		for(j=0; j<height; j++) {
			table[i][j]=0;
		}
	}
}

Table::Table(int x, int y) {
	height=y; width=x; count=0;
	init_table();
}

int Table::set_block(Block bl) {
	int x, y;
	int i;
	for(i=0; i<=3; i++) {
		x=bl.g[i].x; y=bl.g[i].y;
		if(table[x][y]!=0 || x>=width || x<0 || y>=height || y<0) {
			return 0;
		}
	}
	for(i=0; i<=3; i++) {
		x=bl.g[i].x; y=bl.g[i].y;
		table[x][y]=1;
	}
	return 1;
}

void Table::clr_block(Block bl) {
	int x, y;

	for(int i=0; i<=3; i++) {
		x=bl.g[i].x; y=bl.g[i].y;
		table[x][y]=0;
	}
}

int Table::clr_line(int y) {
	if(y<0 || y>=height) return 0;
	for(int i=0; i<width; i++) {
		table[i][y]=0;
	}
	return 1;
}

int Table::get_h() {
	return height;
}

int Table::get_w() {
	return width;
}

int Table::if_full(int y) {
	int i=0;

	for(i; i<width; i++) {
		if(table[i][y]==0) return 0;
	}
	return 1;
}

int Table::get_table(int x, int y) {
	return table[x][y];
}

void Table::paint() {
	int i, j;

	for(i=0; i<width+2; i++) cout<<"-"<<flush;
	cout<<"\n"<<flush;
	for(i=height-1; i>=0; i--) {
		cout<<"|"<<flush;
		for(j=0; j<width; j++) {
			if(table[j][i]==0) cout<<" "<<flush;
			else cout<<"▣"<<flush;
		}
		if(i==10)
			cout<<"|    得分："<<get_count()<<endl;
		else if(i==7)
			cout<<"|    Press 'q' to quit!"<<endl;
		else
			cout<<"|"<<endl;
	}
	for(i=0; i<width+2; i++) cout<<"-"<<flush;
	cout<<"\n"<<flush;
	//cout<<"得分："<<get_count()<<endl;
}

void Table::move_line(int y) {
	int i, j;

	for(i=y; i<height-1; i++) {
		for(j=0; j<width; j++) {
			table[j][i]=table[j][i+1];
		}
	}
}

void Table::set_count(int c) {
	count+=c;
}

int Table::get_count() {
	return count;
}

///////////////////////////////////////////////////////
class Mythread
{
public:
	void init();
	static void *getkey(void *arg);//線程函數在類裏面定義必須定義為static型，以去除類指標。
	static void *paint_loop(void *arg);
};

void Mythread::init()
{
	pthread_t ntid,ntid2;
	int err,err2;		
	err = pthread_create(&ntid,NULL,getkey,NULL);
	err2 = pthread_create(&ntid2,NULL,paint_loop,NULL);
	if(err != 0 || err2 != 0){
		cout<<"can't create thread!"<<endl;
		exit(0);
	}
}

unsigned char flag=1,buf[2];//總體變數
Table tab(15, 20);  //構造一個15,20的棋盤
Block bl;           //構造一個落下方塊
void* Mythread::paint_loop(void *arg)
{
	while(1)
	{
		system("clear");
		tab.paint();
		usleep(50000);       //暫停50 MS
	}
}
void* Mythread::getkey(void *arg)
{
	struct termios saveterm,nt;
	fd_set rfds,rs;
	struct timeval tv;
	int i=0,q,r,fd=0;
	tcgetattr(fd,&saveterm);
	nt=saveterm;

	nt.c_lflag &= ~ECHO;
	nt.c_lflag &= ~ISIG;
	nt.c_lflag &= ~ICANON;

	tcsetattr(fd,TCSANOW,&nt);

	FD_ZERO(&rs);
	FD_SET(fd,&rs);
	tv.tv_sec=0;
	tv.tv_usec=0;
	while(1)
	{	
		read(0,buf,1);
		r=select(fd+1,&rfds,NULL,NULL,&tv);
		if(r<0)
		{
			write(1,"select() error.\n",16);
		}
		rfds=rs;
		if(flag==2||buf[0]==113)//遊戲結束或者用戶按下'q'鍵，則程式退出
		{
			tcsetattr(0,TCSANOW,&saveterm);
			exit(0);
		}
		if(buf[0]<=68&&buf[0]>=65) flag=0;//如果按的鍵是方向鍵，則將標誌位置0並執行相應的處理.
		if(flag==0)
		{
			if(buf[0]==65) {
			//if(dir!=0) {
				if(bl.get_type()==5) continue; //如果出現田字形則不作旋轉
				tab.clr_block(bl);           //清空方塊上一次位置
				bl.rotate();                 //開始旋轉
				if(!tab.set_block(bl)) {     //將旋轉後的方塊寫在棋盤上
					bl.back();				//如果寫失敗(例如到邊線了，或卡住了)則還原旋轉前位置
					continue;
					tab.set_block(bl);       
				}
			}
			//下（加速下落）
			//dir=GetAsyncKeyState(VK_DOWN);   //獲取向下
			if(buf[0]==66) {
				tab.clr_block(bl);			//清空方塊上一次位置
				bl.move(bl.DOWN);			//向下移動一步
				if(!tab.set_block(bl)) {	//將移動後的方塊寫在棋盤上
					bl.move(bl.UP);         //如果失敗，則還原到移動前的位置（即上移一步）
					tab.set_block(bl);
				}
			}
			//左（左移）
			//dir=GetAsyncKeyState(VK_LEFT);
			if(buf[0]==68) {
				tab.clr_block(bl);
				bl.move(bl.LEFT);
				if(!tab.set_block(bl)) {
					bl.move(bl.RIGHT);
					tab.set_block(bl);
				}
			}
			//右（右移）
			//dir=GetAsyncKeyState(VK_RIGHT);
			if(buf[0]==67) {
				tab.clr_block(bl);
				bl.move(bl.RIGHT);
				if(!tab.set_block(bl)) {
					bl.move(bl.LEFT);
					tab.set_block(bl);
				}
			}
			flag=1;
		}
	}
	tcsetattr(0,TCSANOW,&saveterm);
}

////////////主函數部分///////////////////////

int main()
{
	//Table tab(15, 20);  //構造一個15,20的棋盤
	//Block bl;           //構造一個落下方塊
	Mythread thread;
	thread.init();
	int dir,i,c;
	while(true) {
		//生成方塊
		srand(time(0));
		bl.creat_block(tab.get_w(), tab.get_h());
		//判斷遊戲是否結束
		if( !tab.set_block(bl) ) {
			system("clear");
			cout<<"GAME OVER!"<<endl;
			flag=2;
			cout<<"PRESS ANY KEY TO CONTINUE!"<<endl;
			while(1);
		}
		///////////行動按鍵判定
		while(true){
			usleep(500000);       //暫停500 MS
			/////////////向下移動一格
			tab.clr_block(bl);       //清空上一次方塊位置
			bl.move(bl.DOWN);		 //向下移動一步
			if(!tab.set_block(bl)) {     //是否觸底
				bl.move(bl.UP);       //如果觸底，還原觸底前位置
				tab.set_block(bl);
				break;
			}
		}
		//如果滿行則消行
		for(i=0; i<tab.get_h(); i++) {
			if(tab.if_full(i)) {  //是否滿行
				tab.clr_line(i);  //如果是，消行
				tab.move_line(i); //將所消行的上面的棋盤資訊下移
				i--;           //下移後，重新檢查這一行是否滿（可能出現幾行同時消去）
				tab.set_count(100); //記錄得分
			}
		}
		
	}
	return 0;
}
