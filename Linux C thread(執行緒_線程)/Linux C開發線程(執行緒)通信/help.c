#include <stdio.h>
#include <pthread.h>
#define BUFFER_SIZE 16 // �w�İϼƶq
struct prodcons
{
	// �w�İϬ�����Ƶ��c
	int buffer[BUFFER_SIZE]; /* ��ڸ�Ʀs�񪺰}�C*/
	pthread_mutex_t lock; /* ������lock �Ω��w�İϪ������ާ@ */
	int readpos, writepos; /* Ū�g���w*/
	pthread_cond_t notempty; /* �w�İϫD�Ū������ܼ� */
	pthread_cond_t notfull; /* �w�İϥ����������ܼ� */
};
/* ��l�ƽw�İϵ��c */
void init(struct prodcons *b)
{
	pthread_mutex_init(&b->lock, NULL);
	pthread_cond_init(&b->notempty, NULL);
	pthread_cond_init(&b->notfull, NULL);
	b->readpos = 0;
	b->writepos = 0;
}
/* �N���~��J�w�İ�,�o�جO�s�J�@�Ӿ��*/
void put(struct prodcons *b, int data)
{
	pthread_mutex_lock(&b->lock);
	/* ���ݽw�İϥ���*/
	if ((b->writepos + 1) % BUFFER_SIZE == b->readpos)
	{
		pthread_cond_wait(&b->notfull, &b->lock);
	}
	/* �g���,�ò��ʫ��� */
	b->buffer[b->writepos] = data;
	b->writepos++;
	
	if (b->writepos > = BUFFER_SIZE)
		b->writepos = 0;
	
	/* �]�m�w�İϫD�Ū������ܼ�*/
	pthread_cond_signal(&b->notempty);
	pthread_mutex_unlock(&b->lock);
} 
/* �q�w�İϤ����X���*/
int get(struct prodcons *b)
{
	int data;
	pthread_mutex_lock(&b->lock);
	/* ���ݽw�İϫD��*/
	if (b->writepos == b->readpos)
	{
		pthread_cond_wait(&b->notempty, &b->lock);
	}
	/* Ū�ƾ�,����Ū����*/
	data = b->buffer[b->readpos];
	b->readpos++;
	
	if (b->readpos > = BUFFER_SIZE)
		b->readpos = 0;
	
	/* �]�m�w�İϥ����������ܼ�*/
	pthread_cond_signal(&b->notfull);
	pthread_mutex_unlock(&b->lock);
	return data;
}

/* ����:�Ͳ��̽u�{�N1 ��10000 ����ưe�J�w�İ�,���O�̽u
�{�q�w�İϤ�������,��̳��C�L��T*/
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
	/* �ЫإͲ��̩M���O�̽u�{*/
	pthread_create(&th_a, NULL, producer, 0);
	pthread_create(&th_b, NULL, consumer, 0);

	/* ���ݨ�ӽu�{����*/
	pthread_join(th_a, &retval);
	pthread_join(th_b, &retval);
	return 0;
}
