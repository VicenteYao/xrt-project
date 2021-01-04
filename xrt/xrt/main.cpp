// xrt.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include<stdint.h>
#include<Windows.h>




enum md_element_type {
    md_element_type_void,
    md_element_type_uint8,
    md_element_type_uint16,
    md_element_type_uint32,
    md_element_type_uint64,
    md_element_type_int8,
    md_element_type_int16,
    md_element_type_int32,
    md_element_type_int64,
    md_element_type_single,
    md_element_type_double,
    md_element_type_array,
    md_element_type_object,
    md_element_type_string,
    md_element_type_delegate,
};

struct md_method_table;
typedef md_method_table* ptr_md_method_table;

typedef struct vm_primitive_type {
    LPCWSTR full_name;
    ptr_md_method_table method_table;
};


typedef struct vm_array_object {
    uint64_t length;
    uint8_t elements[];
}*ptr_gc_array_object;



enum md_member_visibility
{
    md_member_visibility_public,
    md_member_visibility_private,
    md_member_visibility_protected,
    md_member_visibility_internal,
    md_member_visibility_protected_internal
};

enum md_parameter_direction {
    in,
    out,
    ref
};

typedef struct md_parameter_decs {
    ptr_md_method_table paramter_type;
    md_parameter_direction attributes;
    void* default_value;
};

enum md_method_attributes {
    instance_method,
    static_method,
    local_method,
    tail_recursive,
};

typedef struct md_member_desc {
    ptr_md_method_table instance_type;
    md_member_visibility visibility;
    LPCWSTR name;
};

typedef struct md_method_desc {
    md_member_desc base_info;
    ptr_md_method_table return_type;
    md_method_attributes attributes;
    uint16_t parameters_count;
    uint16_t generic_parametrs_count;
    uint16_t parametrs_count2;
    md_parameter_decs* parameters;
};

enum md_field_attributes {
    instance_field,
    static_field,
};



typedef struct md_field_desc {
    ptr_md_method_table field_type;
    ptr_md_method_table instance_type;
};

typedef struct md_property_desc {
    md_method_desc* get_method;
    md_method_desc* set_method;
};

typedef struct md_interface_mapping {

};



typedef struct gc_allocation_info {
    uint8_t numa_node_allocated : 1;
    uint8_t region_node_allocated : 1;
    uint8_t page0_allocated : 1;
    uint8_t page1_allocated : 1;
    uint8_t page3_allocated : 1;
    uint8_t page2_allocated : 1;
    uint8_t page4_allocated : 1;
    uint8_t numa_node;
    uint8_t region_index;
    uint8_t page_indices[5];
};

typedef struct md_method_table {
    md_method_table* base;
    gc_allocation_info allocation_info;
    const wchar_t* name;
    md_element_type element_type;
    uint16_t methods_count;
    uint16_t fields_count;
    uint16_t property_count;
    uint16_t interface_count;
    uint16_t generic_paramters_count;
    md_method_desc* methods;
    md_field_desc* fields;
    md_property_desc* properties;
    md_method_table** generic_parameters;
    md_interface_mapping* interface_mappings;
}* ptr_md_method_table;

typedef struct vm_object_address {
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
}object_address;



typedef struct gc_object_adress_forward_record
{
    vm_object_address from;
    vm_object_address to;
}*ptr_object_address_forward_record;

typedef struct gc_page_base {
    struct
    {
        uint8_t allocated : 1;
        uint8_t has_method_table : 1;
    }flags;
}*ptr_gc_page_base;

typedef struct gc_object_page
{
    gc_page_base base;
    ptr_md_method_table method_table;
    uint32_t object_size;
    uint32_t active_forward_records_count;
    uint32_t max_allocated_count;
    uint32_t allocated_count;
    ptr_object_address_forward_record forward_records;
    void* next_object_ptr;
    void* first_object_ptr;
}*ptr_gc_object_page;


enum gc_blob_state {
   
};

