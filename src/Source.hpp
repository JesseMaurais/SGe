#ifndef Source_hpp
#define Source_hpp

class Source
{
	friend class Manager;

public:

	virtual ~Source() = default;
	virtual bool UpdateSource() = 0;

protected:

	unsigned id;
};

class Resources
{
public:

	virtual ~Resources() = default;
	virtual unsigned Add(Source *that) = 0;
	virtual Source *Remove(unsigned id) = 0;
};

#endif // file
