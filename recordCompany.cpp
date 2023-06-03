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
	return SUCCESS;
};
StatusType RecordsCompany::makeMember(int c_id){
	return SUCCESS;
};
Output_t<bool> RecordsCompany::isMember(int c_id){
	return SUCCESS;
};
StatusType RecordsCompany::buyRecord(int c_id, int r_id){
	return SUCCESS;
};
StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double  amount){
	return SUCCESS;
};
Output_t<double> RecordsCompany::getExpenses(int c_id) {
	return SUCCESS;
};
StatusType RecordsCompany::putOnTop(int r_id1, int r_id2){
	return records.putOnTop(r_id1, r_id2);
};
StatusType RecordsCompany::getPlace(int r_id, int* column, int* hight){
	return records.getPlace( r_id, column, hight);
};