typedef struct gc_blob_page_entry {
    vm_object_address owner;
    struct gc_blob_page_entry* next;
    size_t start;
    size_t length;
    uint8_t finalizable;
    uint8_t zero_filled;
    uint8_t in_zero_thread_queue;
    uint8_t d;
    uint8_t data[1];
}*ptr_blob_page_entry;



typedef struct gc_blob_page
{
    gc_page_base base;
    uint64_t max_allocated_size;
    uint64_t allocated_size;
    uint64_t active_records_count;
    uint64_t finalizable_records_count;
    struct gc_blob_page_entry* next_record;
    struct gc_blob_page_entry* records;
}*ptr_gc_blob_page;

enum gc_region_flags
{
    free_region,
    object_region,
    blob_region,
};

enum gc_region_size :uint16_t {
    small_region_size,
    medium_region_size,
    large_region_size,
};

typedef struct gc_region {
    void* start_address;
    gc_region_flags flags;
    int32_t active_page_index;
    int32_t active_pages_count;
    uint64_t  size;
    union {
        struct gc_object_page object_pages[256];
        struct gc_blob_page blob_pages[256];
    };
}* ptr_gc_region;

typedef struct gc_numa_node {
    int32_t active_regions_count;
    int32_t active_object_region_index;
    int32_t active_blob_region_index;
    int32_t first_free_region_index;
    struct gc_region regions[256];
}*ptr_gc_numa_node, gc_numa_node;

typedef struct gc_heap {
    uint32_t numa_node_count;
    uint32_t active_numa_node_count;
    uint64_t gc_heap_maximum_size;
    uint64_t gc_heap_allocated_size;
    struct gc_numa_node nodes[];
};

typedef struct gc_instance
{
    void* file_mapping_handle;
    size_t file_map_size;
    uint32_t file_mapping_flags;
    uint32_t file_mapping_size_hign;
    uint32_t file_mapping_size_low;
    uint32_t numa_hignest_node_number;
    uint32_t page_size;
    uint32_t gc_instance_size;
    void* gc_instance_base_address;
    void* os_new_base_address;
    void* vm_address;
    void* finalizable_view_base_address;
    void* finalizable_view_maximum_address;
    void* relocation_view_base_address;
    void* relocation_view_maximum_address;
    void* map0_view_base_address;
    void* map0_view_maximum_address;
    void* map1_view_base_address;
    void* map1_view_maximum_address;
    gc_heap gc_heap;
}* ptr_gc_instance;

static ptr_gc_instance g_gc_instance = 0;

static ptr_md_method_table vm_get_object_method_table(vm_object_address obj) {
    int size = sizeof(gc_blob_page_entry);
    uint64_t start_address = (obj.address_bits >> 40);
    ptr_gc_object_page heap_address = (ptr_gc_object_page)(start_address - sizeof(ptr_gc_object_page));
    return heap_address->method_table;
}



static void  gc_object_free(object_address ptr) {

    return;
}

typedef int(*enum_region_callback)(uint32_t region_number, ptr_gc_region ptr_gc_region, void* parameter);
typedef int(*enum_numa_node_callback)(uint32_t numa_node_number, ptr_gc_numa_node ptr_gc_numa_node, void* parameter);
typedef int(*enum_page_callback)(uint32_t page_number, ptr_gc_page_base ptr_gc_page, void* parameter);


static int gc_enum_regions(gc_numa_node* ptr_gc_numa_node, enum_region_callback callback, void* parameter) {
    if (g_gc_instance == nullptr)
    {
        return -1;
    }
    if (ptr_gc_numa_node == nullptr)
    {
        return -1;
    }
    int region_number = 0;
    int regions_count = sizeof(ptr_gc_numa_node->regions) / sizeof(ptr_gc_numa_node->regions[0]);
    for (; region_number != regions_count; region_number++)
    {
        int result = callback(region_number, &(ptr_gc_numa_node->regions[region_number]), parameter);
        if (!result)
        {
            break;
        }
    }
    return region_number;
}

