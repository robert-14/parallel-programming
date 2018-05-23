#define uint32_t unsigned int
static inline uint32_t rotate_left(uint32_t x, uint32_t n) {
    return  (x << n) | (x >> (32-n));
}
static inline uint32_t encrypt(uint32_t m, uint32_t key) {
    return (rotate_left(m, key&31) + key)^key;
}

__kernel void vecdot( uint32_t keyA, uint3ftr2_t keyB, __global int *C) 
{
    __local int buf[256]; 
    int global_idx = get_global_id(0);
    int group_idx = get_group_id(0);
    int local_idx = get_local_id(0);
    int local_size = get_local_size(0);

    buf[local_idx] = encrypt(global_idx, keyA) * encrypt(global_idx, keyB);

    barrier(CLK_LOCAL_MEM_FENCE );
    for( int  i = local_size/2; i ; i = i/2){
        if( local_idx < i)
            buf[local_idx] += buf[local_idx + i];
        barrier(CLK_LOCAL_MEM_FENCE );
    }
    if( !local_idx) C[group_idx] = buf[local_idx];



}