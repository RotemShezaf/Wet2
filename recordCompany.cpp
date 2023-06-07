#include "recordsCompany.h"
#include <new>




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
        //TODO insert into members
    } catch (std::bad_alloc &) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
};
Output_t<bool> RecordsCompany::isMember(int c_id){
    return customers.is_member(c_id);
};
StatusType RecordsCompany::buyRecord(int c_id, int r_id){
    int t;
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
    //TO DO
    return SUCCESS;
};
Output_t<double> RecordsCompany::getExpenses(int c_id) {
    //TO DO
    return SUCCESS;
};
StatusType RecordsCompany::putOnTop(int r_id1, int r_id2){
    return records.putOnTop(r_id1, r_id2);
};

StatusType RecordsCompany::getPlace(int r_id, int* column, int* hight){
    return records.getPlace( r_id, column, hight);
};


