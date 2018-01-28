#include "IncludeFile.h"

using namespace llvm;

#ifndef COMMON
#define COMMON

class BlockPath{
public:
    static void InsertBlock(BasicBlock* block){
        blocks.push_back(block);
    }
    static std::vector<BasicBlock*> getBlocks(){
        return blocks;
    }
private:
    static std::vector<BasicBlock*> blocks;    
};


class ValuesUse{
public:
    static void InsertValue(Value* value){
        values.push_back(value);
    }
    static std::vector<Value*> getValues(){
        return values;
    }
private:
    static std::vector<Value*> values;
};


class BlockInfo{
public:
    BlockInfo(BasicBlock* block, std::vector<Value*> values){
        this->block = block;
        this->values = std::vector<Value*>(values);
    };
    BlockInfo(BasicBlock* block, Value* value){
        this->block = block;
        this->values.push_back(value);
    }
    BlockInfo(BasicBlock* block){
        this->block = block;
    }

    void insertValue(Value* value){
        this->values.push_back(value);
    };
    std::vector<Value*> getValues(){
        return this->values;
    }
    BasicBlock* getBasicBlock(){
        return this->block;
    }
    int getValuesSize(){
        return this->values.size();
    }
    Value* getValue(){
        if(this->getValuesSize()<= 0){
            int randnum = UtilTool::getrandnum(UtilTool::MAX_NUM);
            IntegerCodeGen icg(randnum);
            Value* newvalue = icg.codegen(this->block);
            this->values.push_back(newvalue);
            return newvalue;
        }
        int randvalue_index = UtilTool::getrandnum(this->getValuesSize());
        return values[randvalue_index];
    }

private:
    BasicBlock* block;
    std::vector<Value*> values;
};

#endif