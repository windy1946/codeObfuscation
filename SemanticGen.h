
#include "IncludeFile.h"
#include "CodeGen.h"
#include "UtilTool.h"
#include "Common.h"

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


