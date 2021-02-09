# Hashtag-Counter

Aim: To find out the n most popular hashtags trending on social media. As the hashtag frequencies are ever changing, there is a need to implement a system that can efficiently support: Incrementing the frequency of a given hashtag and Extracting the most popular hashtag

The following data structures are used for the implementation:

1. Fibonacci Heap: Implement a max priority queue based on the frequencies of the hashtags. (Increase Key runs in O(1) in amortized sense and Remove Max run in O(log n) in amortized sense)
2. Hash Table: A table to keep track of hashtags in the heap, with pointers to the nodes of the Fibonacci Heap.
