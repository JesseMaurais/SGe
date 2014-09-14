#include "VBO.hpp"

template <class C>
 void BufferData(GLenum target, std::vector<C> &list, GLenum usage)
{
	void *data = (void*) list.data();
	size_t size = sizeof(C)*list.size();
	glBufferData(target, size, data, usage);
}

void VBO::Enable()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	/*
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	*/
}

void VBO::Disable()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	/*
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	*/
}

void VBO::GenBuffers(GLenum usage)
{
	glGenBuffers(5, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, vertex);
	BufferData(GL_ARRAY_BUFFER, Mesh::vertexes, usage);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, texCoord);
	BufferData(GL_ARRAY_BUFFER, Mesh::texCoords, usage);
	glBindBuffer(GL_ARRAY_BUFFER, normal);
	BufferData(GL_ARRAY_BUFFER, Mesh::normals, usage);
	glBindBuffer(GL_ARRAY_BUFFER, color);
	BufferData(GL_ARRAY_BUFFER, Mesh::colors, usage);
	*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element);
	BufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh::faces, usage);
}

void VBO::DeleteBuffers()
{
	glDeleteBuffers(5, buffers);
}

void VBO::BindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertex);
	glVertexPointer(3, GL_DOUBLE, 0, NULL);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, texCoord);
	glTexCoordPointer(3, GL_DOUBLE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, normal);
	glNormalPointer(GL_DOUBLE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, color);
	glColorPointer(3, GL_DOUBLE, 0, NULL);
	*/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element);
}

void VBO::Draw(int group)
{
	Group &G = Mesh::groups[group];
	glDrawElements(GL_TRIANGLES, G.count*3, GL_UNSIGNED_INT, G.first*3);
}

