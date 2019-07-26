#ifndef HEADER_DSO_CONF_H
# define HEADER_DSO_CONF_H

#if defined(_WIN32)

# define DSO_WIN32
# define DSO_EXTENSION ".dll"

#elif defined(__APPLE__)

# define DSO_APPLE
# define DSO_EXTENSION ".dylib"

#else
	
# define DSO_UNIX
# define DSO_EXTENSION ".so"

#endif

#endif