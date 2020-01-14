/*
 * 不是很理解 在SGI STL中的次层配置(sub-allocation)
 * 的自由链表的工作原理 故仿照实现之
 */

#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

const int ALIGN = 8;

union obj
{
    union obj * free_list_link;
    char client_data[1];
};

obj *free_list[8];

void init() 
{
    for(int i = 0; i < 8; ++i)
        free_list[i] = 0;
}

size_t inline FREELIST_INDEX(size_t bytes)
{
    return (bytes + ALIGN -1) / ALIGN -1;
}


size_t inline ROUND_UP(size_t bytes) 
{
    return (bytes + ALIGN -1) & ~(ALIGN -1);
}

void* refill(size_t n)
{
    obj * volatile * my_free_list = free_list+ FREELIST_INDEX(n);
    obj * next_obj, * current_obj;
    char * chunk = (char *)malloc(n*4);
    obj * result = (obj *)chunk;

    // 首先让 next_obj 和 my_free_list 指向第二个chunk
    *my_free_list = next_obj = (obj *)(chunk + n);
    for(int i = 1; ; i++) {
        current_obj = next_obj;
        next_obj = (obj *)((char *)next_obj + n);
        if (i == 3) {
            // 最后为 0
            current_obj->free_list_link = 0;
            break;
        } else {
            // 首尾相连接
            current_obj->free_list_link = next_obj;
        }
    }

    cout << "free_list " << n << endl;
    current_obj = *my_free_list;
    while(current_obj) {
        cout << "   value is " << current_obj << endl;
        current_obj = current_obj->free_list_link;
    }
    return result;
}


// 回收空余块
void deallocate(void *p, size_t n)
{
    obj * q = (obj *)p;
    obj * volatile * my_free_list;
    obj *current_obj;

    my_free_list = free_list + FREELIST_INDEX(n);
    q->free_list_link = *my_free_list;
    *my_free_list = q;

    cout << "free_list " << ROUND_UP(n) << endl;
    current_obj = *my_free_list;
    while(current_obj) {
        cout << "   value is " << current_obj << endl;
        current_obj = current_obj->free_list_link;
    }
}


// 分配空余块
void* allocate(size_t n)
{
    obj * volatile * my_free_list;
    obj * result, *current_obj;

    my_free_list = free_list + FREELIST_INDEX(n);
    result = *my_free_list;
    if (result == 0) {
        void *p = refill(n);
        return p;
    }
    *my_free_list = result->free_list_link;

    cout << "free_list " << ROUND_UP(n) << endl;
    current_obj = *my_free_list;
    while(current_obj) {
        cout << "   value is " << current_obj << endl;
        current_obj = current_obj->free_list_link;
    }
    return result;
}

int main(int argc, char const *argv[])
{
    init();
    int array[] = {6,12,53,23,33,58,21,13,4,54};

    for(int i = 0; i < 10; ++i) {
        int n = array[i];
        obj * volatile *my_free_list = free_list+ FREELIST_INDEX(n);
        obj * result = *my_free_list;
        if (result == 0) {
            void *p = refill(ROUND_UP(n));
            cout << p << " this is allocated to client" << endl;
            free(p);
        }
    }
    
    void *p = allocate(19);
    deallocate(p, ROUND_UP(19));
    return 0;
}
