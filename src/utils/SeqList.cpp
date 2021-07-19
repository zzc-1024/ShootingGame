/*#include "SeqList.h"
template<typename T>
inline SeqList<T>::SeqList(unsigned int size)
{
	this->length = size;
	if (size)
		this->data = new T[size];
	else
		this->data = nullptr;
}

template<typename T>
void SeqList<T>::resize(unsigned int size)
{
	if (this->data)
		delete[] this->data;

	this->length = size;

	if (size)
		this->data = new T[size];
	else
		this->data = nullptr;
}

template<typename T>
unsigned int SeqList<T>::size()
{
	return length;
}

template<typename T>
void SeqList<T>::clear()
{
	this->resize(0);
}

template<typename T>
T& SeqList<T>::operator[](unsigned int index) const
{
	return data[index];
}

*/
