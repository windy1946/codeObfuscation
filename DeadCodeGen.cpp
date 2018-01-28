#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"

#include "DeadCodeGen.h"

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <string>


using namespace llvm;
using namespace std;

int valindex = 0;

int DeadCodeGen::getrandnum(int num){
    if(num <= 0){
        return 0;
    }
    srand((unsigned)time(NULL)+this->t);
    t++;
    return (int)(rand()%num);
}


int maxchoice = 2;
Value* DeadCodeGen::choiceOp(BasicBlock* block, int maxdeep){
    maxdeep--;
    if(this->deadcodenum <= 0 || maxdeep <= 0){
        int initnum = getrandnum(100);
        valindex++;
        this->deadcodenum--;
        return this->IntegerCodeGen(block, initnum);
        errs()<<"after integer code gen\n";
    }
    int randchoice = getrandnum(maxchoice);
    switch(randchoice){
        case VALOP:
            return this->valop(block, maxdeep);
        case IFTHENOP:
            return this->ifthenop(block, maxdeep);
        case BOOLOP:
            return this->boolop(block, maxdeep);
        
    }
    return nullptr;
}

//------------------val------------------------------

int valrandsize = 4;
Value* DeadCodeGen::valop(BasicBlock* block, int maxdeep){
    errs() << "valop---------------\n";
    if(this->deadcodenum <= 0 || maxdeep <= 0){
        errs()<<"---------------<=0\n";
        int initnum = getrandnum(100);
        valindex++;
        this->deadcodenum--;
        return this->IntegerCodeGen(block, initnum);
    }
    maxdeep--;
 
    int randchoice = getrandnum(valrandsize);
    Value* val1;
    Value* val2;
    Value* retval;


    
    switch(randchoice){
        case ADD:
        case SUB:
        case MUL:
        case DIV:
            this->deadcodenum--;
            val1 = valop(block, maxdeep);
            this->deadcodenum--;
            val2 = valop(block, maxdeep);
            valindex++;
            valname + to_string(valindex);
            this->deadcodenum--;
            errs()<<"---------------------valop\n";
            retval = this->OperationCodeGen(block, (ARICHOP)randchoice, val1, val2);
            return retval;
        case NUM:
            errs()<<"num : \n";
            int initnum = getrandnum(100);
            valindex++;
            this->deadcodenum--;
            retval = this->IntegerCodeGen(block, initnum);
            return retval;
        
    }
    
    return retval;
}


//-------------------------------bool------------------------------

string boolname = "cmp";
int boolrandsize = 4;
int boolindex = 0;

Value* DeadCodeGen::boolop(BasicBlock* block, int maxdeep){ 
    if(this->deadcodenum <= 0 || maxdeep <= 0){
        
        int initnum = getrandnum(100);
        valindex++;
        this->deadcodenum--;
        Value* val1 = this->IntegerCodeGen(block, initnum);
        initnum = getrandnum(100);
        Value* val2 = this->IntegerCodeGen(block, initnum);
        int randchoice = getrandnum(boolrandsize);
        return this->CmpCodeGen(block, randchoice, val1, val2);
    }
    maxdeep--;
    
    int randchoice = getrandnum(boolrandsize);
    int randmaxdeep = getrandnum(maxdeep);
    Value* val1;
    Value* val2;
    Value* retval;
    this->deadcodenum--;
    val1 = valop(block, randmaxdeep);
    this->deadcodenum--;
    val2 = valop(block, randmaxdeep);
    boolname + to_string(boolindex);
    boolindex++;
    this->deadcodenum--;
    retval = this->CmpCodeGen(block, randchoice, val1, val2);
    return retval;
}


Value* DeadCodeGen::ifthenop(BasicBlock* block, int maxdeep){
    if(this->deadcodenum <= 0 || maxdeep <= 0){
        int initnum = getrandnum(100);
        valindex++;
        this->deadcodenum--;
        return this->IntegerCodeGen(block, initnum);
    }
    maxdeep--;
    
    this->deadcodenum--;
    Value* cmpval = this->CmpCodeGen(block,ADD, this->IntegerCodeGen(block, 4), this->IntegerCodeGen(block, 9));//boolop(block, maxdeep);

    BasicBlock* ThenBB = nullptr;
    BasicBlock* ElseBB = nullptr;
    BasicBlock* MergeBB = nullptr;
    this->deadcodenum--;
    this->IfThenCodeGen(block, cmpval, ThenBB, ElseBB, MergeBB);
    this->deadcodenum--;
    Value* thenval = this->IntegerCodeGen(ThenBB, 18);//this->choiceOp(ThenBB, this->deadcodedeep);
    this->deadcodenum--;
    Value* elseval = this->IntegerCodeGen(ElseBB, 19);//this->choiceOp(ElseBB, this->deadcodedeep);
    this->deadcodenum--;
    Value* mergeval = this->IntegerCodeGen(MergeBB, 33);//this->PHICodeGen(MergeBB, ThenBB, thenval, ElseBB, elseval);
    errs() << "----------------finish ifthenop\n";
    return mergeval;
}
