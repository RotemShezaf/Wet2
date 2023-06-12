#include "unionFind.h"
#include <stdexcept>

UnionFind::~UnionFind() {
	if (size != EMPTY) {
		delete[] records;
		delete[] groups;
	}
}

void UnionFind::newMonth(int* records_stocks, int number_of_records) {
	if (number_of_records < 0) {
		throw  std::invalid_argument("number_of_records must be positive");
	}
	Record* temp_records = new Record[number_of_records];
	Group* temp_groups ;
	try {
		temp_groups = new  Group[number_of_records];
	}
	catch (std::bad_alloc&) {
		delete[] temp_records;
		throw;
	}
	if (size != EMPTY) {
		delete[] groups;
		delete[] records;
	}
	records = temp_records;
	groups = temp_groups;

	for (int i = 0; i < number_of_records; i++) {
		groups[i].height = records_stocks[i];
		groups[i].culomn = i;
	}
	size = number_of_records;
}

UnionFind::UnionFind(int* records_stocks, int number_of_records) {
	newMonth(records_stocks, number_of_records);

}


int UnionFind::findGroup(int r_id,  int* hight) {//find like in union find with 
	int i = r_id;
	int exstra = 0;
	int head;
	int substruct_exstra = 0;

	//find group and hight
	while ( i != EMPTY  && records[i].parent != EMPTY) {
		exstra += records[i].extra;
		
		i = records[i].parent;
	}
	//the group index
	head = i;

	*hight = exstra + records[i].extra;
	i = r_id;

	//records[i] "points to group" - his index in the arrey is the same as the group. put hom as the parent of everyone
	while (i != EMPTY && records[i].parent != EMPTY) {
		int parent = records[i].parent;
		records[i].parent = head;

		//fix exstra
		substruct_exstra = records[i].extra;
		records[i].extra = exstra ;
		exstra -= substruct_exstra;

		i = parent;
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

	if (group_id1 == group_id2) {
		return FAILURE;
	}

	if ( groups[group_id1].size < groups[group_id2].size ) { 
		//union(group_id1, group_id2) like in union find
		records[group_id1].parent = group_id2;  
		groups[group_id2].height += groups[group_id1].height;
		groups[group_id1].size = 0;
		//changing exstra s.t sum over extra in the search group roun gives height
		records[group_id1].extra += groups[group_id2].height - records[group_id2].extra;
		groups[group_id2].size += groups[group_id1].size;


	}

	if ( groups[group_id1].size >= groups[group_id2].size ) {
		// union(group_id1, group_id2) like in union find
		records[group_id2].parent = group_id1; 
		groups[group_id2].size = 0;
		groups[group_id1].culomn = groups[group_id2].culomn;
		groups[group_id1].size += groups[group_id2].size;
		groups[group_id1].height += groups[group_id2].height;

		// changing exstra s.t sum over extra in the search group roun gives height
		records[group_id1].extra += groups[group_id2].height;
		records[group_id2].extra -= records[group_id1].extra;


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

int& UnionFind::operator[](int  r_id) {
	if (r_id >= size) {
		throw std::out_of_range("no such record");
	}
	if (r_id < 0) {
		throw std::invalid_argument("no such record");
	}
	return records[r_id].buys;
}

int UnionFind::buyRecord(int r_id) {
	if (r_id >= size) {
		throw std::out_of_range("no such record");
	}
	if (r_id < 0) {
		throw std::invalid_argument("no such record");
	}
	int temp = records[r_id].buys;
	records[r_id].buys++;
	return temp;
}