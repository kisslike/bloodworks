#pragma once

#include <vector>

template <typename T>
class cVector : public std::vector<T>
{

public:
	cVector() {}
	cVector(int n) : std::vector<T>(n) {}

	int size() const
	{
		return (int)std::vector<T>::size();
	}

	void removeElement(const T& value)
	{
		erase(std::remove(T::begin(), T::end(), value), T::end());
	}

	void swapToTailRemoveElement(const T& value)
	{
		for (int i = 0; i < size(); i++)
		{
			if ((*this)[i] == value)
			{
				swapToTailRemove(i);
				return;
			}
		}
	}

	void swapToTailRemove(int i)
	{
		(*this)[i] = (*this)[size() - 1];
		resize(size() - 1);
	}

	T& insertAndGetReference()
	{
		resize(size() + 1);
		return (*this)[size() - 1];
	}

	void setAll(const T& value)
	{
		for (int i = 0; i < size(); i++)
		{
			(*this)[i] = value;
		}
	}

	int findIndex(const T& value)
	{
		auto it = std::find(begin(), end(), value);
		if (it != end())
		{
			return (int)std::distance(begin(), it);;
		}
		return -1;
	}
};