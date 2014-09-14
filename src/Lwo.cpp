#include "Lwo.hpp"
#include <cstdio>

static void Error(void *user, const char *msg)
{
	fprintf(stderr, "%s\n", msg);
}

bool ModelLwo::Load(const char *path)
{
	lwo_file_t *file = lwo_read(path, Error);
	if (file)
	{
	 for (lwo_chunk_t *it = file->chunks; it; it = it->next)
	 {
	  AddChunk(it);
	 }
	 lwo_destroy(file);
	 return true;
	}
	return false;
}

void ModelLwo::AddChunk(lwo_chunk_t *chunk)
{
	switch (chunk->idtag)
	{
	case lwo_PNTS_id:
		AddPoints(chunk->data.pnts, chunk->length);
		break;
	case lwo_POLS_id:
		AddPolygons(chunk->data.pols);
		break;
	}
}

void ModelLwo::AddPoints(lwo_PNTS_t &pnts, unsigned n)
{
	Vector V;

	for (int i=0; i < n; ++i)
	{
		V.x = pnts.points[i].x;
		V.y = pnts.points[i].y;
		V.z = pnts.points[i].z;

		AddVertex(V);
	}
}

void ModelLwo::AddPolygons(lwo_POLS_t &pols)
{
	for (int i=0; i < pols.poly_num; ++i)
	{
		for (lwo_poly_t *it = pols.polys+i; it; it = it->next)
		{
			Begin(GL_POLYGON);
			for (int j = 0; j < it->vert_num; ++j)
			{
				Next(it->verts[j]);
			}
			End();
		}
	}
}

