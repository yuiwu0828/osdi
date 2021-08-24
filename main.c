#include "uart.h"
#include "buddy_system.h"
#include "sprintf.h"
#define base_addr 0x20000000
#define using 10
#define NULL 0

struct page* list[7] = {};
int frame_array[64] = {};	
struct page* using_array[64] = {};
	
int strcmp(const char *a,const char *b){
  	if (! (*a | *b)){ 
  		return 0;
  	}
  	return (*a!=*b) ? *a-*b : strcmp(++a,++b);
}

int find_char(const char* a, const char* want, int index){
	for(int i = 0; i < index; i++){
		if(*(a+i) == *want){
			return i;
		}
	}
	return -1;
}

char* left(char* need, const char* a, int len){
	while(len--){
		*(need++) = *(a++);
	}
	return need;
}

void main()
{
    	// set up serial console
    	uart_init();
    
    	// say hello
    	uart_puts("Hello World!\n");

	for(int i = 0; i < 64; i++){
		frame_array[i] = -1;
		using_array[i] = NULL;
	}
    	frame_array[0] = 6;
    	struct page* all;
    	all->addr = (void*)(base_addr);
    	all->id = 0;
    	all->next = NULL;
    	all->size = 6;
    	all->last = NULL;
    	list[6] = all;
    	while(1) {
		char cmd[20] = "";
      	  	int index = 0;
        	while(1) {
            		char c = uart_getc();
            		uart_send(c);
            		if(c != '\n') {
                		cmd[index] = c;
                		index++;
            		}
            		else{
                		break;
            		}
        	}
        	if(!strcmp(cmd, "help")) {
            		uart_puts("help: print this command list \nprint: print Hello World!\n");
            		uart_puts("pageloc %d:get a pointer to page\n");
        	}
        	else if(!strcmp(cmd, "print")) {
            		uart_puts("Hello World!\n");
        	}
        	else {
        		int tmp = find_char(cmd, " ", index);
        		if(tmp != -1){
        			char part[20] = {};
        			left(part, cmd, tmp);
        			if(!strcmp(part, "pageloc")){
        				int t = (int)*(cmd+tmp+1)-48;
        				void* place = page_locate(t);
        			}
        			else{
        				uart_puts("Not such command\n");
        			}
        		}
        		if(tmp == -1){
        			uart_puts("Not such command\n");
        		}
        	}
    	}
}
