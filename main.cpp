#include <iostream>
#include <time.h>
#include <stdio.h>
#include <string>
using namespace std;

void begin(int maxdeep);

//basic rule:
//int maxdeep = 4;
int t = 0;
int getrandnum(int num){
    if(num <= 0){
        return 0;
    }
    srand((unsigned)time(NULL)+t);
    t++;
    return rand()%num;
}

//------------------val------------------------------
#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3
#define NUM 4
#define END 5
int valrandsize = 5;
int valindex = 0;
string valname = "dxval";
string valop(int maxdeep){
    maxdeep--;
    
    int randchoice = getrandnum(valrandsize);
    string val1;
    string val2;
    string retval;
    int initnum = 0;
    if(maxdeep <= 0){
        randchoice = NUM;
    }
    switch(randchoice){
        case ADD:
            val1 = valop(maxdeep);
            val2 = valop(maxdeep);
            retval = valname + to_string(valindex);
            valindex++;
            cout << retval << "=" << val1 << "+" << val2 << endl;
            return retval;
            break;
        case SUB:
            val1 = valop(maxdeep);
            val2 = valop(maxdeep);
            retval = valname + to_string(valindex);
            valindex++;
            cout << retval << "=" << val1 << "-" << val2 << endl;
            return retval;
            break;
        case MUL:
            val1 = valop(maxdeep);
            val2 = valop(maxdeep);
            retval = valname + to_string(valindex);
            valindex++;
            cout << retval << "=" << val1 << "*" << val2 << endl;
            return retval;
            break;
        case DIV:
            val1 = valop(maxdeep);
            val2 = valop(maxdeep);
            retval = valname + to_string(valindex);
            valindex++;
            cout << retval << "=" << val1 << "/" << val2 << endl;
            return retval;
            break;
        case NUM:
            initnum = getrandnum(100);
            retval = valname + to_string(valindex);
            valindex++;
            cout << retval << "=" << initnum << endl;
            return retval;
            break;
        case END:
            return "";
            break;
    }
    return "";
}


//-------------------------------bool------------------------------
#define ICMP 0
#define CMP 1
string boolname = "cmp";
int boolrandsize = 2;
int boolindex = 0;
string boolop(int maxdeep){ 
    int randchoice = getrandnum(boolrandsize);
    int initnum = 0;
    int randmaxdeep = getrandnum(maxdeep);
    string val1;
    string val2;
    string retval;
    switch(randchoice){
        case ICMP:
            val1 = valop(randmaxdeep);
            val2 = valop(randmaxdeep);
            retval = boolname + to_string(boolindex);
            boolindex++;
            cout<< retval << " icmp " << val1 << " " <<val2 <<endl;
            return retval;
            break;
        case CMP:
            val1 = valop(randmaxdeep);
            val2 = valop(randmaxdeep);
            retval = boolname + to_string(boolindex);
            boolindex++;
            cout<< retval << " cmp " << val1 << " " <<val2 <<endl;
            return retval;
            break;
    } 
    return "";
}

//--------------------newblock------------------------
string blockname = "basicblock";
int blockindex = 0;
string basicblockop(int maxdeep){
    //----------new basicblock--------
    string newblock = blockname + to_string(blockindex);
    blockindex++;

    //----------content---------------
    
    int randmaxdeep = getrandnum(maxdeep);
    cout<<newblock<<":"<<endl;
    begin(randmaxdeep);
    cout<<"-----end"<<endl;
    //valop(randmaxdeep);

    //----------end block--------------------
    //br jump to the end.
    return newblock;
}

//--------------------br-----------------------------
string brop(int maxdeep){
    int randmaxdeep = getrandnum(maxdeep);
    string boolval = boolop(randmaxdeep);
    randmaxdeep = getrandnum(maxdeep);
    string newblock1 = basicblockop(randmaxdeep);
    randmaxdeep = getrandnum(maxdeep);
    string newblock2 = basicblockop(randmaxdeep);

    cout << "br " << boolval << " " << newblock1 << "," << newblock2 <<endl;
    
    return "";
}


//-----------------------begin------------------------
#define VAL 0
#define BOOL 1
#define BR 2
#define BASICBLOCK 3
#define CALL 4 //to be complete
int choicesize = 4;
void begin(int maxdeep){
    int randchoice = getrandnum(choicesize);
    switch(randchoice){
        case VAL:
            valop(maxdeep);
            break;
        case BOOL:
            boolop(maxdeep);
            break;
        case BR:
            brop(maxdeep);
            break;
        case BASICBLOCK:
            basicblockop(maxdeep);
            break;
        case CALL:
            break;
    }


}



int main(int argc, char* argv[]){
    int step = atoi(argv[1]);
    begin(step);
    cout<<"generated!"<<endl;
    return 0;
}