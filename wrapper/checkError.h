#pragma once 

#ifdef DEBUG
#define GL_CALL(func) do { func; checkError(#func, __FILE__, __LINE__); } while (false)
#else
#define GL_CALL(func) do { func; } while (false)
#endif

void checkError(const char* func, const char* file, int line);