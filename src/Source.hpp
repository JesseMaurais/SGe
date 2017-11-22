#ifndef Source_hpp
#define Source_hpp

class Source
{
	friend class Resources;

public:

	virtual ~Source() = default;
	virtual bool UpdateSource() = 0;

	unsigned id;
};

template <class Resource> class ManagedSource : public Source
{
protected:

	ManagedSource()
	{
		id = Resource::Manager().Add(this);
	}
	~ManagedSource()
	{
		auto const that = Resource::Manager().Remove(id);
		assert(that == this);
	}
};

class Resources
{
public:

	virtual ~Resources() = default;
	virtual unsigned Add(Source *that) = 0;
	virtual Source *Remove(unsigned id) = 0;
};

#endif // file
