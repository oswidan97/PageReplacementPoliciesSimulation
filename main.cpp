
#include <iostream>
#include <vector>
#include <unordered_set>
#include <deque>
#include <iomanip>

using namespace std;
void printFrames(vector<int> frames);
void printFooter(int pageFaults);
void printHeader(string policy);
void FIFO(vector<int> pageReferences, int allocatedPages);
int main() {
    int allocatedPages;
    vector<int> pageReferences;
    string policy;
    cin >> allocatedPages;
    cin >> policy;

    int temp;
    for (int i = 0;; ++i) {
        cin >> temp;
        if (temp == -1)
            break;
        pageReferences.push_back(temp);
    }
    FIFO(pageReferences,allocatedPages);
    printHeader(policy);

    return 0;
}
void LRU(vector<int> pageReferences,int allocatedPages){

}
void FIFO(vector<int> pageReferences, int allocatedPages) {

    vector<int> frames;
    unordered_set<int> inMemSet;
    int pageFaultsCounter = 0, pageFaultFlag = 0,firstElementPointer=0;


    for (auto &&reference  :pageReferences) {//iterate page references
        if (reference==-1)
            break;
        cout<<setw(2)<<setfill('0')<<reference;
        pageFaultFlag = 0;


        if (frames.size() < allocatedPages){//if frames allocated still not full
            if (inMemSet.find(reference) == inMemSet.end()) {//if the page isn't already in a frame
                frames.push_back(reference);//add the page to a frame
                inMemSet.insert(reference);//add the page to the set of pages in frames
            }
        } else if (inMemSet.find(reference) == inMemSet.end()) {//if all of the frames are full and the page referenced isn't in a frame
                pageFaultFlag = 1;
                pageFaultsCounter++;
                //removing the first page
                inMemSet.erase(frames[firstElementPointer]);
                frames[firstElementPointer]=reference;
                //adding the referenced page
            (firstElementPointer+=1)%=allocatedPages;//increase pointer but not beyond the max size
                inMemSet.insert(reference);

            }

        if (pageFaultFlag)
            cout << " F   ";
        else cout << "     ";
        printFrames(frames);

    }
    printFooter(pageFaultsCounter);



}
void printHeader(string policy){
    cout << "Replacement Policy = "<<policy<<"\n-------------------------------------\n"
            "Page   Content of Frames\n"
            "----   -----------------\n";
}
void printFooter(int pageFaults){
    cout<<"-------------------------------------\n"
            "Number of page faults = "<<pageFaults;

}

void printFrames(vector<int> frames) {
        for (auto && frame :frames )
            cout << setw(2) << setfill('0') << frame << " ";

    cout<<"\n";

}