static int gc_enum_numa_nodes(enum_numa_node_callback callback, void* parameter) {
    if (g_gc_instance == nullptr)
    {
        return -1;
    }
    int numa_node_number = 0;
    for (; numa_node_number != g_gc_instance->gc_heap.active_numa_node_count; numa_node_number++)
    {
        int result = callback(numa_node_number, &(g_gc_instance->gc_heap.nodes[numa_node_number]), parameter);
        if (!result)
        {
            break;
        }
    }
    return numa_node_number;
}

static int gc_enum_pages(gc_region* ptr_gc_region, enum_page_callback callback, void* paramter) {
    if (g_gc_instance == nullptr)
    {
        return -1;
    }
    if (ptr_gc_region == nullptr)
    {
        return -1;
    }

    int page_number = 0;
    int pages_count = sizeof(ptr_gc_region->object_pages) / sizeof(ptr_gc_region->object_pages[0]);
    for (; page_number != pages_count; page_number++)
    {
        int result = callback(page_number, (ptr_gc_page_base) & (ptr_gc_region->object_pages[page_number]), paramter);
        if (!result)
        {
            break;
        }
    }
    return page_number;
}

static bool vm_enum_method_table_members() {
    return false;
}

static size_t os_get_large_page_minimum_size() {
    return 0;
}

static bool os_is_large_pages_supported() {

    return os_get_large_page_minimum_size() > 0;
}

static void* os_create_file_mapping() {
    HANDLE hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr,
        g_gc_instance->file_mapping_flags,
        g_gc_instance->file_mapping_size_hign,
        g_gc_instance->file_mapping_size_low, nullptr);
    return hFileMapping;
}

static void* os_map_view_of_file(void* hFileMapping, void* pBaseAddress) {
    void* pAddress = MapViewOfFileEx(hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, g_gc_instance->file_map_size, pBaseAddress);
    return pAddress;
}

static void  os_unmap_view_of_file(void* pBaseAddress) {
    UnmapViewOfFile(pBaseAddress);
}


static bool vm_object_write_value(vm_object_address address, size_t offset, uint64_t value) {

}

static bool vm_object_read_value(vm_object_address address, size_t offset, uint64_t* value) {

}

