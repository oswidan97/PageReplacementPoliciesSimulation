
#include <iostream>
#include <vector>
#include <unordered_set>
#include <iomanip>
#include <bits/unordered_map.h>
#include <list>

using namespace std;

void printFrames(vector<int> frames);

void printFramesClock(vector<pair<int,int>> frames);

void printFooter(int pageFaults);

void printHeader(string policy);

void LRU(vector<int> pageReferences, int allocatedPages);

void FIFO(vector<int> pageReferences, int allocatedPages);

void CLOCK(vector<int> pageReferences, int allocatedPages);



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
    printHeader(policy);
    if(policy=="CLOCK")
        CLOCK(pageReferences, allocatedPages);
    else if(policy=="LRU")
        LRU(pageReferences,allocatedPages);
    else if(policy=="FIFO")
        FIFO(pageReferences,allocatedPages);


    return 0;
}

int getPageForReplacement(vector<pair<int, int>> &frames, int &clockArmPosition, int allocatedPages) {
    while (1) {
        if (frames[clockArmPosition].second == 0) {

            int temp=clockArmPosition;
            clockArmPosition++;
            clockArmPosition%=allocatedPages;
            return temp;
        }
        frames[clockArmPosition].second = 0;
        clockArmPosition++;
        clockArmPosition %= allocatedPages;
    }
}

void CLOCK(vector<int> pageReferences, int allocatedPages) {
    vector<pair<int, int>> frames;
    unordered_set<int> inMemSet;
    unordered_map<int, int> framesOfPages;
    int pageFaultsCounter = 0, pageFaultFlag = 0, clockArmPosition = 0;

    for (auto &&reference  :pageReferences) {//iterate page references
        if (reference == -1)
            break;
        cout << setw(2) << setfill('0') << reference;
        pageFaultFlag = 0;

        if (frames.size() < allocatedPages) {//if frames allocated still not full
            if (inMemSet.find(reference) == inMemSet.end()) {//if the page isn't already in a frame
                frames.push_back(make_pair(reference, 1));//add the page to a frame
                inMemSet.insert(reference);//add the page to the set of pages in frames
                framesOfPages[reference] = (int) (frames.size() - 1);
            } else {
                frames[framesOfPages[reference]].second = 1;//get the frame that contains th referenced page and make the use bit =1

            }
        } else {
            if (inMemSet.find(reference) == inMemSet.end()) {//if all of the frames are full and the page referenced isn't in a frame
                pageFaultFlag = 1;
                pageFaultsCounter++;
                int indexForReplacement = getPageForReplacement(frames, clockArmPosition, allocatedPages);
                inMemSet.erase(frames[indexForReplacement].first);
                framesOfPages.erase(frames[indexForReplacement].first);
                //adding the referenced page
                frames[indexForReplacement].first = reference;
                frames[indexForReplacement].second=1;
                inMemSet.insert(reference);
                framesOfPages[reference]=indexForReplacement;

            } else {
                frames[framesOfPages[reference]].second = 1;//get the frame that contains th referenced page and make the use bit =1

            }
        }

        if (pageFaultFlag)
            cout << " F   ";
        else cout << "     ";
        printFramesClock(frames);

    }
    printFooter(pageFaultsCounter);

}

void LRU(vector<int> pageReferences, int allocatedPages) {
    vector<int> frames;
    unordered_map<int, int> framesOfPages;
    list<int> pagesInFrames;//list of pairs,page number,frame number the page is allocated in
    unordered_set<int> inMemSet;
    int pageFaultsCounter = 0, pageFaultFlag = 0;
    for (auto &&reference  :pageReferences) {
        if (reference == -1)
            break;
        cout << setw(2) << setfill('0') << reference;
        pageFaultFlag = 0;
        if (frames.size() < allocatedPages) {//if frames allocated still not full
            if (inMemSet.find(reference) == inMemSet.end()) {//if the page isn't already in a frame
                frames.push_back(reference);//add the page to a frame
                inMemSet.insert(reference);//add the page to the set of pages in frames
                pagesInFrames.push_back(reference);
                framesOfPages[reference] = (int) (frames.size() - 1);

            } else {
                pagesInFrames.remove(reference);
                pagesInFrames.push_back(reference);
            }
        } else {
            if (inMemSet.find(reference) ==
                inMemSet.end()) {//if all of the frames are full and the page referenced isn't in a frame
                pageFaultFlag = 1;
                pageFaultsCounter++;
                //removing the page from the set
                inMemSet.erase(pagesInFrames.front());
                //get frame index from the map of page number and frame index and add the new page at the same index
                frames[framesOfPages[pagesInFrames.front()]] = reference;
                //remove the element from the map of pages and frame indexes
                framesOfPages.erase(pagesInFrames.front());
                pagesInFrames.pop_front();//remove the LRU page from the list
                //adding the referenced page
                inMemSet.insert(reference);
                pagesInFrames.push_back(reference);
                framesOfPages[reference] = (int) (frames.size() - 1);


            } else {
                pagesInFrames.remove(reference);
                pagesInFrames.push_back(reference);
            }

        }
        if (pageFaultFlag)
            cout << " F   ";
        else cout << "     ";
        printFrames(frames);

    }
    printFooter(pageFaultsCounter);

}

void FIFO(vector<int> pageReferences, int allocatedPages) {

    vector<int> frames;
    unordered_set<int> inMemSet;
    int pageFaultsCounter = 0, pageFaultFlag = 0, firstElementPointer = 0;


    for (auto &&reference  :pageReferences) {//iterate page references
        if (reference == -1)
            break;
        cout << setw(2) << setfill('0') << reference;
        pageFaultFlag = 0;


        if (frames.size() < allocatedPages) {//if frames allocated still not full
            if (inMemSet.find(reference) == inMemSet.end()) {//if the page isn't already in a frame
                frames.push_back(reference);//add the page to a frame
                inMemSet.insert(reference);//add the page to the set of pages in frames
            }
        } else if (inMemSet.find(reference) == inMemSet.end()) {//if all of the frames are full and the page referenced isn't in a frame
            pageFaultFlag = 1;
            pageFaultsCounter++;
            //removing the first page
            inMemSet.erase(frames[firstElementPointer]);
            frames[firstElementPointer] = reference;


            firstElementPointer++;
            firstElementPointer %= allocatedPages;//increase pointer but not beyond the max size
            //adding the referenced page
            inMemSet.insert(reference);

        }

        if (pageFaultFlag)
            cout << " F   ";
        else cout << "     ";
        printFrames(frames);

    }
    printFooter(pageFaultsCounter);


}

void printHeader(string policy) {
    cout << "Replacement Policy = " << policy << "\n-------------------------------------\n"
            "Page   Content of Frames\n"
            "----   -----------------\n";
}

void printFooter(int pageFaults) {
    cout << "-------------------------------------\n"
            "Number of page faults = " << pageFaults;

}

void printFrames(vector<int> frames) {
    for (auto &&frame :frames)
        cout << setw(2) << setfill('0') << frame << " ";

    cout << "\n";

}
void printFramesClock(vector<pair<int,int>> frames){
    for (auto &&frame :frames)
        cout << setw(2) << setfill('0') << frame.first << " ";

    cout << "\n";
}