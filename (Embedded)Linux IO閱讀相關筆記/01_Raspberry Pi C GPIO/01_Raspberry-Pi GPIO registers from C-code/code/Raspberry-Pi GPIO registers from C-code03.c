//
//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//  Revised: 15-Feb-2013


// Access from ARM Running Linux

/*
//at BCM2835-ARM-Peripherals.pdf P6
1.2.2 ARM virtual addresses (standard Linux kernel only)
As is standard practice, the standard BCM2835 Linux kernel provides a contiguous mapping
over the whole of available RAM at the top of memory. The kernel is configured for a
1GB/3GB split between kernel and user-space memory.
The split between ARM and GPU memory is selected by installing one of the supplied
start*.elf files as start.elf in the FAT32 boot partition of the SD card. The minimum amount
of memory which can be given to the GPU is 32MB, but that will restrict the multimedia
performance; for example, 32MB does not provide enough buffering for the GPU to do
1080p30 video decoding.
Virtual addresses in kernel mode will range between 0xC0000000 and 0xEFFFFFFF.
Virtual addresses in user mode (i.e. seen by processes running in ARM Linux) will range
between 0x00000000 and 0xBFFFFFFF.
Peripherals (at physical address 0x20000000 on) are mapped into the kernel virtual address
space starting at address 0xF2000000. Thus a peripheral advertised here at bus address
0x7Ennnnnn is available in the ARM kenel at virtual address 0xF2nnnnnn.
*/
#define BCM2708_PERI_BASE        0x20000000

/* GPIO controller
//Line 135~140 at wiringPi.c
//�ѩ�O����mapping�᪺���G
#define BCM2708_PERI_BASE	                     0x20000000
#define GPIO_PADS		(BCM2708_PERI_BASE + 0x00100000)
#define CLOCK_BASE		(BCM2708_PERI_BASE + 0x00101000)
#define GPIO_BASE		(BCM2708_PERI_BASE + 0x00200000)
#define GPIO_TIMER		(BCM2708_PERI_BASE + 0x0000B000)
#define GPIO_PWM		(BCM2708_PERI_BASE + 0x0020C000)
 */

#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) 


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
void *gpio_map;

// I/O access
volatile unsigned *gpio;


/*GPIO setup macros. 
Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
int registerIndex = GPIO / 10;
int bit = (GPIO % 10) * 3;
unsigned oldValue = s_pGpioRegisters->GPFSEL[registerIndex];
unsigned mask = 0b111 << bit;
*/
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio+13)&(1<<g)) // 0 if LOW, (1<<g) if HIGH

void setup_io();

void printButton(int g)
{
  if (GET_GPIO(g)) // !=0 <-> bit is 1 <- port is HIGH=3.3V
    printf("Button pressed!\n");
  else // port is LOW=0V
    printf("Button released!\n");
}

int main(int argc, char **argv)
{
  int g,rep;

  // Set up gpi pointer for direct register access
  setup_io();

  // Switch GPIO 7..11 to output mode

 /************************************************************************\
  * You are about to change the GPIO settings of your computer.          *
  * Mess this up and it will stop working!                               *
  * It might be a good idea to 'sync' before running this program        *
  * so at least you still have your code changes written to the SD-card! *
 \************************************************************************/

  // Set GPIO pins 7-11 to output
  for (g=7; g<=11; g++)
  {
    INP_GPIO(g); // must use INP_GPIO before we can use OUT_GPIO
    OUT_GPIO(g);
  }

  for (rep=0; rep<10; rep++)
  {
     for (g=7; g<=11; g++)
     {
       GPIO_SET = 1<<g;
       sleep(1);
     }
     for (g=7; g<=11; g++)
     {
       GPIO_CLR = 1<<g;
       sleep(1);
     }
  }

  return 0;

} // main


