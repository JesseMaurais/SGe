#ifndef Source_hpp
#define Source_hpp

class Source
{
public:

	virtual ~Source() = default;
	virtual bool Update() = 0;
};

class Resource
{
public:

	virtual ~Resource() = default;
	virtual unsigned Update() = 0;
	virtual unsigned Add(Source *that) = 0;
	virtual Source *Remove(unsigned id) = 0;
	virtual bool Has(unsigned id) = 0;
};

#endif // file
