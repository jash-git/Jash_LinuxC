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
//由於記憶體mapping後的結果
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
憶體映射函數 mmap 的使用方法

該函數主要用途有三個：
1、將一個普通文件映射到核心中，通常在需要對文件進行頻繁讀寫時使用，這樣用核心讀寫取代I/O讀寫，以獲得較高的性能；
2、將特殊文件進行匿名核心映射，可以為關聯進程提供共享核心空間；
3、為無關聯的進程提供共享核心空間，一般也是將一個普通文件映射到核心中。

函數：void *mmap(void *start,size_t length, int prot, int flags, int fd, off_t offsize);

參數start：指向欲映射的核心起始位址，通常設為NULL，代表讓系統自動選定位址，核心會自己在進程位址空間中選擇合適的位址建立映射。
映射成功後返回該位址。如果不是NULL，則給核心一個提示，應該從什麼位址開始映射，核心會選擇start之上的某個合適的位址開始映射。
建立映射後，真正的映射位址通過返回值可以得到。

參數length：代表映射的大小。將文件的多大長度映射到記憶體。 

參數prot：映射區域的保護方式。可以為以下幾種方式的組合：
PROT_EXEC 映射區域可被執行
PROT_READ 映射區域可被讀取
PROT_WRITE 映射區域可被寫入
PROT_NONE 映射區域不能存取

參數flags：影響映射區域的各種特性。在調用mmap()時必須要指定MAP_SHARED 或MAP_PRIVATE。
MAP_FIXED 如果參數start所指的位址無法成功建立映射時，則放棄映射，不對位址做修正。通常不鼓勵用此旗標。
MAP_SHARED 允許其他映射該文件的行程共享，對映射區域的寫入數據會複製回文件。
MAP_PRIVATE 不允許其他映射該文件的行程共享，對映射區域的寫入操作會產生一個映射的複製(copy-on-write)，對此區域所做的修改不會寫回原文件。
MAP_ANONYMOUS 建立匿名映射。此時會忽略參數fd，不涉及文件，而且映射區域無法和其他進程共享。
MAP_DENYWRITE 只允許對映射區域的寫入操作，其他對文件直接寫入的操作將會被拒絕。
MAP_LOCKED 將映射區域鎖定住，這表示該區域不會被置換(swap)。

參數fd：由open返回的文件描述符，代表要映射到核心中的文件。如果使用匿名核心映射時，即flags中設置了MAP_ANONYMOUS，fd設為-1。
有些系統不支持匿名核心映射，則可以使用fopen打開/dev/zero文件，然後對該文件進行映射，可以同樣達到匿名核心映射的效果。

參數offset：從文件映射開始處的偏移量，通常為0，代表從文件最前方開始映射。
offset必須是分頁大小的整數倍(在32位體系統結構上通常是4K)。

返回值：
若映射成功則返回映射區的核心起始位址，否則返回MAP_FAILED(-1)，錯誤原因存於errno 中。

錯誤代碼：
EBADF 參數fd 不是有效的文件描述詞。
EACCES 存取權限有誤。如果是MAP_PRIVATE 情況下文件必須可讀，使用MAP_SHARED則要有PROT_WRITE以及該文件要能寫入。
EINVAL 參數start、length 或offset有一個不合法。
EAGAIN 文件被鎖住，或是有太多核心被鎖住。
ENOMEM 核心不足。

下面說一下記憶體映射的步驟:
1.用open系統調用打開文件, 並返回描述符fd. 
2.用mmap建立記憶體映射, 並返回映射開始的地址指標start. 
3.對映射(文件)進行各種操作
4.用munmap(void *start, size_t lenght)關閉記憶體映射. 
5.用close系統調用關閉文件fd.

系統調用mmap()用於共享核心的兩種方式：

(1)使用普通文件提供的核心映射：

適用於任何進程之間。此時，需要打開或創建一個文件，然後再調用mmap()

典型調用代碼如下：

fd=open(name, flag, mode); if(fd<0) ...

ptr=mmap(NULL, len , PROT_READ|PROT_WRITE, MAP_SHARED , fd , 0);

通過mmap()實現共享核心的通信方式有許多特點和要注意的地方，可以參看UNIX網絡編程第二卷。

(2)使用特殊文件提供匿名核心映射：

適用於具有親緣關係的進程之間。由於父子進程特殊的親緣關係，在父進程中先調用mmap()，然後調用fork()。
那麼在調用fork()之後，子進程繼承父進程匿名映射後的位址空間，同樣也繼承mmap()返回的位址，這樣，父子進程就可以通過映射區域進行通信了。
注意，這裡不是一般的繼承關係。一般來說，子進程單獨維護從父進程繼承下來的一些變量。
而mmap()返回的位址，卻由父子進程共同維護。對於具有親緣關係的進程實現共享核心最好的方式應該是採用匿名核心映射的方式。
此時，不必指定具體的文件，只要設置相應的標誌即可。

munmap執行相反的操作，刪除特定位址區域的對象映射，基於文件的映射，在mmap和munmap執行過程的任何時刻，被映射文件的st_atime可能被更新。
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