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
// #include <iterator>
using namespace std;
typedef pair<string,int> mypair;

struct FPnode{
    string name;
    int frequent;
    FPnode* link;
    FPnode* parent;
    FPnode* child;
    // FPnode(string name,FPnode* parent,int frequent =1):name(name),parent(parent),count(count){
    //     link = NULL;
    // }

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
        map<string,int> Itemfrequent; 
        vector<mypair> ItemfrequentVec;
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
                cout<<ItemfrequentVec[i].first<<" : "<<ItemfrequentVec[i].second<<endl;
            }
        }

        void setMinSupport(int x){
            minSupport = x;
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
                cout<<"in checkfre's for!"<<endl;
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

        // construction fptree
        void constructionTree(){
            // vector<mypair>::iterator it;
            // for (it = ItemfrequentVec.begin(); it != ItemfrequentVec.end();it++){

            // }
            FPnode* curr = root;
            for(int i=0;i<ItemfrequentVec.size();i++){
                string name = ItemfrequentVec[i].first;
                int count = ItemfrequentVec[i].second;
                
            }
            bool isnew = true;
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
}
