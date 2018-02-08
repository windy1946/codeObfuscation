#include "IncludeFile.h"
#include "UtilTool.h"
#ifndef COMMON
#define COMMON

using namespace llvm;


class ValueInfo{
public:
    ValueInfo(int num, Value* value);

    int getNum();

    Value* getValue();

private:
    int num;
    Value* value;

};


class BlockInfo{
public:
    BlockInfo(BasicBlock* block, std::vector<ValueInfo*> values, int childblocknum = 10);
    BlockInfo(BasicBlock* block, ValueInfo* value, int childblocknum = 10);
    BlockInfo(BasicBlock* block, int childblocknum = 10);

    void insertValue(ValueInfo* value);
    std::vector<ValueInfo*> getValues();
    BasicBlock* getBasicBlock();
    int getValuesSize();
    ValueInfo* getValue();
    ValueInfo* getLastValue();
    void setChildblockNum(int num){
        this->childblocknum = num;
    }
    int getChildblockNum(){
        return this->childblocknum;
    }
private:
    BasicBlock* block;
    std::vector<ValueInfo*> values;
    int childblocknum;
};


//static std::vector<BasicBlock*> blocks;

static std::vector<BasicBlock*> blocks;   

class BlockPath{
public:
    static void InsertBlock(BasicBlock* block){
        blocks.push_back(block);
    };
    static std::vector<BasicBlock*> getBlocks(){
        return blocks;
    };
private:
     
};


static std::vector<ValueInfo*> values;

class ValuesUse{
public:
    static void setValues(std::vector<ValueInfo*> tvalues){
        for(int i=0;i<tvalues.size();i++){
            values.push_back(tvalues[i]);
        }
    }

    static std::vector<ValueInfo*> getValues(){
        return values;
    };

    static ValueInfo* getValue(int i){
        return values[i];
    }

    static ValueInfo* getValue(){
        int index = UtilTool::getrandnum(values.size());
        return values[index];
    }

};



/*
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


class ValueInfo{
public:
    ValueInfo(int num, Value* value){
        this->num = num;
        this->value = value;
    }

    int getNum(){
        return this->num;
    }

    Value* getValue(){
        return this->value;
    }

private:
    int num;
    Value* value;

};

class BlockInfo{
public:
    BlockInfo(BasicBlock* block, std::vector<ValueInfo*> values){
        this->block = block;
        this->values = std::vector<ValueInfo*>(values);
    };
    BlockInfo(BasicBlock* block, ValueInfo* value){
        this->block = block;
        this->values.push_back(value);
    }
    BlockInfo(BasicBlock* block){
        this->block = block;
    }

    void insertValue(ValueInfo* value){
        this->values.push_back(value);
    };
    std::vector<ValueInfo*> getValues(){
        return this->values;
    }
    BasicBlock* getBasicBlock(){
        return this->block;
    }
    int getValuesSize(){
        return this->values.size();
    }
    ValueInfo* getValue(){
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
    ValueInfo* getLastValue(){
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
private:
    BasicBlock* block;
    std::vector<ValueInfo*> values;
};
*/
#endif