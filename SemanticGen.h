#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/ADT/APInt.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <string>

#include "CodeGen.h"
#include "UtilTool.h"

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


class SemanticGen{
public:
SemanticGen(int ExpressionChoices = 2, int CmpChoices = 4, int Maxswitch = 10){
    this->ExpressionChoices = ExpressionChoices;
    this->CmpChoices = CmpChoices;
    this->Maxswitch = Maxswitch;
}

BlockInfo expressionGen(BlockInfo blockinfo){
    BasicCodeGen* bcg = NULL;
    int randchoice = UtilTool::getrandnum(this->ExpressionChoices);
    switch(randchoice){
        case 0: bcg = this->valdefine(); break;
        case 1: 
            Value* randvalue1 = blockinfo.getValue();
            Value* randvalue2 = blockinfo.getValue();
            bcg = this->Op1(randvalue1, randvalue2);
    }
    Value* newvalue = bcg->codegen(blockinfo.getBasicBlock());
    blockinfo.insertValue(newvalue);

    return this->StatementGen(blockinfo);
}


BlockInfo StatementGen(BlockInfo blockinfo){
    int randchoice = UtilTool::getrandnum(3);
    switch(randchoice){
        case 0:
            return this->expressionGen(blockinfo);
        case 1:
            return this->ifGen(blockinfo);
        case 2:
            return this->switchGen(blockinfo);
    }
}

BlockInfo ifGen(BlockInfo blockinfo){
    BasicCodeGen* bcg = NULL;

    Value* randval1 = blockinfo.getValue();
    Value* randval2 = blockinfo.getValue();

    BoolCmpCodeGen* bccg = Op2(randval1, randval2);

    Value* boolvalue = bccg->codegen(blockinfo.getBasicBlock());

    IfThenCodeGen itcg(boolvalue);
    itcg.codegen(blockinfo.getBasicBlock());
    
    BasicBlock* trueblock = itcg.getTrueBlock();
    BasicBlock* falseblock = itcg.getFalseBlock();
    
    BlockInfo trueblockinfo = this->StatementGen(BlockInfo(trueblock, blockinfo.getValues()));
    BlockInfo falseblockinfo = this->StatementGen(BlockInfo(falseblock, blockinfo.getValues()));

    BasicBlock* mergeblock = CodeGenTool::CreateBlock(blockinfo.getBasicBlock());

    Value* phivalue = CodeGenTool::MergeBlocks(trueblockinfo.getBasicBlock(),trueblockinfo.getValue, 
                                               falseblockinfo.getBasicBlock, falseblockinfo.getValue(), 
                                               mergeblock);

    return this->StatementGen(BlockInfo(mergeblock, phivalue));

}   

BlockInfo switchGen(BlockInfo blockinfo){
    int condnum = UtilTool::getrandnum(this->Maxswitch);
    SwitchCodeGen scg(blockinfo.getValue(), condnum);
    scg.codegen(blockinfo.getBasicBlock());
    std::vector<BasicBlock*> blocks;
    std::vector<Value*> values;

    BasicBlock* truecaseblock = scg.getTruecaseBlock();

    BlockInfo truecaseblockinfo = this->StatementGen(BlockInfo(truecaseblock));
    blocks.push_back(truecaseblockinfo.getBasicBlock());
    values.push_back(truecaseblockinfo.getValue());

    std::vector<BasicBlock*> otherblocks = scg.getOthercaseBlocks();

    for(int i=0;i<otherblocks.size();i++){
        BlockInfo othercaseblocksinfo = this->StatementGen(BlockInfo(otherblocks[i]));
        blocks.push_back(othercaseblocksinfo.getBasicBlock());
        values.push_back(othercaseblocksinfo.getValue());
    }

    BasicBlock* mergeblock = CodeGenTool::CreateBlock(blockinfo.getBasicBlock());

    Value* phivalue = CodeGenTool::MergeBlocks(blocks, values, mergeblock);

    return this->StatementGen(BlockInfo(mergeblock, phivalue)); 
}

private:
    
    int ExpressionChoices;
    int OperatorChoices;
    int Maxswitch;
    int CmpChoices;
    BasicCodeGen* Op1(Value* val1, Value* val2){
        int operatorchoice = UtilTool::getrandnum(this->OperatorChoices);
        return new IntegerArithCodeGen(val1, val2, (ARICHOP)operatorchoice); 
    }

    BoolCmpCodeGen* Op2(Value* val1, Value* val2){
        int cmpchoice = UtilTool::getrandnum(this->CmpChoices);
        return new BoolCmpCodeGen(val1, val2, (CMPOP)cmpchoice);
    }

    BasicCodeGen* valdefine(){
        int randnum = UtilTool::getrandnum(UtilTool::MAX_NUM);
        return new IntegerCodeGen(randnum);
    }

};


