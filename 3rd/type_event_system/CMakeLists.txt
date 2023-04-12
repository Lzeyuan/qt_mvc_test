add_library(
	type_event_system
	"type_event_system.cpp"
)

target_include_directories(type_event_system PUBLIC "/")

target_link_libraries(type_event_system
	PRIVATE
	Qt5::Core
)