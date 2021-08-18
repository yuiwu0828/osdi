#define base_addr = 0x20000000
#define using = 10
#include<stdio.h>
#include<math.h>

struct page{
    int size;
    int id;
    struct page* next;
    struct page* last;
};

struct page* list[7];
int frame_array[63];

int check(int index){
    if(index > 6 || index < 0){
        return -1;
    }
    else if(list[index] == NULL){
        return 0;
    }
    return 1;
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
    	struct page* right = (struct page*)(base_addr+tmp->id*pow(2, tmp->size)*4096);
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
        	p->next = tmp;
        	tmp->last = p;
        	p->last = tmp->last;
        	tmp->last->next = p;
        	return;
	}
	if(condition == 1){
		int tmp = ((int)p-base_addr)/4096;
		if(left_right == 0){
    			if(frame_array[tmp+pow(2, p->size)] == p->size){
    				frame_array[tmp+pow(2, done->size)] = -1;
    				frame_array[tmp]++;
    				done->size++;
    				done->id = done->id/2;
    				if(done->size == 6){
    					merge(done, 0, done->id%2);
    				}
    				merge(done, 1, done->id%2);
    			}
    			if(frame_array[tmp+pow(2, p->size)] == p->size){
    				merge(done, 0, 0);
    			}
    		}
    		if(left_right == 1){
    			if(frame_array[tmp-pow(2, done->size)] == done->size){
    				frame_array[tmp+pow(2, done->size)] = -1;
    				frame_array[tmp]++;
    				done->size++;
    				done->id = done->id/2;
    				if(done->size == 6){
    					merge(done, 0, done->id%2);
    				}
    				merge(done, 1, done->id%2);
    			}
    			if(frame_array[tmp-pow(2, done->size)] == done->size){
    				merge(done, 0, 1);
    			}
    		}
	}
}

struct page* page_locate(int need){
	int t = check(need);
    	if(t == -1){
    	   	printf("Unvalid number!\n");
   	    	return NULL;
    	}
    	else if(t == 0){
     	  	if(!split(need+1)){
      	     		printf("Fail! Don't have enough space\n");
      	     		return NULL;
      	  	}
   	}
    	struct page* tmp = list[need];
    	list[need] = tmp->next;
    	frame_array[tmp->id*pow(2, need)] = using;
    	return tmp;
}

void space_free(struct page* done){
    	int s = ((int)done-base_addr)/4096;
    	int tmp = 1;
    	while(frame_array[s+1] == -1){
    		tmp++;
    		s++;
    	}
    	s = 0;
    	while(tmp != 1){
    		tmp = tmp/2;
    		s++;
    	}
    	done->size = s;
    	tmp = ((int)done-base_addr)/4096;
    	frame_array[tmp] = s;
    	done->id = tmp/done->size;
    	done->next = NULL;
    	done->last = NULL;
    	if(done->id%2 == 0){
    		if(frame_array[tmp+pow(2, done->size)] == done->size){
    			frame_array[tmp+pow(2, done->size)] = -1;
    			frame_array[tmp]++;
    			done->size++;
    			done->id = done->id/2;
    			if(done->size == 6){
    				merge(done, 0, done->id%2);
    			}
    			merge(done, 1, done->id%2);
    		}
    		if(frame_array[tmp+pow(2, done->size)] != done->size){
    			merge(done, 0, 0);
    		}
    	}
    	if(done->id%2 == 1){
    		if(frame_array[tmp-pow(2, done->size)] == done->size){
    			frame_array[tmp+pow(2, done->size)] = -1;
    			frame_array[tmp]++;
    			done->size++;
    			done->id = done->id/2;
    			if(done->size == 6){
    				merge(done, 0, done->id%2);
    			}
    			merge(done, 1, done->id%2);
    		}
    		if(frame_array[tmp-pow(2, done->size)] != done->size){
    			merge(done, 0, 1);
    		}
    	}
    	return;
}

int main(){
	for(int i = 1; i < 63; i++){
		frame_array[i] = -1;
	}
    	frame_array[0] = 6;
    	
    	struct page* all = (struct page*)(base_addr);
    	all->id = 0;
    	all->next = NULL;
    	all->size = 6;
    	all->last = NULL;
    	list[6] = all;
    	return 0;
}
