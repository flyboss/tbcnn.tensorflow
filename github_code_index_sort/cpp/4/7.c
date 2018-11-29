/*
 *  Copyright(c) 2017 Gang Zhang
 *  All rights reserved.
 *  Author: Gang Zhang
 *  Creation date: 2017.3.15
 *  Last modified: 2017.3.15
 *
 *  Function:
 *      Test for the priority queue.
 */

#include "priority_queue.h"
#include <ctime>

int main() {
    cout << "Step 1: there is a priority queue with four elements {9,8,5,6} in it,"
         << " you need to insert some integers into it again. Once you"
         << " input one, you will see the front element in the"
         << " priority queue. After you finish inserting data, two elements"
         << " will be popped from the queue, also you will see the first"
         << " element in the queue." << endl
         << endl;

    /* to build a priority queue. */
    Priority_Queue<int> heap{9, 8, 5, 6};

    /* insert some integers into the priority queue. */
    cout << "Insert some integers ending up with 0:" << endl;
    int n;
    while (cin >> n && n != 0) {
        heap.push(n);
        cout << "front = " << heap.front() << endl;
    }

    /**
     * pop two elements in the priority queue.
     * and display first element in it.
     */
    heap.pop();
    cout << "front = " << heap.front() << endl;
    heap.pop();
    cout << "front = " << heap.front() << endl;

    cout << "\nStep 2: here you need to input data of a array,and then "
         << "you will see result of the function findKth and the time they "
         << "each has cost. At the end, you can see heap-sort result of the array.\n"
         << endl;

    /* input data of the array. */
    vector<int> num;
    cout << "Please input the data ending up with 0:" << endl;
    while (cin >> n && n != 0)
        num.push_back(n);

    /* result of the two functions findKth and their accuracy. */
    clock_t t1, t2;
    t1 = clock();
    cout << "\nUsing min-heap:\nThe " << num.size() / 3 << "th largest number is " << findKth(num, num.size() / 3)
         << "." << endl;
    t2 = clock();
    cout << "time = " << double(t2 - t1) / CLOCKS_PER_SEC << "s" << endl;
    t1 = clock();
    cout << "\nUsing ordered array:\nThe " << num.size() / 3 << "th largest number is " << findKth2(num, num.size() / 3)
         << "." << endl;
    t2 = clock();
    cout << "time = " << double(t2 - t1) / CLOCKS_PER_SEC << "s" << endl;

    /* result of the heap-sort. */
    cout << "\nHeap-sort result:" << endl;
    heapSort(num);
    size_t cnt = 0;
    for (int n : num) {
        cout << n << "\t";
        if (++cnt % 10 == 0) cout << endl;
    }
    cout << endl;

    return 0;
}