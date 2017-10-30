#ifndef Source_hpp
#define Source_hpp

class Source
{
	friend class Manager;

public:

	virtual ~Source() = default;
	virtual bool Update() = 0;

protected:

	unsigned id;
};

class Resource
{
public:

	virtual ~Resource() = default;
	virtual unsigned Add(Source *that) = 0;
	virtual Source *Remove(unsigned id) = 0;
};

#endif // file
