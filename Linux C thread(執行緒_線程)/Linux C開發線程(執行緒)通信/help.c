#include <stdio.h>
#include <pthread.h>
#define BUFFER_SIZE 16 // 緩衝區數量
struct prodcons
{
	// 緩衝區相關資料結構
	int buffer[BUFFER_SIZE]; /* 實際資料存放的陣列*/
	pthread_mutex_t lock; /* 互斥體lock 用於對緩衝區的互斥操作 */
	int readpos, writepos; /* 讀寫指針*/
	pthread_cond_t notempty; /* 緩衝區非空的條件變數 */
	pthread_cond_t notfull; /* 緩衝區未滿的條件變數 */
};
/* 初始化緩衝區結構 */
void init(struct prodcons *b)
{
	pthread_mutex_init(&b->lock, NULL);
	pthread_cond_init(&b->notempty, NULL);
	pthread_cond_init(&b->notfull, NULL);
	b->readpos = 0;
	b->writepos = 0;
}
/* 將產品放入緩衝區,這裏是存入一個整數*/
void put(struct prodcons *b, int data)
{
	pthread_mutex_lock(&b->lock);
	/* 等待緩衝區未滿*/
	if ((b->writepos + 1) % BUFFER_SIZE == b->readpos)
	{
		pthread_cond_wait(&b->notfull, &b->lock);
	}
	/* 寫資料,並移動指標 */
	b->buffer[b->writepos] = data;
	b->writepos++;
	
	if (b->writepos > = BUFFER_SIZE)
		b->writepos = 0;
	
	/* 設置緩衝區非空的條件變數*/
	pthread_cond_signal(&b->notempty);
	pthread_mutex_unlock(&b->lock);
} 
/* 從緩衝區中取出整數*/
int get(struct prodcons *b)
{
	int data;
	pthread_mutex_lock(&b->lock);
	/* 等待緩衝區非空*/
	if (b->writepos == b->readpos)
	{
		pthread_cond_wait(&b->notempty, &b->lock);
	}
	/* 讀數據,移動讀指標*/
	data = b->buffer[b->readpos];
	b->readpos++;
	
	if (b->readpos > = BUFFER_SIZE)
		b->readpos = 0;
	
	/* 設置緩衝區未滿的條件變數*/
	pthread_cond_signal(&b->notfull);
	pthread_mutex_unlock(&b->lock);
	return data;
}

/* 測試:生產者線程將1 到10000 的整數送入緩衝區,消費者線
程從緩衝區中獲取整數,兩者都列印資訊*/
#define OVER ( - 1)
struct prodcons buffer;
void *producer(void *data)
{
	int n;
	for (n = 0; n < 10000; n++)
	{
		printf("%d --->\n", n);
		put(&buffer, n);
	}
	put(&buffer, OVER);
	return NULL;
}

void *consumer(void *data)
{
	int d;
	while (1)
	{
		d = get(&buffer);
		if (d == OVER)
			break;
		printf("--->%d \n", d);
	}
	return NULL;
}

int main(void)
{
	pthread_t th_a, th_b;
	void *retval;
	init(&buffer);
	/* 創建生產者和消費者線程*/
	pthread_create(&th_a, NULL, producer, 0);
	pthread_create(&th_b, NULL, consumer, 0);

	/* 等待兩個線程結束*/
	pthread_join(th_a, &retval);
	pthread_join(th_b, &retval);
	return 0;
}
