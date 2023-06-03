#ifndef UNION_FIND_H
#define  UNION_FIND_H
#include "utilesWet2.h"
#include <stdexcept>
class UnionFind
{
private:
	static const int EMPTY = -1;
	int size = EMPTY;
	class Record;
	class Group;
	Record* records;
	Group* groups;

	int findGroup(int r_id, int* hight);
	
public:
	void newMonth(int* records_stocks, int number_of_records);
	StatusType getPlace(int r_id, int* column, int* hight);
	UnionFind(int* records_stocks, int number_of_records);
	UnionFind( const UnionFind& other);
	UnionFind() = default;
	~UnionFind();
	StatusType putOnTop(int r_id1, int r_id2);
	StatusType buyRecord(int r_id);
};

class UnionFind::Record {
	public :
		int parent = EMPTY;
		int buys = 0;
		int extra = 0;  
		int inStock = 0;
		Record() : buys(0), extra(0), inStock(), parent(EMPTY) {};
};

class UnionFind::Group {
	public:
		int culomn = EMPTY;
		int size = 1;
		Group() : culomn(0), size(0){};
};

#endif // UNION_FIND_H_