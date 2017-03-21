#ifndef Ply_hpp
#define Ply_hpp

#include "Model.hpp"

struct ModelPly : virtual Model
{
	bool Load(const char *path);
};

#endif // __Ply__

