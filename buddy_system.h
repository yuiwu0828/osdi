struct page{
    int size;
    int id;
    void* addr;
    struct page* next;
    struct page* last;
};

extern struct page* list[7];
extern int frame_array[64];	
extern struct page* using_array[64];
extern struct page page_pool[64];

int check(int index);
int pow(int a, int b);
int split(int target);
void merge(struct page* p, int condition, int left_right);
void* page_locate(int need);
void space_free(void* d);
void print();
