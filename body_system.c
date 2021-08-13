#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct page{
    int size;
    int id;
    int addr;
    struct page* next;
    struct page* last;
};

static int base_addr = 0x20000000;
struct page* list[7];

int check(int index){
    if(index > 6 || index < 0){
        return -1;
    }
    else if(list[index] == NULL){
        return 0;
    }
    return 1;
}

int seek(int ID, int check_size){
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
    struct page* left = malloc(sizeof(struct page));
    struct page* right = malloc(sizeof(struct page));
    left->id = tmp->id*2;
    left->addr = tmp->addr;
    left->next = right;
    left->size = target-1;
    left->last = NULL;
    right->id = tmp->id*2+1;
    right->addr = tmp->addr + pow(2, target-1)*4096;
    right->next = NULL;
    right->size = target-1;
    right->last = left;
    list[target] = tmp->next;
    free(tmp);
    list[target-1] = left;
    /////////////////////////
    printf("split\n");
    print();
    /////////////////////////
    return 1;
}

void merge(struct page* p, int pos, int lorr){
    struct page* tmp;
    if(pos == -1){
        tmp = list[p->size];
        if(tmp == NULL){
            list[p->size] = p;
            //////////////////////////
            printf("just link\n");
            print();
            //////////////////////////
            return;
        }
        if(tmp->id > p->id){
            p->next = tmp;
            tmp->last = p;
            p->last = NULL;
            list[p->size] = p;
            //////////////////////////
            printf("just link\n");
            print();
            //////////////////////////
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
        //////////////////////////
        printf("just link\n");
        print();
        //////////////////////////
        return;
    }
    else if(pos == 0){
        tmp = list[p->size];
        /////////////////////////////////////////////////
        printf("merge %0x %0x\n", p->addr, tmp->addr);
        ////////////////////////////////////////////////
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
        /////////////////////////////////////////
        printf("before merge\n");
        print();
        ///////////////////////////////////////////
        printf("%d\n", pos);
        merge(p, pos, lorr);
        return;
    }
    else{
        tmp = list[p->size];
        for(int i = 0; i < pos; i++){
            tmp = tmp->next;
        }
        /////////////////////////////////////////////////
        printf("merge %0x %0x\n", p->addr, tmp->addr);
        ////////////////////////////////////////////////
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
        /////////////////////////////////////////
        printf("before merge\n");
        print();
        ///////////////////////////////////////////
        printf("%d\n", pos);
        merge(p, pos, lorr);
        return;
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
    ///////////////////////////////////
    printf("get %0x\n", tmp->addr);
    print();
    /////////////////////////////////
    return tmp;
}

void space_free(struct page* done){
    /////////////////////////
    printf("free addr %0x\nfree id %d\n", done->addr, done->id);
    ///////////////////////////
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
}
//////////////////
void print(){
    for(int i = 0; i < 7; i++){
        printf("list[%d]: ", i);
        struct page* tmp = list[i];
        while(tmp != NULL){
            printf("%0x(%d) ", tmp->addr, tmp->id);
            tmp = tmp->next;
        }
        if(tmp == NULL){
            printf("NULL\n");
        }
    }
    printf("\n");
    return;
}
//////////////////
int main(){
    struct page* all = malloc(sizeof(struct page));
    all->addr = 0x00000000 + base_addr;
    all->id = 0;
    all->next = NULL;
    all->size = 6;
    all->last = NULL;
    list[6] = all;
    ///////////////////////////
    print();
    int s;
    struct page* test[5];
    for(int i = 0; i < 5; i++){
        scanf("%d", &s);
        test[i] = page_locate(s);
    }
    for(int i = 0; i < 5; i++){
        space_free(test[i]);
    }
    ///////////////////////////
    return 0;
}
