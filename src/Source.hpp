#ifndef Source_hpp
#define Source_hpp

#include <vector>

class Source
{
	friend class Resources;

public:

	virtual ~Source() = default;
	virtual bool UpdateSource() = 0;

protected:

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
	virtual unsigned Add(Source *that);
	virtual Source *Remove(unsigned id);
	bool Has(unsigned id);
	unsigned Size();

protected:

	unsigned UpdateSources();
	unsigned UpdateSources(std::vector<Source*> const &sources);
	unsigned UpdateSources(std::vector<unsigned> const &ids);

private:

	std::vector<Source*> sources;
};

#endif // file
