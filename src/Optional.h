#ifndef OPTIONAL_H
#define OPTIONAL_H

#ifdef OPTIONAL_17
	#include <optional>
#else
	#include <experimental/optional>
		
	namespace std
	{
		template<class T> using optional = std::experimental::optional<T>;
	}
#endif

#endif
