#include <iostream>
#include <bits/stdc++.h>
using ll = long long;
#include <string>
#include <map>
#include <sstream>
using namespace std;
fstream file;
struct record{
    int ikey;
    int ival;
};
struct block{
    int RecordSize;
    map<int,int> records;
    record head;
    /* int firstIndexEmpty()
     {
         for (int i = 0; i < this->RecordSize; ++i) {
             if(this->records[i].ikey==0) {
                 //cout<< this->records[i].ikey<<" "<< this->records->ival<<endl;
                 return i;
             }
         }return -1;
     }*/
};
vector<block> blocks;
set<pair<int,int>> sett;
void readFile(char *cIndexFile);
bool CreateRecordFile(char *cIndexFile, int m, int n); // returns true if success and false if
//failure. m is the number of blocks in the file and n is the number of records in a block
int InsertVal(char *cIndexFile, int iVal, int iKey); // returns index of block in which iToken
//and iKey were stored and -1 if failed, where iKey is the key of the record, and iToken = iVal in  the record.
int GetKey(char *cIndexFile, int iBlock, int iRecord); // get value iKey stored in a given block
//iBlock and given record iRecord – returns -1 if record on block is empty
int GetVal(char *cIndexFile, int iBlock, int iRecord); // get value iVal stored in a given block
//iBlock and given record iRecord – returns -1 if record on block is empty
int GetBlockIndex (char *cIndexFile, int iToken); // get index of block containing iKey = iToken
//and -1 if record does not exist
int GetRecordIndex (char *cIndexFile, int iToken); // get index of record containing iKey = iToken
//and -1 if record does not exist
void DeleteKey (char *cIndexFile, int iToken); // delete record containing value iKey
//= iToken
static int Size;
static int blocksSize;
void writeFile(char *cIndexFile,int m, int n);

void merge(){

}


