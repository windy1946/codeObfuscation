
#include "IncludeFile.h"
#include "CodeGen.h"
#include "UtilTool.h"
#include "Common.h"
#include "log.h"

#ifndef SEMANTICGEN
#define SEMANTICGEN

class SemanticGen{
public:
SemanticGen(int OperatorChoices = 4, int ExpressionChoices = 2, int CmpChoices = 4, int Maxswitch = 10){
    this->OperatorChoices = OperatorChoices;
    this->ExpressionChoices = ExpressionChoices;
    this->CmpChoices = CmpChoices;
    this->Maxswitch = Maxswitch;
    
}

BlockInfo expressionGen(BlockInfo blockinfo){
    BasicCodeGen* bcg = NULL;
    int randchoice = UtilTool::getrandnum(this->ExpressionChoices);
    switch(randchoice){
        case 0: LOGD("value define"); bcg = this->valdefine(); break;
        case 1: 
            LOGD("value arithcode gen");
            ValueInfo* randvalue1 = blockinfo.getValue();
            ValueInfo* randvalue2 = blockinfo.getValue();
            bcg = this->Op1(randvalue1, randvalue2);
            break;
    }
    ValueInfo* newvalue = bcg->codegen(blockinfo.getBasicBlock());
    if(newvalue == nullptr){
        LOGE("newvalue is a null");
        return blockinfo;
    }
    blockinfo.insertValue(newvalue);

    return this->StatementGen(blockinfo);
}


BlockInfo StatementGen(BlockInfo blockinfo){
    LOGD("==============================================");
    LOGD(this->codenum);
    if(this->codenum <= 0){
        return blockinfo;
    };
    this->codenum--;
    int randchoice = UtilTool::getrandnum(2);
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
    LOGD("IF GEN");
    BasicBlock* mergeblock = CodeGenTool::CreateBlock(blockinfo.getBasicBlock());

    ValueInfo* randval1 = blockinfo.getValue();
    ValueInfo* randval2 = blockinfo.getValue();

    BoolCmpCodeGen* bccg = Op2(randval1, randval2);

    Value* boolvalue = bccg->codegen(blockinfo.getBasicBlock());
    bool boolnum = bccg->getRet(); 
    IfThenCodeGen itcg(boolvalue);
    
    itcg.codegen(blockinfo.getBasicBlock());
    
    BasicBlock* trueblock = itcg.getTrueBlock();
    

    BasicBlock* falseblock = itcg.getFalseBlock();
    

    LOGD("BEFORE IF THEN TRUE BLOCK");
    BlockInfo trueblockinfo = this->expressionGen(BlockInfo(trueblock, blockinfo.getValues()));
    LOGD("AFTER IF THEN TRUE BLOCK");
    BlockInfo falseblockinfo = this->expressionGen(BlockInfo(falseblock, blockinfo.getValues()));
    LOGD("AFTEER IF THEN FALSE BLOCK");
    

    ValueInfo* phivalue = CodeGenTool::MergeBlocks(trueblockinfo.getBasicBlock(),trueblockinfo.getLastValue(), 
                                               falseblockinfo.getBasicBlock(), falseblockinfo.getLastValue(), 
                                               mergeblock);

    return this->StatementGen(BlockInfo(mergeblock, phivalue));

}   

BlockInfo switchGen(BlockInfo blockinfo){
    int condnum = UtilTool::getrandnum(this->Maxswitch);
    BasicBlock* mergeblock = CodeGenTool::CreateBlock(blockinfo.getBasicBlock());

    SwitchCodeGen scg(blockinfo.getValue(), condnum);
    scg.codegen(blockinfo.getBasicBlock());
    std::vector<BasicBlock*> blocks;
    std::vector<ValueInfo*> values;

    BasicBlock* truecaseblock = scg.getTruecaseBlock();
    LOGD("TRUE CASSE BLOCK");
    BlockInfo truecaseblockinfo = this->StatementGen(BlockInfo(truecaseblock, blockinfo.getValues()));
    LOGD("AFTER TRUE CASE BLOCK");
    blocks.push_back(truecaseblockinfo.getBasicBlock());
    values.push_back(truecaseblockinfo.getValue());

    std::vector<BasicBlock*> otherblocks = scg.getOthercaseBlocks();

    for(int i=0;i<otherblocks.size();i++){
        BlockInfo othercaseblocksinfo = this->StatementGen(BlockInfo(otherblocks[i],blockinfo.getValues()));
        blocks.push_back(othercaseblocksinfo.getBasicBlock());
        values.push_back(othercaseblocksinfo.getValue());
    }

    
    ValueInfo* phivalue = CodeGenTool::MergeBlocks(blocks, values, mergeblock);

    return this->StatementGen(BlockInfo(mergeblock, phivalue)); 
}

private:
    int codenum = 20;
    int ExpressionChoices;
    int OperatorChoices;
    int Maxswitch;
    int CmpChoices;
    BasicCodeGen* Op1(ValueInfo* val1, ValueInfo* val2){
        int operatorchoice = UtilTool::getrandnum(this->OperatorChoices);
        return new IntegerArithCodeGen(val1, val2, (ARICHOP)operatorchoice); 
    }

    BoolCmpCodeGen* Op2(ValueInfo* val1, ValueInfo* val2){
        int cmpchoice = UtilTool::getrandnum(this->CmpChoices);
        return new BoolCmpCodeGen(val1, val2, (CMPOP)cmpchoice);
    }

    BasicCodeGen* valdefine(){
        int randnum = UtilTool::getrandnum(UtilTool::MAX_NUM);
        return new IntegerCodeGen(randnum);
    }

};


#endif