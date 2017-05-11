#ifndef linear_hpp
#define linear_hpp

#include "blas.hpp"
#include <cassert>
#include <iterator>

namespace blas
{
	// Conforming matrix class with optional shared memory policy and random access container

	template <class numeric, template <class> class container, template <class> class shared> class matrix
	{
		// numeric : float, double, complex<float>, complex<double>
		// container : vector, array
		// shared : shared_ptr, weak_ptr

		friend class column_iterator;
		friend class row_iterator;

	public:

		using matrix_type = matrix<numeric, container, shared>;
		using numeric_type = numeric;
		using container_type = container<numeric_type>;
		using size_type = container_type::size_type;
		using shared_ptr = shared<container_type>;

	private:

		const unsigned M, N, inc;
		const size_type offset;
		shared_ptr pointer;

	public:

		matrix diagonal() const
		{
			matrix that = *this;
			++that.inc;
			that.M = 1;
			return that;
		}

		matrix column(const size_type column) const
		{
			matrix that = *this;
			that.offset += column;
			that.M = 1;
			return that;
		}

		matrix row(const size_type row) const
		{
			matrix that = *this;
			that.offset += row * inc;
			that.inc = 1;
			that.M = 1;
			return that;
		}

		matrix sub(const size_type M, const size_type N, const size_type row, const size_type column) const
		{
			matrix that = *this;
			that.offset += row * inc + column;
			that.M = M;
			that.N = N;
			return that;
		}

		const numeric_type &at(const size_type row, const size_type column) const
		{
			return pointer->at(offset + row * inc + column);
		}

		numeric_type &at(const size_type row, const size_type column)
		{
			return pointer->at(offset + row * inc + column);
		}

		const numeric_type *data() const
		{
			return pointer->data() + offset;
		}

		numeric_type *data()
		{
			return pointer->data() + offset;
		}

		size_type column_size() const
		{
			return M;
		}

		size_type row_size() const
		{
			return N;
		}

		size_type dim() const
		{
			return row_size();
		}

		size_type stride() const
		{
			return inc;
		}

		size_type capacity() const
		{
			return pointer->size() - offset;
		}

		size_type segment() const
		{
			return M * inc + N;
		}

		size_type use_count() const
		{
			return pointer.use_count();
		}

		// Column iterators

		struct column_iterator : matrix
		{
			void operator++()
			{
				++offset;
			}
		};

		column_iterator columns_begin() const
		{
			return column(size_type(0));
		}

		column_iterator columns_end() const
		{
			return column(column_size());
		}

		struct columns_range : matrix
		{
			column_iterator begin() const
			{
				return columns_begin();
			}

			column_iterator end() const
			{
				return columns_end();
			}
		};

		columns_range columns()
		{
			return *this;
		}

		// Row iterators

		struct row_iterator : matrix
		{
			void operator++()
			{
				offset += inc;
			}
		};

		row_iterator rows_begin() const
		{
			return row(size_type(0));
		}

		row_iterator rows_end() const
		{
			return row(row_size());
		}

		struct row_range : matrix
		{
			row_iterator begin() const
			{
				return rows_begin();
			}

			row_iterator end() const
			{
				return rows_end();
			}
		};

		row_range rows()
		{
			return *this;
		}
	};
}

#endif // file
