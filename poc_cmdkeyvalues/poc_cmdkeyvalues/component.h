#ifndef COMPONENT_H
#define COMPONENT_H

// Register components

#define CREATE_COMPONENT( name ) \
	extern bool g_component_initialized_##name; \
	bool component_##name##_Init(); \
	void component_##name##_Shutdown(); \

#define REGISTER_COMPONENT_INIT( name ) \
	bool g_component_initialized_##name; \
	bool component_##name##_Init() \

#define REGISTER_COMPONENT_SHUTDOWN( name ) \
	void component_##name##_Shutdown() \

// Load components

#define INIT_COMPONENT( name ) \
	Msg( "Initializing component (" #name ")\n" ); \
	if ( component_##name##_Init() ) \
	{ \
		g_component_initialized_##name = true; \
		Msg( "Initialized component (" #name ")\n" ); \
	} \
	else \
	{ \
		component_##name##_Shutdown(); \
		g_component_initialized_##name = false; \
		Msg( "Failed to initialize component (" #name ")\n" ); \
		return 0; \
	} \

#define SHUTDOWN_COMPONENT( name ) \
	if ( g_component_initialized_##name ) \
	{ \
		component_##name##_Shutdown(); \
		g_component_initialized_##name = false; \
		Msg( "Shutdown component (" #name ")\n" ); \
	} \

#endif // COMPONENT_H