static bool md_get_element_size(md_element_type element_type) {
    size_t element_size = 0;
    switch (element_type)
    {
    case md_element_type_void:
        break;
    case md_element_type_int8:
    case md_element_type_uint8:
        element_size = 1;
        break;
    case md_element_type_int16:
    case md_element_type_uint16:
        element_size = 2;
        break;
    case md_element_type_int32:
    case md_element_type_uint32:
        element_size = 4;
        break;
    case md_element_type_int64:
    case md_element_type_uint64:
        element_size = 8;
        break;
    case md_element_type_single:
        element_size = 4;
        break;
    case md_element_type_double:
        element_size = 8;
        break;
    case md_element_type_array:
        element_size = 4;
        break;
    case md_element_type_object:
        break;
    case md_element_type_string:
        break;
    default:
        break;
    }
    return element_size;
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

static void* os_malloc(size_t size) {
    if (g_gc_instance == nullptr)
    {
        return nullptr;
    }
    void* ptr = VirtualAlloc(g_gc_instance->os_new_base_address, size, MEM_COMMIT, PAGE_READWRITE);
    g_gc_instance->os_new_base_address = (void*)((size_t)g_gc_instance->os_new_base_address + size);
    return ptr;
}

static void* os_realloc() {
    return nullptr;
}

static void os_free(void* ptr) {
    
}

static void rt_invoke_method(md_method_desc* methodDesc) {

}

typedef void (*thread_start_routine)(void* param);

struct create_thread_parameters {
    void* param;
    thread_start_routine routine;
};

static  DWORD WINAPI thread_proc(void* lParam) {
    create_thread_parameters* parameters = (create_thread_parameters*)lParam;
    parameters->routine(parameters->param);
    os_free(parameters);
    return 0;
}

static thread_t os_create_thread(thread_start_routine routine, void* thread_parameter) {

    create_thread_parameters* parameters = (create_thread_parameters*)os_malloc(sizeof(create_thread_parameters));
    parameters->param = thread_parameter;
    parameters->routine = routine;
    DWORD thread_id = 0;
    HANDLE thread_handle = CreateThread(nullptr, 0, thread_proc, thread_parameter, 0, &thread_id);
    return thread_handle;
}

static uint32_t os_get_current_thread_id() {
    return GetCurrentThreadId();
}

static uint32_t os_get_current_process_id() {
    return GetCurrentProcessId();
}

static bool os_suspend_thread(thread_t thread) {
    return false;
}

static bool os_resume_thread(thread_t thread) {
    return false;
}

static uint32_t os_get_numa_hignest_node_number() {
    ULONG numa_number = 0;
    GetNumaHighestNodeNumber(&numa_number);
    return numa_number;
}





static bool gc_initialize() {
    if (g_gc_instance == nullptr)
    {
        gc_instance gc_instance_object;
        g_gc_instance = &gc_instance_object;
        gc_instance_object.vm_address = 0x0;

        SYSTEM_INFO system_info;
        GetSystemInfo(&system_info);
        gc_instance_object.file_mapping_flags = PAGE_READWRITE | SEC_RESERVE;
        gc_instance_object.file_mapping_size_hign = 0x400;
        gc_instance_object.file_mapping_size_low = 0x00000000;
        gc_instance_object.file_map_size = 0x10000000000;
        gc_instance_object.finalizable_view_base_address = (void*)0x200000000000;
        gc_instance_object.relocation_view_base_address = (void*)0x100000000000;
        gc_instance_object.map1_view_base_address = (void*)0x80000000000;
        gc_instance_object.map0_view_base_address = (void*)0x40000000000;
        gc_instance_object.file_mapping_handle = os_create_file_mapping();
        gc_instance_object.finalizable_view_base_address = os_map_view_of_file(gc_instance_object.file_mapping_handle, gc_instance_object.finalizable_view_base_address);
        gc_instance_object.relocation_view_base_address = os_map_view_of_file(gc_instance_object.file_mapping_handle, gc_instance_object.relocation_view_base_address);
        gc_instance_object.map1_view_base_address = os_map_view_of_file(gc_instance_object.file_mapping_handle, gc_instance_object.map1_view_base_address);
        gc_instance_object.map0_view_base_address = os_map_view_of_file(gc_instance_object.file_mapping_handle, gc_instance_object.map0_view_base_address);
        gc_instance_object.numa_hignest_node_number = os_get_numa_hignest_node_number();
        gc_instance_object.page_size = system_info.dwPageSize;
        gc_instance_object.gc_heap.numa_node_count = 1;
        gc_instance_object.gc_heap.active_numa_node_count = 1;
        gc_instance_object.gc_instance_size = sizeof(gc_instance) + sizeof(gc_numa_node) * (gc_instance_object.numa_hignest_node_number + 1);
        gc_instance_object.gc_instance_base_address = gc_instance_object.map0_view_base_address;
        gc_instance_object.os_new_base_address = gc_instance_object.map0_view_base_address;
        ptr_gc_instance p_gc_instance = (ptr_gc_instance)os_malloc(gc_instance_object.gc_instance_size);

        g_gc_instance = p_gc_instance;
    }
    return true;
}





static void gc_shutdown() {
    if (g_gc_instance == 0)
    {
        return;
    }
    gc_instance gc_instance = *g_gc_instance;
    os_unmap_view_of_file(gc_instance.finalizable_view_base_address);
    os_unmap_view_of_file(gc_instance.map0_view_base_address);
    os_unmap_view_of_file(gc_instance.map1_view_base_address);
    os_unmap_view_of_file(gc_instance.relocation_view_base_address);
    CloseHandle(gc_instance.file_mapping_handle);
}

static void gc_trigger() {

}


static object_address gc_object_alloc(ptr_gc_object_page object_page, ptr_md_method_table method_table) {
    object_address obj;
    if (object_page == nullptr || method_table == nullptr)
    {
        return obj;
    }

}

static size_t vm_calculate_method_table_fields_size(ptr_md_method_table method_table) {
    if (method_table == nullptr)
    {
        return 0;
    }
    size_t size = 0;
    ptr_md_method_table base = method_table->base;
    do
    {

    } while (true);

    return 0;
}

static ptr_gc_object_page gc_object_page_alloc(ptr_gc_region gc_region, ptr_md_method_table method_table) {
    if (gc_region == nullptr)
    {
        return nullptr;
    }
    ptr_gc_object_page page = nullptr;
    for (int i = 0; i != sizeof(gc_region->object_pages) / sizeof(gc_region->object_pages[0]); i++)
    {
        ptr_gc_object_page current_page = &gc_region->object_pages[i];
        if (!current_page->base.flags.allocated)
        {
            page = current_page;
            break;
        }
    }
    if (page != nullptr)
    {
        page->base.flags.allocated = true;
        page->base.flags.has_method_table = true;
        page->method_table = method_table;
        page->object_size = vm_calculate_method_table_fields_size(method_table);
        page->max_allocated_count = (gc_region->size) / page->object_size;
    }
    return page;
}


static int __gc_object_new_enum_gc_region_callback(uint32_t region_number, ptr_gc_region p_gc_region, void* paramter) {
    ptr_gc_region* pp_gc_region = (ptr_gc_region*)paramter;
    if (p_gc_region->flags == gc_region_flags::object_region)
    {
        *pp_gc_region= p_gc_region;
        return 1;
    }
    return -1;
}



static void gc_collect() {

}

static void* md_load_assembly(const wchar_t* pAssemblyName) {

}

static int gc_region_initialize(ptr_gc_region region, gc_region_flags flags, gc_region_size size) {
    if (region == nullptr)
    {
        return 0;
    }
    if (region->flags != gc_region_flags::free_region)
    {
        return 0;
    }
    region->flags = flags;
    region->size = 4096* 512;
    region->start_address = os_malloc(region->size);
    return 1;
}

static int  __find_first_free_region_callback(uint32_t region_number, ptr_gc_region p_gc_region, void* parameter) {
    ptr_gc_region* pp_free_region = (ptr_gc_region*)parameter;
    if (p_gc_region->flags == gc_region_flags::free_region && !p_gc_region->start_address)
    {
        *pp_free_region = p_gc_region;
        return 0;
    }
    return 1;
}

static ptr_gc_region gc_region_alloc(ptr_gc_numa_node numa_node, gc_region_flags flags, gc_region_size size) {
    if (numa_node == nullptr)
    {
        return nullptr;
    }
    ptr_gc_region free_region = nullptr;
    gc_enum_regions(numa_node, __find_first_free_region_callback, &free_region);
    if (free_region != nullptr)
    {
        gc_region_initialize(free_region, flags, size);
    }
    return free_region;
}

static vm_object_address gc_object_new(ptr_md_method_table method_table) {
    vm_object_address object_address;
    if (g_gc_instance == nullptr)
    {
        return   object_address;
    }
    gc_allocation_info allocation_info = method_table->allocation_info;
    ptr_gc_numa_node numa_node = &g_gc_instance->gc_heap.nodes[allocation_info.numa_node];
    ptr_gc_region region = &numa_node->regions[allocation_info.region_index];
    ptr_gc_object_page page = &region->object_pages[allocation_info.page_indices[0]];
    object_address = gc_object_alloc(page, method_table);
    return object_address;
}

static vm_object_address gc_array_new(ptr_md_method_table method_table, uint64_t length) {
    auto array_object = gc_object_new(method_table);
}

int main()
{
    int size = sizeof(gc_allocation_info);
    gc_initialize();
    gc_object_new(nullptr);


    gc_shutdown();
}