int GetKey(char *cIndexFile, int iBlock, int iRecord){
    readFile(cIndexFile);

    for (int i = 1; i < blocksSize; ++i) {
        if(i==iBlock)
        {
            int counter=0;
            for(auto &iter:blocks[i].records){
                counter++;
                if(counter==iRecord)
                {
                    return iter.first;
                }
            }
        }

    }
    return -1;
}
int GetVal(char *cIndexFile, int iBlock, int iRecord){
    readFile(cIndexFile);

    for (int i = 1; i < blocksSize; ++i) {
        if(i==iBlock)
        {
            int counter=0;
            for(auto &iter:blocks[i].records){
                counter++;
                if(counter==iRecord)
                {
                    return iter.second;
                }
            }
        }

    }
    return -1;
}
int GetRecordIndex (char *cIndexFile, int iToken){
    readFile(cIndexFile);
    for (int i = 1; i < blocksSize; ++i) {
        int counter=0;
        for(auto &iter:blocks[i].records)
        {
            counter++;
            if(iter.first==iToken)
            {
                return counter;
            }
        }
    }
    return -1;
}
int GetBlockIndex (char *cIndexFile, int iToken){
    readFile(cIndexFile);
    for (int i = 1; i < blocksSize; ++i) {
        for(auto &iter:blocks[i].records)
        {
            if(iter.first==iToken)
            {
                return i;
            }
        }
    }
    return -1;
}
/*int nonEmptyBlock()
{
    for (int i = 1; i < blocks.size(); ++i) {
        if(!(blocks[i].isEmpty()))
        {
            return i;
        }
    }
    return -1;
}*/
void fromMapsToSet()
{
    int m = blocksSize;
    for (int i = 1; i < m; ++i) {
        for(auto &x:blocks[i].records)
        {
            sett.insert({x.first,x.second});
        }
    }

}
void fromSetToMaps() {
    int m = blocksSize - 1;
    int n = Size - 1;
    int CASE = m * (n - 1);
    int counter = 0;
    if (sett.size() <= n) {
        for (auto &iter:sett) {
            blocks[1].records[iter.first] = iter.second;
        }
    }
    else if (sett.size() <= CASE) {
        for (int i = 1; i < blocksSize; ++i) {
            for (auto &iter: sett) {
                blocks[i].records.insert({iter.first, iter.second});
                counter++;
                if (counter == n - 1) {
                    break;
                }
            }

            counter=0;
            set<pair<int,int>>::iterator it1;
            it1=sett.begin();

            for ( auto &iter: sett) {

                ++it1;
                counter++;
                if (counter == n-1) {
                    sett.erase(sett.begin(),it1);
                    break;
                }

            }

        }

    }

    else {
        cout<<"else"<<endl;
        for (int i = 1; i < blocksSize; ++i) {
            for (auto &iter: sett) {
                blocks[i].records.insert({iter.first, iter.second});
                counter++;
                if (counter == n ) {
                    break;
                }
            }
            counter=0;

            for (auto &iter: sett) {
                sett.erase(iter);
                counter++;
                if (counter == n ) {
                    break;
                }
            }
        }
    }
}
int updateIkey(int i)
{
    for ( int x= i+1; x < blocks.size() ; ++x) {
        if(!(blocks[x].records.empty()))
        {
            return x;
        }
    }
    return -1;
}
int firstEmptyBlock(int i){
    for ( int x= i+1; x < blocks.size() ; ++x) {
        if((blocks[x].records.empty()))
        {
            return x;
        }
    }
    return -1;
}
int updateIVal(int i)
{

    if ((blocks[i].records.empty()))
    {
        return firstEmptyBlock(i);
    }

    else {
        int max = -1;
        for (auto &iter: blocks[i].records) {
            if (iter.first > max) {
                max = iter.first;
            }
        }
        return max;
    }
}
void updateHeaders()
{
    int headFileKey;
    int headFileVal;
    for (int i = 1; i < blocks.size(); ++i) {
        if(!(blocks[i].records.empty()))
        {
            headFileKey=i;
            break;
        }
        else{
            headFileKey=-1;
        }
    }
    for (int i = 1; i < blocks.size(); ++i) {
        if((blocks[i].records.empty()))
        {
            headFileVal=i;
            break;
        }
        else{
            headFileVal=-1;
        }
    }
    blocks[0].head.ikey=headFileKey;
    blocks[0].head.ival=headFileVal;
    for (int i = 1; i < blocks.size(); ++i) {
        blocks[i].head.ival=updateIVal(i);
    }
    for (int i = 1; i < blocks.size(); ++i) {
        blocks[i].head.ikey=updateIkey(i);
    }
}
int InsertVal(char *cIndexFile, int iVal, int iKey){
    int m =blocksSize;
    int n = Size-1;
    readFile(cIndexFile);
    fromMapsToSet();
    for (int i = 1; i < m; ++i) {
        if(blocks[i].records.size()!=n )
        {
            blocks[i].records.insert({iKey,iVal});
            sett.insert({iKey,iVal});
            break;
        }
    }
    for (int i = 1; i < m; ++i) {
        blocks[i].records.clear();
    }
    fromSetToMaps();
    updateHeaders();
    int parm =n+1;
    writeFile(cIndexFile,m,parm);
    for (int i = 1; i < blocks.size(); ++i) {
        for(auto &iter:blocks[i].records)
        {
            if(iter.first==iKey)
            {
                return i;
            }
        }
    }
    return -1;
}
block parse(string head)
{   string a="";
    string first="";
    string second="";
    block b;
    int counter=-1;
    for (int i = 0; i < head.length(); ++i) {
        if(head[i]==',')
        {
            first=a;
            a="";
        }
        else if(head[i]=='|')
        {
            second=a;
            a="";
            if(!first.empty()&& !second.empty()&&first!=" "&&second!=" ")
            {
                int in1=stoi(first);
                int in2= stoi(second);
                counter++;
                b.records.insert({in1,in2});
                first="";
                second="";
            }
        }
        else if(head[i]=='$')
        {
            second=a;
            a="";
            if(!first.empty()&& !second.empty()){
                int in1=stoi(first);
                int in2= stoi(second);
                b.head.ikey=in1;
                b.head.ival=in2;
                first="";
                second="";}
        }
        else{
            a+=head[i];
        }
    }
    return b;
}
void writeFile(char *cIndexFile,int m, int n) {
    file.close();
    file.open(cIndexFile, ios::out);
    file<<blocks[0].head.ikey << ','<< blocks[0].head.ival<<"$#";
    for(int i=1;i<blocksSize;i++)
    {
        file<<blocks[i].head.ikey << ','<< blocks[i].head.ival<<"$";
        int realRecords=0;
        for (auto & i:blocks[i].records)
        {
            realRecords++;
            file<<i.first <<',' << i.second << '|';
        }
        if (realRecords<n-1){
            for (int x=realRecords;x<n-1;x++){
                file <<" , |";
            }
        }
        if (i!=blocks.size()-1)
            file << '#';
    }
    file.close();
}
void readFile(char *cIndexFile)
{
    blocks.clear();
    file.close();
    file.open(cIndexFile,ios::out|ios::in);
    while (!(file.eof()))
    {
        block b;
        string str;
        getline(file,str,'#');
        b=parse(str);
        blocks.push_back(b);
    }
    file.close();
}

