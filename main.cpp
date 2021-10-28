#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <typeinfo>
#include <list>
using namespace std;

typedef pair<string,int> mypair;
typedef pair<int,int> item;
int const MAX = 10000;

struct FPnode{
    string name;
    int frequent;
    FPnode* parent;
    FPnode* sibling;
    map<string,FPnode*> child;
    
    FPnode(string name,FPnode*parent,int frequent = 1):name(name), parent(parent), frequent(frequent) {
        sibling = NULL;
        child.clear();
    }
    bool isRoot(){
        return (parent==NULL);
    }
};

class FPtree{
    private:
        FPnode* root;
        map<string,FPnode*> headerTable; // maybe map??
        int minSupport; //min-support
        map<string,int> Itemfrequent; // each item frequency
        vector<mypair> ItemfrequentVec; // each item frequency (DS:vector)
        vector<vector<int>> newTrans; // new transaction, after delete <minSupport item
        vector<vector<item>> sortTra; // just for sorting new transaction
        vector<vector<int>> sortedTra; //last transaction, after sorting newTrans

    public:
        
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

        void showSibling(){
            cout<<"show sibling : "<<endl;
            map<string,FPnode*>::iterator iter = headerTable.find("7");
            if( iter != headerTable.end()){
                FPnode *curr = iter->second;
                while(curr != NULL){
                    cout<<"-------------------------"<<endl;
                    cout<<"curr node name : "<<curr->name<<endl;
                    cout<<"parent name : "<<curr->parent->name<<endl;
                    cout<<"-------------------------"<<endl;
                    curr = curr->sibling;
                }
            }
        }
        void showHeader(){
            map<string,FPnode*>::iterator iter = headerTable.begin();
            while(iter != headerTable.end()){
                cout<<"node name : "<<iter->first<<endl;
                iter++;
            }
        }

        //extract item (in sortedTra) frequent prefix patten
        //conditional patten base
        void findPrefixPath(){
            map<string,FPnode*>::iterator iter ;
            for(iter = headerTable.end();iter--!=headerTable.begin();){
                // every time fix one item , find all prefix path
                map<vector<string>,int> prefix;
                prefix.clear();
                //[a,b,c,d]:2 (prefix path:prefix frequent)
                //prefix's frequent depand on leaf node's frequency
                FPnode *curr = iter->second;
                FPnode *currCpy = curr;
                cout<<"current find : "<<iter->first<<endl;
                while(currCpy != NULL){
                    //find all path for this node
                    vector<string> path;
                    path.clear();
                    int minFre = MAX; 
                    int nowFre = MAX;
                    while(curr != NULL && curr->parent->name !="root"){
                        //one path for each node
                        nowFre = curr->frequent;
                        if (nowFre<minFre){
                            minFre = nowFre;
                        }
                        path.push_back(curr->parent->name);
                        curr = curr->parent;
                    }
                    if(!path.empty()){
                        prefix[path] = minFre;
                    }
                    
                    /* just for check conditional patten base (path & fre)*/
                    // cout<<"now path : ";
                    // for(int i=0;i<path.size();i++){
                    //     cout<<path[i];
                    // }
                    // cout<<"\t ,freq : "<<prefix[path]<<endl;                    

                    curr = currCpy -> sibling;
                    currCpy = curr;
                }

                /* just for check conditional patten base (prefix)*/
                // map<vector<string>,int>::iterator s;
                // s = prefix.begin();
                // while(s!=prefix.end()){
                //     vector<string> p = s->first;
                //     if(s->second==0){
                //         continue;
                //     }
                //     for(int i=0;i<p.size();i++){
                //         cout<<p[i];
                //     }
                //     cout<<","<<s->second<<endl;
                //     s++;
                // }

                getFrePatt(prefix,iter->first);
            }
        }

        void getFrePatt(map<vector<string>,int> prefix,string fix){
            FPnode temp("temp",NULL);
            int nodeFre = Itemfrequent[fix];
            map<vector<string>,int>::iterator it;
            it = prefix.begin();
            while(it!=prefix.end()){ //each row of prefix           
                FPnode *curr = &temp;
                vector<string> tag = it->first;
                // cout<<"current tag:  ";
                // for(int i=0;i<tag.size();i++){
                //     cout<<tag[i];
                // }
                // cout<<"\n";

                int eachFre = it->second;
                for(int i=tag.size()-1;i>=0;i--){
                    map<string,FPnode*>::iterator iter = curr->child.begin();
                    while(iter!=curr->child.end()){
                        if(iter->first == tag[i]){
                            // cout<<tag[i]<<" same! \n";
                            curr = curr->child[tag[i]];
                            curr->frequent += eachFre;
                            break;
                        }
                        iter++;
                    }
                    if(iter == curr->child.end()){
                        // cout<<"create "<<tag[i]<<endl;
                        curr->child[tag[i]] = new FPnode(tag[i],curr,eachFre);
                        curr = curr->child[tag[i]];
                    }
                }
                it++;
            }

            FPnode *checkMinSupport = &temp;
            checkMin(checkMinSupport);
            /* just for check !*/
            FPnode *check = &temp;
            traversal(check);
        }

        void checkMin(FPnode *checkMinSupport){
            if(checkMinSupport){
                map<string,FPnode*>::iterator checkit = checkMinSupport->child.begin();
                while(checkit != checkMinSupport->child.end()) {
                    FPnode* next = checkit->second;
                    if(next->frequent<minSupport){
                        checkMinSupport->child.erase(checkit);
                    }
                    checkMin(next);
                    checkit++;
                }
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
                sortedTra.push_back(row);
                row.clear();
            }
            
        }

        vector<vector<int>> getSortedTra(){
            return sortedTra;
        }

        // construction fptree
        void myContrust(FPnode *root){
            for(int i=0;i<sortedTra.size();i++){
                FPnode *curr = root;
                for(int j=0;j<sortedTra[i].size();j++){
                    stringstream ss;
                    ss<<sortedTra[i][j];
                    string s = ss.str(); // s == name
                    map<string,FPnode*>::iterator it = curr->child.begin();
                    while(it!=curr->child.end()){
                        // cout<<"check ! "<<it->first<<endl;
                        if(it->first == s){
                            // cout<<"no need to create new node !"<<endl;
                            curr = it->second;
                            curr->frequent +=1;
                            break;
                        }
                        it++;
                    }
                    if(it==curr->child.end()){
                        // cout<<"create new node! name : "<<s<<endl;
                        curr->child[s] = new FPnode(s,curr,1);
                        curr = curr->child[s];
                        if(headerTable.find(s) != headerTable.end()){
                            curr->sibling = headerTable[s];
                        }
                        headerTable[s] = curr;
                    }
                }
            }
        }

        void traversal(FPnode *current){
            if(current){
                cout<<"-------------------------"<<endl;
                cout<<"name : "<<current->name<<"\t";
                cout<<"count : "<<current->frequent<<"\n";
                cout<<"-------------------------"<<endl;
                map<string,FPnode*>::iterator it = current->child.begin();
                while(it != current->child.end()) {
                    FPnode* next = it->second;
                    traversal(next);
                    // cout<<"next name : " <<next->name<<endl;
                    it++;
                }
            }
        }

};


class InputReader{
    private:
        ifstream filein;
        vector<vector<int>> transactions;
    public:
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
    InputReader reader("test3.txt");
    vector<vector<int> > transactions = reader.getTransactions();
    FPnode root("root",NULL);
    FPtree tree;
    tree.setMinSupport(2);
    tree.scanDB(transactions);
    tree.generateNewTrans(transactions);
    tree.myContrust(&root);
    tree.findPrefixPath();
}
