# B+ Trees

This is the router for all the data. For example, we saw that the Pager just stored up all the information, but the information about the Pager is kept in this one. The primary key is the representative for this one, and the offset of the file and the slot number are the additional information which are required to access the thing.

Let’s have a very basic overview of the structure of the B+ Trees.

There is the data, the one I was talking about above. These are considered as **leafs**, and the ones which are above them are known as **internal nodes**.

The internal node consists of all the primary keys used by all the children so that we can route and the next one points to the right next struct directly, and then we can get the thing.

## Insertion

Coming to the main functions of the B+ Trees is the insertion of the data in the thing directly.

The very first thing to note down is the path with which we need to go through all the nodes, which index number it is. We store it up using the binary search on each and every node which is there in the parents, and similarly we get the index where we need to put the data at the leaf as well.

We move forward to insertion.

The best one is if the space is there in the node. Each and every node has a max capacity to hold the data, which is just **340**. So if there is some space, we put the data directly in the thing and update the counters in the struct.

If not, then it is quite a big stuff for it.

First, let’s go through the logic which I had in mind. Pretty much like we have the stack with us having all the nodes where we need to put the data, and now we just see that we are at the leaf thing and we need to insert the thing in it.

So what we do is we go up in the tree, up in the parents, the **bottom-up approach** first. Then we just update all the representatives which are there at the top, which is the middle of all the primary keys which are there in the struct.

We use them and then go up, put the updated representative in the upper parent, and then move forward the same way till we don’t reach the parent which has some space for splitting of the data.

So till the way up, the representative which stayed, we just put there, and then from there we just go from **top-down approach**.

We go on splitting all the nodes which we have in the stack, and then once we reach the end, we split the data as well and then insert the data.

And if there is just no space in the entire stack, we need to make a new one and then put all the data in the thing and then move forward with the similar approach as I conveyed up.

## Current Issue

Now comes the main issue. It doesn’t work pretty much like I am facing the segmentation errors everywhere.

The issue is in the stack and the indexes which we are handling. Somewhere the pointers are gone and I just can’t find the solution.

So I have quoted the code and probably raised as an issue as well. If someone is curious and wants to fix up the thing, here is the approach.

