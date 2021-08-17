#define base_addr = 0x20000000
#define using = 10
#include<stdio.h>
#include<stdlib.h>
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

/*int seek(int ID, int check_size){
    struct page* tmp = list[check_size];
    int position = 0;
    if(tmp == NULL){
        return -1;
    }
    while(tmp != NULL){
        if(tmp->id == ID){
            return position;
        }
        position++;
        tmp = tmp->next;
    }
    return -1;
}*/

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

/*void merge(struct page* p, int pos, int lorr){
    struct page* tmp;
    if(pos == -1){
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
    else if(pos == 0){
        tmp = list[p->size];
        if(lorr == 1){
            p->addr = tmp->addr;
        }
        list[p->size] = tmp->next;
        free(tmp);
        p->id = p->id/2;
        p->size = p->size+1;
        p->last = NULL;
        p->next = NULL;
        lorr = p->id%2;
        if(lorr == 0){
            pos = seek(p->id+1, p->size);
        }
        else{
            pos = seek(p->id-1, p->size);
        }
        merge(p, pos, lorr);
        return;
    }
    else{
        tmp = list[p->size];
        for(int i = 0; i < pos; i++){
            tmp = tmp->next;
        }
        if(lorr == 1){
            p->addr = tmp->addr;
        }
        if(tmp->last != NULL){
            tmp->last->next = tmp->next;
        }
        if(tmp->next != NULL){
            tmp->next->last = tmp->last;
        }
        free(tmp);
        p->id = p->id/2;
        p->size = p->size+1;
        p->last = NULL;
        p->next = NULL;
        lorr = p->id%2;
        if(lorr == 0){
            pos = seek(p->id+1, p->size);
        }
        else{
            pos = seek(p->id-1, p->size);
        }
        printf("%d\n", pos);
        merge(p, pos, lorr);
        return;
    }
}*/

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

/*void space_free(struct page* done){
    int place, left_right;
    left_right = done->id % 2;
    if(left_right == 0){
        place = seek(done->id+1, done->size);
    }
    else{
        place = seek(done->id-1, done->size);
    }
    merge(done, place, left_right);
    return;
}*/

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
