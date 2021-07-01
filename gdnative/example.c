// 1) #define HGDN_IMPLEMENTATION on exactly one C/C++ file and include hgdn.h
// Optionally #define other compile-time options, check out hgdn.h for documentation
#define HGDN_STATIC
#define HGDN_IMPLEMENTATION
#include "hgdn.h"

// 2.a) Declare native functions to be used by script code, if there are any
//      Any function with prototype `godot_variant (godot_array *)` can be called in script with
//      `gdnative_instance.call_native("standard_varcall", "<name of your C function", ["some", "arguments", "..."])`
GDN_EXPORT godot_variant get_message(godot_array *args) {
    // `hgdn_new_variant` is type-aware using C11 _Generic/C++ overloads
    // In this case, it calls `hgdn_new_cstring_variant`
    return hgdn_new_variant("Hello world!");
}

GDN_EXPORT godot_variant square(godot_array *args) {
    // returns null and prints an error if array size < 1
    HGDN_ASSERT_ARRAY_SIZE(args, 1);
    godot_real x = hgdn_array_get_real(args, 0);
    // Overloaded to `hgdn_new_real_variant(x * x)`
    return hgdn_new_variant(x * x);
}

GDN_EXPORT godot_variant sum_ints(godot_array *args) {
    HGDN_ASSERT_ARRAY_SIZE(args, 1);
    hgdn_int_array int_array = hgdn_array_get_int_array(args, 0);
    int sum = 0;
    for (int i = 0; i < int_array.size; i++) {
        sum += int_array.ptr[i];
    }
    // Overloaded to `hgdn_new_int_variant(sum)`
    return hgdn_new_variant(sum);
}

// 2.b) Declare NativeScript methods, if there are any (TODO)
typedef struct {
    hgdn_string message;
} example_class_data;

void example_destroy(godot_object *instance, void *method_data, void *data) {
    example_class_data *example = (example_class_data *) data;
    hgdn_string_destroy(&example->message);
}

void example_set_message(godot_object *instance, void *method_data, void *data, godot_variant *var) {
    example_class_data *example = (example_class_data *) data;
    example->message = hgdn_variant_get_string(var);
    hgdn_object_call(instance, "emit_signal", "message_set", var);
}

godot_variant example_get_message(godot_object *instance, void *method_data, void *data) {
    example_class_data *example = (example_class_data *) data;
    return hgdn_new_variant(example->message.ptr);
}

godot_variant example_print_message(godot_object *instance, void *method_data, void *data, int argc, godot_variant **argv) {
    if (argc == 0) {
        example_class_data *example = (example_class_data *) data;
        hgdn_print("Example message: %s", example->message.ptr);
    }
    else {
        hgdn_string message = hgdn_args_get_string(argv, 0);
        hgdn_print("Argument message: %s", message.ptr);
        hgdn_string_destroy(&message);
    }
    return hgdn_new_nil_variant();
}


// 3.a) Add `godot_gdnative_init`, the function that will be called when Godot
// initializes this GDNativeLibrary, and call `hgdn_gdnative_init` from it.
GDN_EXPORT void godot_gdnative_init(godot_gdnative_init_options *options) {
    // `hgdn_gdnative_init` needs to be called before any other HGDN call,
    // as it populates the global API pointers from options
    hgdn_gdnative_init(options);
    // `hgdn_print` uses `printf` formatted values
    hgdn_print("GDNativeLibrary initialized%s", options->in_editor ? " in editor" : "");
}

// 3.b) Add `godot_gdnative_terminate`, the function that will be called when Godot
// unloads this GDNativeLibrary, and call `hgdn_gdnative_terminate` from it.
GDN_EXPORT void godot_gdnative_terminate(godot_gdnative_terminate_options *options) {
    hgdn_gdnative_terminate(options);
    hgdn_print("GDNativeLibrary terminated%s", options->in_editor ? " in editor" : "");
}

// 4) Add `godot_nativescript_init`, the function that will be called when Godot
// initializes a NativeScript with this GDNativeLibrary, and register classes,
// if there are any.
GDN_EXPORT void godot_nativescript_init(void *handle) {
    hgdn_class_info example_class_info = {
        "Example", "Node",
        .create = hgdn_instance_create_func_alloc(example_class_data),
        .destroy = { &example_destroy },
        .properties = hgdn_properties({
            "message", { &example_set_message }, { &example_get_message },
            .type = GODOT_VARIANT_TYPE_STRING,
            .usage = GODOT_PROPERTY_USAGE_DEFAULT,
        }, {
            "some_constant", .getter = hgdn_property_constant(((hgdn_vector2){ 1, 2 })),
        }),
        .methods = hgdn_methods({
            "print_message", { &example_print_message },
        }),
        .signals = hgdn_signals({
            "message_set", hgdn_signal_arguments({ "message", GODOT_VARIANT_TYPE_STRING }),
        }),
        .tool = false,
        .documentation = "This is an example class",
    };
    hgdn_register_class(handle, &example_class_info);
}
