#define HGDN_STATIC
#define HGDN_IMPLEMENTATION
#include "hgdn.h"

const char *MESSAGE = "Hello world!";

GDN_EXPORT godot_real square(godot_real x) {
    return x * x;
}

GDN_EXPORT int sum_ints(const godot_int *buffer, size_t size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += buffer[i];
    }
    return sum;
}

godot_variant native_callback(void *symbol, godot_array *array) {
    if (symbol == &MESSAGE) {
        // `hgdn_new_variant` is type-aware using C11 _Generic/C++ overloads
        // In this case, it calls `hgdn_new_cstring_variant`
        return hgdn_new_variant(MESSAGE);
    }
    else if (symbol == &square) {
        // returns null and prints an error if array size < 1
        HGDN_ASSERT_ARRAY_SIZE(array, 1);
        godot_real arg0 = hgdn_array_get_real(array, 0);
        godot_real result = square(arg0);
        // Overloaded to `hgdn_new_real_variant(result)`
        return hgdn_new_variant(result);
    }
    else if (symbol == &sum_ints) {
        HGDN_ASSERT_ARRAY_SIZE(array, 1);
        hgdn_int_array int_array = hgdn_array_get_int_array(array, 0);
        int res = sum_ints(int_array.ptr, int_array.size);
        hgdn_int_array_destroy(&int_array);
        // Overloaded to `hgdn_new_int_variant(result)`
        return hgdn_new_variant(res);
    }
    // Overloaded to `hgdn_new_object_variant(NULL)`, which returns a nil Variant
    return hgdn_new_variant(NULL);
}

GDN_EXPORT void godot_gdnative_init(godot_gdnative_init_options *options) {
    // `hgdn_gdnative_init` needs to be called before any other HGDN call,
    // as it populates the global API pointers from options
    hgdn_gdnative_init(options);
    hgdn_core_api->godot_register_native_call_type("native", &native_callback);
    // `hgdn_print` uses `printf` formatted values
    hgdn_print("GDNative initialized%s", options->in_editor ? " in editor" : "");
}

GDN_EXPORT void godot_gdnative_terminate(godot_gdnative_terminate_options *options) {
    hgdn_gdnative_terminate(options);
}
