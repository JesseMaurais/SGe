#ifndef Height_hpp
#define Height_hpp

#include "Model.hpp"

struct HeightMap : virtual Model
{
	bool Load(const char *path);
	void Free();
	void Draw();

 protected:

	int **index, width, height;
};

#endif // file

