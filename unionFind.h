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
	UnionFind( const UnionFind& other) = delete;
	UnionFind() = default;
	~UnionFind();
	StatusType putOnTop(int r_id1, int r_id2);
	int& operator[](int  r_id);
	int buyRecord(int r_id);
};

class UnionFind::Record {
	public :
		int parent = EMPTY; //an index in the records arrey.  "pointer" to the next record in the group search route - the record in the index. 
		//if EMPTY, pointing to the group with the same index as record index
		int buys = 0;
		int extra = 0;  //sum over exstra in the group find track gives height
		Record() : buys(0), extra(0), parent(EMPTY) {};
};

class UnionFind::Group {
	public:
		int culomn = EMPTY;
		int size = 1;
		int height = 0;
		Group() : culomn(EMPTY), size(1), height(0)
			 {};
};

#endif // UNION_FIND_H_