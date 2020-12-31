// xrt.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include<Windows.h>


typedef enum element_type {
    Void,
    UInt8,
    Int8,
    UInt16,
    Int16,
    UInt32,
    Int32,
    UInt64,
    Int64,
    Single32,
    Double,
    Array,
    Object,
};

typedef struct method_table {
    method_table* base;
    element_type type;
   
}* ptr_method_table;

typedef struct object_address {
    union {
        struct
        {
            uint64_t address : 40;
            uint8_t finalizable : 1;
            uint8_t remapped : 1;
            uint8_t marked1 : 1;
            uint8_t marked0 : 1;
        };
        uint64_t address_bits : 44;
    };
    uint32_t unused : 20;
}*ptr_object;

typedef struct object_adress_forward_record
{
    object_address from;
    object_address to;
}*ptr_object_adress_forward_record;

typedef struct gc_layout_heap_base {
    ptr_method_table method_table;
    uint64_t dummy[5];
}* ptr_gc_layout_heap_base;

typedef struct gc_fixed_layout_heap
{
    ptr_method_table method_table;
    uint32_t layout_size;
    ptr_object_adress_forward_record forward_records;
    uint32_t active_forward_records_count;
    uint32_t max_alloc_count;
    size_t next_object_ptr;
    size_t first_object_ptr;
}*ptr_gc_fixed_layout_heap;

typedef struct gc_array_struct {
    uint64_t length;
    uint8_t elements[];
}* ptr_gc_array_struct;

typedef struct gc_dynamic_layout_heap
{
    ptr_method_table method_table;
    size_t active_allocated_size;
    ptr_object_adress_forward_record forward_records;
    uint32_t active_forward_records_count;
    uint32_t active_allocated_count;
    gc_array_struct* next_array_ptr;
    gc_array_struct* first_array_ptr;
}*ptr_gc_dynamic_layout_heap;

enum gc_region_flags :uint32_t
{
    large_object_heap,
};

typedef struct gc_region {
    size_t start_address;
    size_t  region_size_base;
    size_t region_factor;
    gc_region_flags flags;
    size_t active_fixed_heaps_count;    
    size_t active_dynamic_heaps_count;
    struct gc_fixed_layout_heap fixed_heaps[256];
    struct gc_dynamic_layout_heap dynamic_heaps[256];
}* ptr_gc_region;

typedef struct gc_numa_node {
    uint16_t processor_index;
    uint16_t active_regions_count;
    struct gc_region root_region;
    struct gc_region regions[255];
}* ptr_gc_numa_node;

struct gc_instance
{
    uint32_t numa_node_count;
    uint32_t active_numa_node_count;
    struct gc_numa_node nodes[];
};

static ptr_method_table vm_get_object_method_table(object_address obj) {

    uint64_t start_address = (obj.address_bits >> 40);
    ptr_gc_layout_heap_base heap_address = (ptr_gc_layout_heap_base)(start_address - sizeof(gc_layout_heap_base));
    return heap_address->method_table;
}

static bool os_is_large_pages_supported() {

    return false;
}

static size_t os_get_large_page_minimum_size() {
    return 0;
}

static void* os_create_shared_memory() {
    return nullptr;
}

static void* os_map_shared_memory() {
    return nullptr;
}

static void* os_unmap_shared_memory() {
    return nullptr;
}

static void* gc_init() {
    return nullptr;
}

static void* gc_uninit() {

    return nullptr;
}

static ptr_object gc_object_new(ptr_method_table method_table) {

    return nullptr;
}

static ptr_object gc_array_new(ptr_method_table method_table, uint64_t length) {

}


static void  gc_object_free(void* ptr) {
    
    return ;
}

static size_t os_get_processors_count() {
    return 0;
}

typedef void* thread_t;

static bool os_get_thread_context(thread_t thread, void* context) {
    return false;
}

static bool os_set_thread_context(thread_t thread,void* context) {
    return false;
}

typedef void (*fn_thread_proc)(void* param);

static thread_t os_create_thread(fn_thread_proc fn, void* param);

static uint32_t os_get_current_thread_id() {
    return 0;
}

static uint32_t os_get_current_process_id() {
    return 0;
}

static bool os_suspend_thread(thread_t thread) {
    return false;
}

static bool os_resume_thread(thread_t thread) {
    return false;
}


int main()
{
    std::wcout << L"Hello xrt!" << std::endl;
}

