#include "IncludeFile.h"
#include "UtilTool.h"
#include "CodeGen.h"
#include "Common.h"

using namespace llvm;


ValueInfo::ValueInfo(int num, Value* value){
    this->num = num;
    this->value = value;
}

int ValueInfo::getNum(){
    return this->num;
}

Value* ValueInfo::getValue(){
    return this->value;
}

BlockInfo::BlockInfo(BasicBlock* block, std::vector<ValueInfo*> values, int childblocknum){
    this->block = block;
    this->values = std::vector<ValueInfo*>(values);
    this->childblocknum = childblocknum;
    this->flag = false;
};
BlockInfo::BlockInfo(BasicBlock* block, ValueInfo* value, int childblocknum){
    this->block = block;
    this->values.push_back(value);
    this->childblocknum = childblocknum;
    this->flag = false;
}
BlockInfo::BlockInfo(BasicBlock* block, int childblocknum){
    this->block = block;
    this->childblocknum = childblocknum;
    this->flag = false;
}

void BlockInfo::insertValue(ValueInfo* value){
    this->values.push_back(value);
};
std::vector<ValueInfo*> BlockInfo::getValues(){
    return this->values;
}
BasicBlock* BlockInfo::getBasicBlock(){
    return this->block;
}
int BlockInfo::getValuesSize(){
    return this->values.size();
}
ValueInfo* BlockInfo::getValue(){
    if(this->getValuesSize()<= 0){
        int randnum = UtilTool::getrandnum(UtilTool::MAX_NUM);
        IntegerCodeGen icg(randnum);
        ValueInfo* newvalue = icg.codegen(this->block);
        this->values.push_back(newvalue);
        return newvalue;
    }
    int randvalue_index = UtilTool::getrandnum(this->getValuesSize());
    return values[randvalue_index];
}
ValueInfo* BlockInfo::getLastValue(){
    if(this->getValuesSize() <= 0){
        int randnum = UtilTool::getrandnum(UtilTool::MAX_NUM);
        IntegerCodeGen icg(randnum);
        ValueInfo* newvalue = icg.codegen(this->block);
        this->values.push_back(newvalue);
        return newvalue;
    }
    int last_index = this->getValuesSize() - 1;
    return this->values[last_index];
}
