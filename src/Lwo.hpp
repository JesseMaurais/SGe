#ifndef __Lwo__
#define __Lwo__

#include "Model.hpp"
#include <lwo.h>

struct ModelLwo : virtual Model
{
	bool Load(const char *path);

 private:

	void AddChunk(lwo_chunk_t *chunk);
	void AddPoints(lwo_PNTS_t &pnts, unsigned n);
	void AddPolygons(lwo_POLS_t &pols);
};

#endif // file

