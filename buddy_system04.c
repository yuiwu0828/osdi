#define base_addr = 0x20000000
#define using = 10

struct page{
    int size;
    int id;
    void* addr;
    struct page* next;
    struct page* last;
};

struct page* list[7];
int frame_array[64];
struct page* using_array[64];

int check(int index){
    if(index > 6 || index < 0){
        return -1;
    }
    else if(list[index] == NULL){
        return 0;
    }
    return 1;
}

int pow(int a, int b){
	int c = 1;
	if(b == 0){
		return 1;
	}
	while(b--){
		c = c*a;
	}
	return c;
}

int split(int target){
    	int t = check(target);
    	if(t == -1){
    	    return 0;
    	}
    	else if(t == 0){
     	   if(!split(target+1)){
       	     return 0;
       	 }
   	}
   	struct page* tmp = list[target];
    	list[target] = tmp->next;
    	frame_array[tmp->id*pow(2, tmp->size)] = target-1;
    	frame_array[tmp->id*pow(2, tmp->size)+pow(2, tmp->size-1)] = target-1;
    	struct page* right;
    	right->addr = (void*)((int)tmp->addr+pow(2, tmp->size-1)*4096);
    	tmp->id = tmp->id*2;
    	tmp->next = right;
    	tmp->size = target-1;
    	tmp->last = NULL;
    	right->id = tmp->id*2+1;
    	right->next = NULL;
    	right->size = target-1;
    	right->last = left;
    	list[target-1] = left;
    	return 1;
}


void merge(struct page* p, int condition, int left_right){
	struct page* tmp;
	if(p->size == 6){
		condition = 0;
	}
	if(condition == 0){
		tmp = list[p->size];
		if(tmp == NULL){
            		list[p->size] = p;
            		return;
        	}
        	if(tmp->id > p->id){
            		p->next = tmp;
            		tmp->last = p;
            		p->last = NULL;
            		list[p->size] = p;
            		return;
        	}
        	while(tmp != NULL){
           		if(p->id > tmp->id){
            		 	tmp = tmp->next;
            		}
            		else{
                		break;
            		}
        	}
        	p->last = tmp->last;
        	tmp->last->next = p;
        	p->next = tmp;
        	tmp->last = p;
        	return;
	}
	if(condition == 1){
		if(left_right == 0){
    			if(frame_array[(p->id+1)*pow(2, p->size)] == p->size){
    				frame_array[(p->id+1)*pow(2, p->size)] = -1;
    				frame_array[p->id*pow(2, p->size)]++;
    				p->size++;
    				p->id = p->id/2;
    				merge(p, 1, p->id%2);
    			}
    			else{
    				merge(p, 0, 0);
    			}
    		}
    		if(left_right == 1){
    			if(frame_array[(p->id-1)*pow(2, p->size)] == done->size){
    				frame_array[p->id*pow(2, p->size)] = -1;
    				frame_array[(p->id-1)*pow(2, p->size)]++;
    				p->addr = (void*)((int)p->addr-pow(2, p->size)*4096);
    				p->size++;
    				p->id = done->id/2;
    				merge(p, 1, p->id%2);
    			}
    			else{
    				merge(p, 0, 1);
    			}
    		}
	}
}

void* page_locate(int need){
	int t = check(need);
    	if(t == -1){
    	   	uart_puts("Unvalid number!\n");
   	    	return NULL;
    	}
    	else if(t == 0){
     	  	if(!split(need+1)){
      	     		uart_puts("Fail! Don't have enough space\n");
      	     		return NULL;
      	  	}
   	}
    	struct page* tmp = list[need];
    	list[need] = tmp->next;
    	tmp->last = NULL;
    	tmp->next = NULL;
    	frame_array[tmp->id*pow(2, need)] = using;
    	using_array[tmp->id*pow(2, need)] = tmp;
    	return tmp->addr;
}

void space_free(void* d){
    	struct page* done = using_array[((int)done-base_addr)/4096];
    	frame_array[done->id*pow(2, done->size)] = done->size;
    	using_array[done->id*pow(2, done->size)] = NULL;
    	if(done->id%2 == 0){
    		if(frame_array[(done->id+1)*pow(2, done->size)] == done->size){
    			frame_array[(done->id+1)*pow(2, done->size)] = -1;
    			frame_array[done->id*pow(2, done->size)]++;
    			done->size++;
    			done->id = done->id/2;
    			merge(done, 1, done->id%2);
    		}
    		else{
    			merge(done, 0, 0);
    		}
    	}
    	if(done->id%2 == 1){
    		if(frame_array[(done->id-1)*pow(2, done->size)] == done->size){
    			frame_array[done->id*pow(2, done->size)] = -1;
    			frame_array[(done->id-1)*pow(2, done->size)]++;
    			done->addr = (void*)((int)done->addr-pow(2, done->size)*4096);
    			done->size++;
    			done->id = done->id/2;
    			merge(done, 1, done->id%2);
    		}
    		else{
    			merge(done, 0, 1);
    		}
    	}
    	return;
}

int main(){
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
    	return 0;
}
