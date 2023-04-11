add_library(
	event_bus
	"event_bus.cpp"
)

target_include_directories(event_bus PUBLIC "/")

target_link_libraries(event_bus
	PRIVATE
	Qt5::Core
)