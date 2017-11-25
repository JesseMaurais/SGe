#ifndef Stream_hpp
#define Stream_hpp

#include <cstdio>
#include <string>

bool InitStream(std::FILE *file, std::string prompt, std::size_t buffer, bool strict);
void SignalStream();

#endif // file

