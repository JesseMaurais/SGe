#include "Render.hpp"
#include "OpenGL.hpp"

void Render::DrawGroup(Group group)
{
	Point points[3];
	for (int it = 0, index = group.first; it < group.count; ++it, ++index)
	{
	 GetPoints(index, points);
	 for (auto &item : points)
	 {
	  glVertex3dv(vertexes.at(item.vertex).v);
	 }
	}
}

void Render::Draw()
{
	glBegin(GL_TRIANGLES);
	for (auto item : Mesh::groups)
	{
	 glPushName(item.first);
	 DrawGroup(item.second);
	}
	glEnd();
}

