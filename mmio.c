#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdint.h>

#define BASE        0x3F000000
#define GPIO_BASE   (BASE + 0x200000)


volatile uint32_t *gpio_addr;

#define GPIO_IN(x) *(gpio_addr+((x)/10)) &= ~(7<<(((x)%10)*3))
#define GPIO_OUT(x) *(gpio_addr+((x)/10)) |=  (1<<(((x)%10)*3))


void mmio_gpio(){
    int  mem_fd;
    void *gpio_map;
    
    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
        printf("without privilege of /dev/mem \n");
        exit(-1);
    }

    gpio_map = mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,mem_fd,GPIO_BASE);

    close(mem_fd);

    if (gpio_map == MAP_FAILED) {
        printf("mmap error %d\n", (int)gpio_map);
        exit(-1);
    }

    gpio_addr = (volatile uint32_t *)gpio_map;
}

int main(int argc, char **argv)
{
    int pin = 17;
    
    mmio_gpio();
 
    GPIO_IN(pin);
    GPIO_OUT(pin);
 
    volatile uint32_t *GPSETn = (gpio_addr+7);
    volatile uint32_t *GPCLRn = (gpio_addr+10);

    while(1){
	*(GPSETn) = 1 << pin;    
    	sleep(1);
 
        *(GPCLRn) = 1 << pin;
    	sleep(1);
    	printf("a iteration\n");
    };
    return 0;
}
