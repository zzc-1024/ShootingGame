#ifndef _SEQLIST_H_
#define _SEQLIST_H_
template <typename T>
class SeqList
{
	using uint = unsigned int;
public:
	SeqList(uint size = 0);
	void resize(uint size);
	uint size();
	void clear();

	T& operator[](uint index) const;

private:
	T* data;
	uint length;
};

#endif //_SEQLIST_H_


