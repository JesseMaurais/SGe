#ifndef SkyBox_hpp
#define SkyBox_hpp

struct SkyBox
{
	union {
	 struct {
	  unsigned left, right, bottom, top, back, front;
	 };
	 unsigned maps[6];
	};

	void Load(const char *dir, const char *ext="jpg");
	void Free(void);
	void Draw(void);
};

#endif // file