//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit(-1);
   }

   /*
   mmap GPIO:
����M�g��� mmap ���ϥΤ�k

�Ө�ƥD�n�γ~���T�ӡG
1�B�N�@�Ӵ��q���M�g��֤ߤ��A�q�`�b�ݭn����i���W�cŪ�g�ɨϥΡA�o�˥ή֤�Ū�g���NI/OŪ�g�A�H��o�������ʯ�F
2�B�N�S����i��ΦW�֤߬M�g�A�i�H�����p�i�{���Ѧ@�ɮ֤ߪŶ��F
3�B���L���p���i�{���Ѧ@�ɮ֤ߪŶ��A�@��]�O�N�@�Ӵ��q���M�g��֤ߤ��C

��ơGvoid *mmap(void *start,size_t length, int prot, int flags, int fd, off_t offsize);

�Ѽ�start�G���V���M�g���֤߰_�l��}�A�q�`�]��NULL�A�N�����t�Φ۰ʿ�w��}�A�֤߷|�ۤv�b�i�{��}�Ŷ�����ܦX�A����}�إ߬M�g�C
�M�g���\���^�Ӧ�}�C�p�G���ONULL�A�h���֤ߤ@�Ӵ��ܡA���ӱq�����}�}�l�M�g�A�֤߷|���start���W���Y�ӦX�A����}�}�l�M�g�C
�إ߬M�g��A�u�����M�g��}�q�L��^�ȥi�H�o��C

�Ѽ�length�G�N��M�g���j�p�C�N��󪺦h�j���׬M�g��O����C 

�Ѽ�prot�G�M�g�ϰ쪺�O�@�覡�C�i�H���H�U�X�ؤ覡���զX�G
PROT_EXEC �M�g�ϰ�i�Q����
PROT_READ �M�g�ϰ�i�QŪ��
PROT_WRITE �M�g�ϰ�i�Q�g�J
PROT_NONE �M�g�ϰ줣��s��

�Ѽ�flags�G�v�T�M�g�ϰ쪺�U�دS�ʡC�b�ե�mmap()�ɥ����n���wMAP_SHARED ��MAP_PRIVATE�C
MAP_FIXED �p�G�Ѽ�start�ҫ�����}�L�k���\�إ߬M�g�ɡA�h���M�g�A�����}���ץ��C�q�`�����y�Φ��X�СC
MAP_SHARED ���\��L�M�g�Ӥ�󪺦�{�@�ɡA��M�g�ϰ쪺�g�J�ƾڷ|�ƻs�^���C
MAP_PRIVATE �����\��L�M�g�Ӥ�󪺦�{�@�ɡA��M�g�ϰ쪺�g�J�ާ@�|���ͤ@�ӬM�g���ƻs(copy-on-write)�A�惡�ϰ�Ұ����ק藍�|�g�^����C
MAP_ANONYMOUS �إ߰ΦW�M�g�C���ɷ|�����Ѽ�fd�A���A�Τ��A�ӥB�M�g�ϰ�L�k�M��L�i�{�@�ɡC
MAP_DENYWRITE �u���\��M�g�ϰ쪺�g�J�ާ@�A��L���󪽱��g�J���ާ@�N�|�Q�ڵ��C
MAP_LOCKED �N�M�g�ϰ���w��A�o��ܸӰϰ줣�|�Q�m��(swap)�C

�Ѽ�fd�G��open��^�����y�z�šA�N��n�M�g��֤ߤ������C�p�G�ϥΰΦW�֤߬M�g�ɡA�Yflags���]�m�FMAP_ANONYMOUS�Afd�]��-1�C
���Ǩt�Τ�����ΦW�֤߬M�g�A�h�i�H�ϥ�fopen���}/dev/zero���A�M���Ӥ��i��M�g�A�i�H�P�˹F��ΦW�֤߬M�g���ĪG�C

�Ѽ�offset�G�q���M�g�}�l�B�������q�A�q�`��0�A�N��q���̫e��}�l�M�g�C
offset�����O�����j�p����ƭ�(�b32����t�ε��c�W�q�`�O4K)�C

��^�ȡG
�Y�M�g���\�h��^�M�g�Ϫ��֤߰_�l��}�A�_�h��^MAP_FAILED(-1)�A���~��]�s��errno ���C

���~�N�X�G
EBADF �Ѽ�fd ���O���Ī����y�z���C
EACCES �s���v�����~�C�p�G�OMAP_PRIVATE ���p�U��󥲶��iŪ�A�ϥ�MAP_SHARED�h�n��PROT_WRITE�H�θӤ��n��g�J�C
EINVAL �Ѽ�start�Blength ��offset���@�Ӥ��X�k�C
EAGAIN ���Q���A�άO���Ӧh�֤߳Q���C
ENOMEM �֤ߤ����C

�U�����@�U�O����M�g���B�J:
1.��open�t�νեΥ��}���, �ê�^�y�z��fd. 
2.��mmap�إ߰O����M�g, �ê�^�M�g�}�l���a�}����start. 
3.��M�g(���)�i��U�ؾާ@
4.��munmap(void *start, size_t lenght)�����O����M�g. 
5.��close�t�νե��������fd.

�t�νե�mmap()�Ω�@�ɮ֤ߪ���ؤ覡�G

(1)�ϥδ��q��󴣨Ѫ��֤߬M�g�G

�A�Ω����i�{�����C���ɡA�ݭn���}�γЫؤ@�Ӥ��A�M��A�ե�mmap()

�嫬�եΥN�X�p�U�G

fd=open(name, flag, mode); if(fd<0) ...

ptr=mmap(NULL, len , PROT_READ|PROT_WRITE, MAP_SHARED , fd , 0);

�q�Lmmap()��{�@�ɮ֤ߪ��q�H�覡���\�h�S�I�M�n�`�N���a��A�i�H�Ѭ�UNIX�����s�{�ĤG���C

(2)�ϥίS���󴣨ѰΦW�֤߬M�g�G

�A�Ω�㦳�˽t���Y���i�{�����C�ѩ���l�i�{�S���˽t���Y�A�b���i�{�����ե�mmap()�A�M��ե�fork()�C
����b�ե�fork()����A�l�i�{�~�Ӥ��i�{�ΦW�M�g�᪺��}�Ŷ��A�P�ˤ]�~��mmap()��^����}�A�o�ˡA���l�i�{�N�i�H�q�L�M�g�ϰ�i��q�H�F�C
�`�N�A�o�̤��O�@�몺�~�����Y�C�@��ӻ��A�l�i�{��W���@�q���i�{�~�ӤU�Ӫ��@���ܶq�C
��mmap()��^����}�A�o�Ѥ��l�i�{�@�P���@�C���㦳�˽t���Y���i�{��{�@�ɮ֤̦߳n���覡���ӬO�ĥΰΦW�֤߬M�g���覡�C
���ɡA�������w���骺���A�u�n�]�m�������лx�Y�i�C

munmap����ۤϪ��ާ@�A�R���S�w��}�ϰ쪺��H�M�g�A����󪺬M�g�A�bmmap�Mmunmap����L�{������ɨ�A�Q�M�g���st_atime�i��Q��s�C
   */
   gpio_map = mmap(
      NULL,             //Any adddress in our space will do
      BLOCK_SIZE,       //Map length
      PROT_READ|PROT_WRITE,// Enable reading & writting to mapped memory
      MAP_SHARED,       //Shared with other processes
      mem_fd,           //File to map
      GPIO_BASE         //Offset to GPIO peripheral
   );

   close(mem_fd); //No need to keep mem_fd open after mmap

   if (gpio_map == MAP_FAILED) {
      printf("mmap error %d\n", (int)gpio_map);//errno also set!
      exit(-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io