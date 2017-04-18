#ifndef Source_hpp
#define Source_hpp

class Source
{
private:

	virtual ~Source() = default;

public:

	virtual bool Update() = 0;
	virtual void Rename(unsigned id) = 0;
};

class Resource
{
private:

	virtual ~Resource() = default;

public:

	virtual bool Enabled() = 0;
	virtual unsigned Update() = 0;
	virtual unsigned Add(Source *that) = 0;
	virtual Source *Remove(unsigned id) = 0;
};

#endif // file
