extends Node

const Example = preload("res://gdnative/example_example.gdns")

onready var _example_instance = $ExampleNode


func _ready() -> void:
	var example = GDNative.new()
	example.library = preload("res://gdnative/example.gdnlib")
	example.initialize()  # --> "GDNative initialized"
	print(example.call_native("standard_varcall", "get_message", []))  # --> "Hello world!"
	print(example.call_native("standard_varcall", "square", [5]))  # --> 25
	print(example.call_native("standard_varcall", "sum_ints", [[1, 2.5, 3]]))  # --> 6
	
	_example_instance.connect("message_set", self, "_on_message_set")
	print(_example_instance.message)  # --> "Scene message" (as set in scene)
	_example_instance.message = "new message"  # "message_set" will be emitted here
	print(_example_instance.some_constant)  # --> (1, 2)

func _on_message_set(message: String) -> void:
	print("Example message set: ", message)   # --> "Example message set: \"new message\""
