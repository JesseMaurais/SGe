#ifndef Source_hpp
#define Source_hpp

class Source
{
	friend class ResourcesCommon;

public:

	virtual ~Source() = default;
	virtual bool UpdateSource() = 0;

protected:

	unsigned id;
};

template <class Resource> class SourceCommon : public Source
{
protected:

	SourceCommon()
	{
		id = Resource::Manager().Add(this);
	}
	~SourceCommon()
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