block readBlocks(char *cIndexFile,int i,int n){
    file.open(cIndexFile,ios::out|ios::in);
    int indx = 1+((i*n)-n);
    file.seekg(indx);
    string s;
    getline(file,s,'#');
    file.close();
    return parse(s);
}

void InsertRecord(char *cIndexFile,int iVal, int iKey){
    block b;
    readFile(cIndexFile);
    int m = blocksSize;
    int n = Size;
    for(int i=1;i<m;i++){
        b= readBlocks(cIndexFile,i,n);
        int count=0;
        for (auto & ii:b.records) { count++; }
        if (count<n){
            InsertVal(cIndexFile,iVal,iKey);
        }
    }
}

bool CreateRecordFile(char *cIndexFile, int m, int n)
{
    Size=n;
    blocksSize=m;
    int counter=0;
    file.open(cIndexFile,ios::out);
    if(file.fail())
    {
        file.close();
        return false;
    }else {
        for (int i = 0; i < m; ++i) {
            block b;
            if(i==0)///header block
            {

                counter++;
                file<<"-1,"<<counter<<"$";
            }
            else {
                if(i!=m-1){
                    counter++;
                    file<<"-1,"<<counter<<"$";}
                else{
                    file<<"-1,-1$";
                }
                for (int k = 1; k < n; ++k) {
                    file<<" , |";
                }
            }
            if(!(i==m-1))
                file<<"#";
        }
        file.close();
        return true;
    }

}


void DeleteKey (char *cIndexFile, int iToken)
{
    readFile(cIndexFile);
    for (int i = 0; i < blocksSize; ++i) {
        for(auto &iter:blocks[i].records)
        {
            if(iter.first==iToken)
            {
                blocks[i].records.erase(iter.first);
            }
        }
    }
    fromMapsToSet();
    for (int i = 1; i < blocksSize; ++i) {
        blocks[i].records.clear();
    }
    fromSetToMaps();
    updateHeaders();

    writeFile(cIndexFile,blocksSize,Size);
}


int main() {

    CreateRecordFile("cIndexFile.txt", 5, 4);


    InsertRecord("cIndexFile.txt", 4, 6);
    InsertRecord("cIndexFile.txt", 3, 5);
    InsertRecord("cIndexFile.txt", 1, 3);
    InsertRecord("cIndexFile.txt", 11, 11);
    InsertRecord("cIndexFile.txt", 9, 8);

    // DeleteKey("cIndexFile.txt",5);


    for (auto &ii: sett) {
        cout << ii.first << "," << ii.second << endl;
    }
    for (int i = 0; i < blocks.size(); ++i) {
        if (i == 0) {
            cout << "head: " << blocks[i].head.ikey << "," << blocks[i].head.ival << endl;
        } else {
            cout << "HEAD" << blocks[i].head.ikey << "," << blocks[i].head.ival << endl;
            for (auto &iter: blocks[i].records) {
                cout << "record: " << iter.first << "," << iter.second << endl;
            }
        }

    }


    return 0;
}