#include "UnionFind.h"
#include <stdexcept>

UnionFind::~UnionFind() {
	delete[] records;
	delete[] groups;
}

void UnionFind::newMonth(int* records_stocks, int number_of_records) {
	if (number_of_records < 0) {
		throw  std::invalid_argument("number_of_records must be positive");
	}
	if (size = EMPTY) {
		delete[] groups;
		delete[] records;
	}
	records = new Record[number_of_records];
	try { 
		groups = new  Group[number_of_records];
	}
	catch (std::bad_alloc&) {
		delete[] records;
		throw;
	}
	for (int i = 0; i < number_of_records; i++) {
		records[i].inStock = records_stocks[i];
		groups[i].culomn = i;
	}
	size = number_of_records;
}

UnionFind::UnionFind(int* records_stocks, int number_of_records) {
	newMonth(records_stocks, number_of_records);

}


int UnionFind::findGroup(int r_id,  int* hight) {
	int i = r_id;
	int exstra = 0;
	int substruct_exstra = 0;
	while ( i != EMPTY && records[i].parent != EMPTY) {
		exstra += records[i].extra;
		i = records[i].parent;
	}
	int head = i;
	*hight = exstra;
	i = r_id;
	while (i != EMPTY && records[i].parent != EMPTY) {
		records[i].parent = head;
		substruct_exstra = records[i].extra;
		records[i].extra = exstra ;
		exstra -= substruct_exstra;
		i = records[i].parent;
	}
	return head;
}

StatusType UnionFind::putOnTop(int r_id1, int r_id2) {

	if (r_id1 < 0 || r_id2 < 0 ) {
		return INVALID_INPUT;
	}

	if (r_id1 >= size || r_id2 >= size ) {
		return DOESNT_EXISTS;
	}

	int  height1 = 0,  height2 = 0;
	int group_id1 = findGroup(r_id1, &height1), group_id2 = findGroup(r_id2, &height2);
	Group group1 = groups[group_id1];
	Group group2 = groups[group_id2];

	if (group_id1 == group_id2) {
		return FAILURE;
	}

	if ( group1.size < group2.size ) {
		records[r_id1].parent = r_id2;
		records[r_id1].extra += height2 - records[r_id2].extra;
		group2.size += group1.size;
		group1.size = 0;
		if (group1.culomn > group2.culomn)
			group2.culomn = group1.culomn;
	}

	if ( group1.size > group2.size ) {
		records[r_id2].parent = r_id1;
		records[r_id1].extra += height2;
		records[r_id2].extra -= records[r_id1].extra;
		group1.size += group2.size;
		group2.size = 0;
		if (group2.culomn > group1.culomn)
			group1.culomn = group2.culomn;
	}

	return SUCCESS;
	
}

StatusType UnionFind::getPlace(int r_id, int* column, int* hight) {
	if (r_id < 0) {
		return INVALID_INPUT;
	}

	if (r_id >= size ) {
		return DOESNT_EXISTS;
	}

	Group group = groups[findGroup(r_id, hight)];
	*column = group.culomn;
	return SUCCESS;
}