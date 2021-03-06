/*
#include "IncludeFile.h"
#include "CodeGen.h"
#include "UtilTool.h"
#include "Common.h"
#include "log.h"

#ifndef SEMANTICGEN0
#define SEMANTICGEN0

class SemanticGen0{
public:
SemanticGen0(int OperatorChoices = 4, int ExpressionChoices = 2, int CmpChoices = 4, int Maxswitch = 10){
    this->OperatorChoices = OperatorChoices;
    this->ExpressionChoices = ExpressionChoices;
    this->CmpChoices = CmpChoices;
    this->Maxswitch = Maxswitch;

}

BlockInfo expressionGen(BlockInfo blockinfo){
    BasicCodeGen* bcg = NULL;
    int randchoice = UtilTool::getrandnum(this->ExpressionChoices);
    switch(randchoice){
        case 0: LOGDLN("value define"); bcg = this->valdefine(); break;
        case 1: 
            LOGDLN("value arithcode gen");
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
    LOGDLN("==============================================");
    LOGDLN(this->codenum);
    if(this->codenum <= 0){
        return blockinfo;
    };
    this->codenum--;
    int randchoice = UtilTool::getrandnum(5);
    switch(randchoice){
        case 0:
        case 1:
        case 2:
            return this->expressionGen(blockinfo);
        case 3:
            return this->ifGen(blockinfo);
        case 4:
            return this->switchGen(blockinfo);
    }
}

BlockInfo ifGen(BlockInfo blockinfo){
    LOGDLN("IF GEN");
    BasicBlock* mergeblock = CodeGenTool::CreateBlock(blockinfo.getBasicBlock());

////////////////////////////////////////////////////////////////////////////////

    ValueInfo* randval1 = ValuesUse::getValue();
    ValueInfo* randval2 = ValuesUse::getValue();
/////////////////////////////////////////////////////////////////////////////////


    BoolCmpCodeGen* bccg = Op2(randval1, randval2);

    Value* boolvalue = bccg->codegen(blockinfo.getBasicBlock());


    bool boolnum = bccg->getRet(); 


    IfThenCodeGen itcg(boolvalue);
    
    itcg.codegen(blockinfo.getBasicBlock());
    
    BasicBlock* trueblock = itcg.getTrueBlock();
    

    BasicBlock* falseblock = itcg.getFalseBlock();
    
    if(boolnum == true){
        BlockPath::InsertBlock(trueblock);
    }else{
        BlockPath::InsertBlock(falseblock);
    }

    BlockInfo trueblockinfo = this->expressionGen(BlockInfo(trueblock, blockinfo.getValues()));


    BlockInfo falseblockinfo = this->expressionGen(BlockInfo(falseblock, blockinfo.getValues()));



    ValueInfo* phivalue = CodeGenTool::MergeBlocks(trueblockinfo.getBasicBlock(),trueblockinfo.getLastValue(), 
                                               falseblockinfo.getBasicBlock(), falseblockinfo.getLastValue(), 
                                               mergeblock);

    BlockInfo mergeblockinfo(mergeblock, blockinfo.getValues());
    
    mergeblockinfo.insertValue(phivalue);

    BlockPath::InsertBlock(mergeblock);

    return this->StatementGen(mergeblockinfo);

}   

BlockInfo switchGen(BlockInfo blockinfo){
    LOGDLN("Switchc gen");
    int condnum = UtilTool::getrandnum(this->Maxswitch);
    BasicBlock* mergeblock = CodeGenTool::CreateBlock(blockinfo.getBasicBlock());
    
/////////////////////////////////////////////////////////////////////////////////////////
    ValueInfo* vuvalueinfo = ValuesUse::getValue();
    SwitchCodeGen scg(vuvalueinfo, condnum);
/////////////////////////////////////////////////////////////////////////////////////////
    scg.codegen(blockinfo.getBasicBlock());
    std::vector<BasicBlock*> blocks;
    std::vector<ValueInfo*> values;

    BasicBlock* truecaseblock = scg.getTruecaseBlock();

    BlockPath::InsertBlock(truecaseblock);

    LOGDLN("TRUE CASSE BLOCK");
    BlockInfo truecaseblockinfo = this->expressionGen(BlockInfo(truecaseblock, blockinfo.getValues()));
    LOGDLN("AFTER TRUE CASE BLOCK");
    blocks.push_back(truecaseblockinfo.getBasicBlock());
    values.push_back(truecaseblockinfo.getLastValue());

    std::vector<BasicBlock*> otherblocks = scg.getOthercaseBlocks();

    for(int i=0;i<otherblocks.size();i++){
        BlockInfo othercaseblocksinfo = this->expressionGen(BlockInfo(otherblocks[i],blockinfo.getValues()));
        blocks.push_back(othercaseblocksinfo.getBasicBlock());
        values.push_back(othercaseblocksinfo.getLastValue());
    }

    
    ValueInfo* phivalue = CodeGenTool::MergeBlocks(blocks, values, mergeblock);
    BlockInfo mergeblockinfo(mergeblock, blockinfo.getValues());
    mergeblockinfo.insertValue(phivalue);

    BlockPath::InsertBlock(mergeblock);
    
    return this->StatementGen(mergeblockinfo); 
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
*/