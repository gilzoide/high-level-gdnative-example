extends Node

func _ready() -> void:
	var example = GDNative.new()
	example.library = preload("res://gdnative/example.gdnlib")
	example.initialize()  # --> "GDNative initialized"
	print(example.call_native("standard_varcall", "get_message", []))  # --> "Hello world!"
	print(example.call_native("standard_varcall", "square", [5]))  # --> 25
	print(example.call_native("standard_varcall", "sum_ints", [[1, 2.5, 3]]))  # --> 6
