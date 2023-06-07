#include "recordsCompany.h"




StatusType RecordsCompany::newMonth(int* records_stocks, int number_of_records) {
	try {
		records.newMonth(records_stocks ,number_of_records);
	}
	catch (std::invalid_argument&) {
		return StatusType::INVALID_INPUT;
	}
	catch (std::bad_alloc&) {
		return StatusType::ALLOCATION_ERROR;
	}
	return StatusType::SUCCESS;

};
	
StatusType RecordsCompany::addCostumer(int c_id, int phone){
	return SUCCESS;
};
Output_t<int> RecordsCompany::getPhone(int c_id){
	//TO DO
	return SUCCESS;
};
StatusType RecordsCompany::makeMember(int c_id){
	club_members.insert(c_id, c_id);
	//TO DO
	return SUCCESS;
};
Output_t<bool> RecordsCompany::isMember(int c_id){
	//TO DO
	return SUCCESS;
};
StatusType RecordsCompany::buyRecord(int c_id, int r_id){
	int t =0;
	t++;
	try {
		t = records[r_id];
		records[r_id]++;
	}
	catch ( std::invalid_argument&) {
		return INVALID_INPUT;
	}
	catch (std::out_of_range&) {
		return DOESNT_EXISTS;
	}
	//TO COMPLETE
	return SUCCESS;
};
StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double  amount){
	if (amount <= 0 || c_id1 < 0 || c_id1 > c_id2) {
		return INVALID_INPUT;
	}
	club_members.add_rank(c_id2, amount);
	club_members.add_rank(c_id1, -amount);
	return SUCCESS;
};
Output_t<double> RecordsCompany::getExpenses(int c_id) {
	if ( c_id < 0 ) {
		return INVALID_INPUT;
	}
	int result;
	try {
		result = club_members.get_rank(c_id);
	}
	catch (KeyNotFound&) {
		return StatusType::FAILURE;
	}
	
	return result;
};
StatusType RecordsCompany::putOnTop(int r_id1, int r_id2){
	return records.putOnTop(r_id1, r_id2);
};

StatusType RecordsCompany::getPlace(int r_id, int* column, int* hight){
	return records.getPlace( r_id, column, hight);
};


