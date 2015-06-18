#include <stdio.h>
#include "Halide.h"

using namespace Halide;

// Check that assertion failures free allocations appropriately

int malloc_count = 0;
int free_count = 0;

void *my_malloc(void *user_context, size_t x) {
    __sync_fetch_and_add(&malloc_count, 1);
    void *orig = malloc(x+32);
    void *ptr = (void *)((((size_t)orig + 32) >> 5) << 5);
    ((void **)ptr)[-1] = orig;
    return ptr;
}

void my_free(void *user_context, void *ptr) {
    __sync_fetch_and_add(&free_count, 1);
    free(((void**)ptr)[-1]);
}

bool error_occurred = false;
void my_error_handler(void *user_context, const char *) {
    error_occurred = true;
}

int main(int argc, char **argv) {
    Func f, g, h;
    Var x;

    f(x) = x;
    f.compute_root();
    g(x) = f(x)+1;
    g.compute_root();
    h(x) = g(x)+1;

    // This should fail an assertion at runtime after f has been allocated
    int g_size = 100000;
    g.bound(x, 0, g_size);

    h.set_custom_allocator(my_malloc, my_free);
    h.set_error_handler(my_error_handler);

    Image<int> im = h.realize(g_size+100);

    printf("%d %d\n", malloc_count, free_count);

    assert(error_occurred && malloc_count == free_count);

    printf("Success!\n");
    return 0;
}
