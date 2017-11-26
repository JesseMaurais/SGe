#ifndef Stream_hpp
#define Stream_hpp

#include <cstdio>
#include <string>

bool InitStream(std::string const &prompt, std::FILE *file, std::size_t buffer, bool strict);
void SignalStream();

#endif // file

