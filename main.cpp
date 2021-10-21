#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <tuple>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;

class TreeNode{
    private:
        int count;
        string name;
        TreeNode *link;
        TreeNode *parent;
        TreeNode *children;
    public:
    //     int addCount(string name){
    //         return ++count;
    //     }
        
};
class FPtree{
    private:
        vector<vector<int>> transactions;
        vector<TreeNode*> a[20]; //todo : should change to transaction.size() but how?
    public:
        FPtree(){
            
        }
        void createFPtree(vector<vector<int>> transactions,double minSup){
            for(int i=0;i<transactions.size();i++){
                
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
}
