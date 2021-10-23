#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <tuple>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <map>
using namespace std;

struct FPnode{
    string name;
    int frequent;
    FPnode* link;
    FPnode* parent;
    FPnode* child;
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
        long support; //min-support
        map<string,int> Itemfrequent; 
        // vector<vector<int>> transactions;
    // FPtree(){

    // }
    public:
        // void addCount(FPnode x){
        //     x.frequent += 1;
        // }
        // int getCount(FPnode x){
        //     return x.frequent;
        // }
        
        void scanDB(vector<vector<int>> transactions){
            for(int i =0;i<transactions.size();i++){
                for(int j=0;j<transactions[i].size();j++){
                    cout<<transactions[i][j];
                }
                cout<<endl;
            }
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

            /* print all transaction*/ 

            // for(int i=0;i<transactions.size();i++){
            //     for(int j=0;j<transactions[i].size();j++){
            //         cout<<transactions[i][j];
            //     }
            //     cout<<endl;
            // }

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
    tree.scanDB(transactions);
}
