
#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>

using namespace std;

int main() {
    int allocatedPages;
    vector<int> pageReferences;
    string algorithm;
    cin >> allocatedPages;
    cin >> algorithm;

    int temp;
    for (int i = 0;; ++i) {
        cin >> temp;
        if (temp == -1)
            break;
        pageReferences.push_back(temp);
    }


    return 0;
}

void FIFO(vector<int> pageReferences, int allocatedPages) {

    queue<int> frames;
    unordered_set<int> inMemSet;
    int pageFaultsCounter = 0, pageFaultFlag = 0;
    cout << "Replacement Policy = FIFO\n-------------------------------------\n"
            "Page   Content of Frames\n"
            "----   -----------------";

    for (auto &&reference  :pageReferences) {//iterate page references
        printf("%2d", reference);
        pageFaultFlag = 0;
        if (frames.size() < allocatedPages)//if frames allocated still not full
            if (inMemSet.find(reference) != inMemSet.end()) {//if the page isn't already in a frame

                frames.push(reference);//add the page to a frame
                inMemSet.insert(reference);//add the page to the set of pages in frames
            } else if (inMemSet.find(reference) !=
                       inMemSet.end()) {//if all of the frames are full and the page referenced isn't in a frame
                pageFaultFlag = 1;
                pageFaultsCounter++;
                //removing the first page
                inMemSet.erase(frames.front());
                frames.pop();
                //adding the referenced page
                frames.push(reference);
                inMemSet.insert(reference);

            }

        if (pageFaultFlag)
            cout << " F   ";
        else cout << "     ";

    }


}

void printFrames(queue<int> frames) {

}