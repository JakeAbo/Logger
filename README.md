# Logger
Modern C++ Logger - Logger developed under visual studio 2019 using Modern C++ 17.
Support different severity levels, auto, generic, scalable and easy to use logger.
The module is tested.

----
*Author*:      [Jack Abulafia](https://www.linkedin.com/in/jackabu)

## Dependencies
The library requires no dependencies.

## Building 
The library is header-only. 

## How to use
1) Clone the repository.
2) Run the main which run the tester and check everything is passed.

## Getting Started
Pre-defined Macros in Log.hpp makes usage easier:
```cpp
#define LOG_INFO	Log(__FILE__, __LINE__, __FUNCTION__).info()
#define LOG_NOTICE	Log(__FILE__, __LINE__, __FUNCTION__).notice()
#define LOG_DEBUG	Log(__FILE__, __LINE__, __FUNCTION__).debug()
#define LOG_ERROR	Log(__FILE__, __LINE__, __FUNCTION__).error()
```

And you can use it like:
```cpp
#include "Log.hpp"

void example()
{
	LOG_INFO	<< "hello" << std::endl;
	LOG_NOTICE	<< "hello" << std::endl;
	LOG_DEBUG	<< "hello" << std::endl;
	LOG_ERROR	<< "hello" << std::endl;
}
```

Notice: you should always end with std::endl which simply push the msg to the queue.

## License 
This project is open source and freely available.

  Copyright (c) 2020, Jack Abulafia 
  All rights reserved.
