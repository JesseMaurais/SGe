#ifndef Stream_hpp
#define Stream_hpp

#include <cstdio>

bool InitStream(std::FILE *file, std::size_t buffer, bool strict);
void SignalStream();

#endif // file

