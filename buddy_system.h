struct page{
    int size;
    int id;
    void* addr;
    struct page* next;
    struct page* last;
};
int check(int index);
void system_init();
int pow(int a, int b);
int split(int target);
void merge(struct page* p, int condition, int left_right);
void* page_locate(int need);
void space_free(void* d);
