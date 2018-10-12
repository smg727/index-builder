#include <iostream>
#include "utils.h"
#include "helper.h"

using namespace std;

int main() {
    cout << "Warning: Huge program starting" << std::endl;

    // used to run tests.
    //tupleTest();

    // generates postings from all wet files
    // warning: this is a heavy operation do not run unless you want
    // to convert all wet files to postings
    // generatePostings();

    // create one large posting file using unix sort

    // transform one large posting file to an inverted index
    createInvertedIndex();

    return 0;
}