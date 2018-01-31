#include "IncludeFile.h"
#include "UtilTool.h"
#include "CodeGen.h"
#include "Common.h"

using namespace llvm;

/*
static void BlockPath::InsertBlock(BasicBlock* block){
    blocks.push_back(block);
}
    
static std::vector<BasicBlock*> BlockPath::getBlocks(){
    return blocks;
}



static void ValuesUse::InsertValue(Value* value){
    values.push_back(value);
}

static std::vector<Value*> ValuesUse::getValues(){
    return values;
}
*/


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


BlockInfo::BlockInfo(BasicBlock* block, std::vector<ValueInfo*> values){
    this->block = block;
    this->values = std::vector<ValueInfo*>(values);
};
BlockInfo::BlockInfo(BasicBlock* block, ValueInfo* value){
    this->block = block;
    this->values.push_back(value);
}
BlockInfo::BlockInfo(BasicBlock* block){
    this->block = block;
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
