#include <iostream>
#include <vector>
// #include <set>
#include <string>
// #include <tuple>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <map>
#include <typeinfo>
// #include <iterator>
using namespace std;
typedef pair<string,int> mypair;
typedef pair<int,int> item;

struct FPnode{
    string name;
    int frequent;
    FPnode* link;
    FPnode* parent;
    FPnode* child;
    FPnode(string name,FPnode* parent,int frequent =1):name(name),parent(parent),frequent(frequent){
        link = NULL;
    }

    bool isRoot(){
        return (parent==NULL);
    }
    void addCount(FPnode x){
            x.frequent += 1;
        }
    int getCount(FPnode x){
        return x.frequent;
    }
};

class FPtree{
    private:
        FPnode* root;
        vector<int> headerTable; // maybe map??
        int minSupport; //min-support
        map<string,int> Itemfrequent; // each item frequency
        vector<mypair> ItemfrequentVec; // each item frequency (DS:vector)
        vector<vector<int>> newTrans; // new transaction, after delete <minSupport item
        vector<vector<item>> sortTra; // just for sorting new transaction
        vector<vector<int>> sortedTra; //last transaction, after sorting newTrans
    public:
        // void addCount(FPnode x){
        //     x.frequent += 1;
        // }
        // int getCount(FPnode x){
        //     return x.frequent;
        // }
        
        //traversal transactions and show 
        void showTrans(vector<vector<int>> transactions){
            for(int i =0;i<transactions.size();i++){
                for(int j=0;j<transactions[i].size();j++){
                    cout<<transactions[i][j];
                }
                cout<<endl;
            }
            // cout<<typeid(transactions[1][2]).name();
        }
        
        // traversal frequent map and show 
        void showMap(){
            cout<<"showMap : "<<endl;
            for(map<string, int>::iterator iter = Itemfrequent.begin(); iter != Itemfrequent.end(); iter++){
                cout<<iter->first<<" : "<<iter->second<<endl;
            }
        }

        void showVec(){
            cout<<"showVec : "<<endl;
            for(int i=0;i<ItemfrequentVec.size();i++){
                cout<<ItemfrequentVec[i].first<<"\t"<<ItemfrequentVec[i].second<<endl;
            }
        }

        void setMinSupport(int x){
            minSupport = x;
        }

        //show NewTrans (delete < minSupport item in transaction)
        void showNewTra(){
            cout<<"show NewTra : "<<endl;
            for(int i=0;i<newTrans.size();i++){
                for(int j=0;j<newTrans[i].size();j++){
                    cout<<newTrans[i][j];
                }
                cout<<endl;
            }
        }

        //show sortedTra (sorted transaction by decreasing)
        void showSortTra(){
            cout<<"show sortedTra : "<<endl;
            for(int i=0;i<sortedTra.size();i++){
                for(int j=0;j<sortedTra[i].size();j++){
                    cout<<sortedTra[i][j]<<"\t";
                }
                cout<<endl;
            }
        }

        // scan the transactions-DB and count frequent of each item 
        void scanDB(vector<vector<int>> transactions){
            for ( const auto &nowRow : transactions){
                for(const auto &nowItem : nowRow){
                    stringstream  s;
                    s<<nowItem;
                    string nowName = s.str();
                    Itemfrequent[nowName] += 1;
                    // cout<<nowItem<<"\t";
                }
            }
            checkFre();
        }
  
        // check whether item frequent is bigger enough or not
        void checkFre(){
            map<string, int>::iterator iter;
            for(auto iter = Itemfrequent.cbegin();iter!=Itemfrequent.cend();){
                string name = (*iter).first;
                int count = (*iter).second;
                if( count < minSupport ){
                    Itemfrequent.erase(iter++);
                }
                else{
                    ++iter;
                }
            }
            sortItem();
        }

        struct MySort{
            public:
                bool operator()(const mypair &p1,mypair &p2) const{
                    return p1.second > p2.second;
                }
        };

        //sort item frequent by map-value , decreasing
        void sortItem(){
            vector<mypair> tempVec(Itemfrequent.begin(),Itemfrequent.end());
            sort(tempVec.begin(),tempVec.end(),MySort());
            for(int i=0;i<tempVec.size();i++){
                ItemfrequentVec.push_back(tempVec[i]);
            }
        }

        // scan transaction again and sort each item in transaction 
        // decreasing
        
        void generateNewTrans(vector<vector<int>> transactions){
            // scan transaction again , delete < minSupport item
            vector<item> rowForSort;
            vector<int> row;
            map<string, int>::iterator iter;
            for(int i=0;i<transactions.size();i++){
                for(int j=0;j<transactions[i].size();j++){
                    stringstream s;
                    s<<transactions[i][j];
                    string numStr = s.str();
                    iter =  Itemfrequent.find(numStr);
                    if(iter != Itemfrequent.end()){
                        row.push_back(transactions[i][j]);
                        rowForSort.push_back(make_pair(transactions[i][j],iter->second));
                    }
                }
                sortTra.push_back(rowForSort);
                newTrans.push_back(row);
                row.clear();
                rowForSort.clear();
            }
            sortingTra();
        }

        // sorting new transaction by decreasing order
        void sortingTra(){
            struct newTra{
                public:
                    bool operator()(item a,item b){
                        return a.second > b.second;
                    }
            };
            
            //(sortTra vector<vector<item>> ,item = <int,int>)
            for(int i=0;i<sortTra.size();i++){
                sort(sortTra[i].begin(),sortTra[i].end(),newTra());
            }
            vector<int> row;
            for(int i=0;i<sortTra.size();i++){
                for(int j=0;j<sortTra[i].size();j++){
                    // cout<<sortTra[i][j].first;
                    row.push_back(sortTra[i][j].first);
                }
                cout<<"\n";
                sortedTra.push_back(row);
                row.clear();
            }
            
        }

        // construction fptree
        void constructionTree(){
            // FPnode* cur = &Root; //pointer cur point to root's address
            // bool isNew = true;
            // for(int i=0;i<ItemfrequentVec.size();i++){
            //     string name = ItemfrequentVec[i].first;
            //     int count = ItemfrequentVec[i].second;
                
            // }
        }


        
};


class InputReader{
    private:
        ifstream filein;
        vector<vector<int>> transactions;
    public:
        InputReader(){
            cout<<"Wanna show something?"<<endl;
        }
        InputReader(string filename) {
            filein.open(filename);
            if(!filein) {
                cout << "Input file could not be opened\n";
                exit(0);
            }
            analysize();
            filein.close();
        }
        void analysize(){
            string buffer;
            while(!getline(filein, buffer).eof()){
                vector<int> arr;
                int pre = 0;
                for(int i=0;i<buffer.size();i++){
                    if(buffer[i] == '\t') {
                        int num = atoi(buffer.substr(pre, i).c_str());
                        arr.push_back(num);
                        pre = i+1;
                    }
                }
                int num = atoi(buffer.substr(pre, buffer.size()).c_str());
                arr.push_back(num);
                
                transactions.push_back(arr);
            }
        }
        vector<vector<int> > getTransactions() {
            return transactions;
        }
};


int main(){
    InputReader reader("test.txt");
    vector<vector<int> > transactions = reader.getTransactions();
    FPtree tree;
    tree.setMinSupport(2);
    tree.scanDB(transactions);
    tree.generateNewTrans(transactions);
}
