// xrt.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

static bool is_large_pages_supported() {
    return false;
}

static size_t get_large_page_minimum_size() {
    return 0;
}

static void* create_shared_memory() {
    return nullptr;
}

static void* map_shared_memory() {
    return nullptr;
}

static void* unmap_shared_memory() {
    return nullptr;
}

typedef struct method_table {
   
}* ptr_method_table;

struct object_address {
    uint8_t unused : 20;
    uint8_t Finalizable : 1;
    uint8_t Remapped : 1;
    uint8_t Marked1 : 1;
    uint8_t Marked0 : 1;
    uint8_t virtual_address : 40;
};

struct gc_typed_layout_node
{
    struct
    {
        uint32_t method_table_index : 20;
        size_t start_address : 44;
    };
    struct
    {
        uint32_t layout_size : 20;
        size_t next_obj_ptr : 44;
    };
};



struct gc_region {
    struct
    {
        size_t active_node_count : 8;
        size_t active_node_count1 : 12;
        size_t      start_address : 44;
    };
    struct
    {
        size_t  region_size : 24;
        size_t region_factor : 8;
        gc_region_flags flags : 32;
    };
    struct gc_typed_layout_node nodes[256];
};

enum gc_region_flags :uint32_t
{
    large_object_heap,

};

struct gc_numa_node {
    uint16_t processor_index;
    uint16_t gc_regions_count;
    struct gc_region regions[];
};

struct gc_instance
{

};

static void* gc_init() {

}

static void* gc_uninit() {

}

static void* gc_alloc(size_t size) {

}

static void  gc_free(void* ptr) {

}

static size_t get_processors_count() {
    return 0;
}

static bool get_thread_context(void* context) {

}

static bool set_thread_context(void* context) {

}

static uint32_t get_current_thread_id() {
    return 0;
}

static uint32_t get_current_process_id() {
    return 0;
}

static bool suspend_thread() {

}

static bool resume_thread() {

}

int main()
{
    std::wcout << L"Hello xrt!" << std::endl;
}

