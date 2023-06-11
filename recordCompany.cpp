#include "recordsCompany.h"
#include <new>


StatusType RecordsCompany::newMonth(int* records_stocks, int number_of_records) {
    try {
        records.newMonth(records_stocks ,number_of_records);
		club_members.clear_rank_info();
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
    return customers.add_customer(c_id, phone);
};


Output_t<int> RecordsCompany::getPhone(int c_id){
    return customers.get_phone(c_id);
};

StatusType RecordsCompany::makeMember(int c_id){
    StatusType result = customers.make_member(c_id);
    if(result != SUCCESS) {
        return result;
    }
    try {
        club_members.insert(c_id, c_id);
    } catch (std::bad_alloc &) {
        customers.unmember_latest();
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
};


Output_t<bool> RecordsCompany::isMember(int c_id){
    return customers.is_member(c_id);
};

StatusType RecordsCompany::buyRecord(int c_id, int r_id){

	int t;
	Output_t<bool> result_is_member = customers.is_member(c_id);
	try {
		
		t = records[r_id] + 100;
	}
	catch ( std::invalid_argument&) {
		return INVALID_INPUT;
	}
	catch (std::out_of_range&) {
		return result_is_member.status();
	}
	if(result_is_member..status() == SUCCESS)
		records[r_id]++;

	if (result_is_member.ans() == true) {
		club_members.add_rank(c_id + 1, t);
		club_members.add_rank(c_id, -t);
	}
	//TO COMPLETE
	return SUCCESS;

};

StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double  amount){
	if (amount <= 0 || c_id1 < 0 || c_id1 > c_id2) {
		return INVALID_INPUT;
	}
	club_members.add_rank(c_id2, -amount);
	club_members.add_rank(c_id1, amount);
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

    //TO COMPLETE
    return SUCCESS;
};



StatusType RecordsCompany::putOnTop(int r_id1, int r_id2){
    return records.putOnTop(r_id1, r_id2);
};

StatusType RecordsCompany::getPlace(int r_id, int* column, int* hight){
    return records.getPlace( r_id, column, hight);
};