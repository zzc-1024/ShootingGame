#ifndef _SEQLIST_H_
#define _SEQLIST_H_
template <typename T>
class SeqList
{
public:
	SeqList(unsigned int size = 0);
	~SeqList();
	void resize(unsigned int size);
	unsigned int size();
	void clear();

	T& operator[](unsigned int index) const;
	void operator=(const SeqList<T>& val);

private:
	T* data;
	unsigned int length;
};

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
inline SeqList<T>::~SeqList()
{
	/*if (data)
		delete[] data;
	data = nullptr;*/
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

template<typename T>
inline void SeqList<T>::operator=(const SeqList<T>& val)
{
	this->resize(val->size());
	for (int i = 0; i < this->length; i++)
	{
		this->data[i] = val[i];
	}
}


#endif //_SEQLIST_H_


