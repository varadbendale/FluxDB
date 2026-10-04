# B+ Trees

This is the router for all the data. For example, we saw that the Pager just stored all the information, but the information about the Pager is kept in this one. The primary key is the representative for this one, and the offset of the file and the slot number are the additional information which are required to access the thing.

Let’s have a very basic overview of the structure of the B+ Trees. There is the data, the one I was talking about above. These are considered as leafs, and the ones which are above them are known as internal nodes. The internal node consists of all the primary keys used by all the children so that we can route, and the next one points to the right next struct directly, and then we can get the thing.

Coming to the main function of the B+ Trees, it is the insertion of the data in the thing directly. So, the very first thing is to note down the path with which we need to go through all the nodes and which index number it is in the thing. We store it using binary search on each and every node which is there in the parents, and similarly, we get the index where we need to put the data at the leaf as well. We then move forward to insertion.

The best case is if there is space in the node. Each and every node has a max capacity to hold the data, which is just 340. So, if there is some space, we put the data directly in the thing and update the counters in the struct.

If not, then it is quite a big thing. First, let’s go through the logic which I had in mind. Pretty much, we have the stack with us having all the nodes where we need to put the data, and now we just see that we are at the leaf and we need to insert the thing in it. So, what we do is we go up in the tree, up in the parents, using the bottom-up approach first. Then we just update all the representatives which are there at the top, which is the middle of all the primary keys which are there in the struct. We use them and then go up, put the updated representative in the upper parent, and then move forward the same way till we reach the parent which has some space for splitting the data.

So, till the way up, the representative which stayed, we just put there, and then from there we just go from the top-down approach. We go on splitting all the nodes which we have in the stack, and then once we reach the end, we split the data as well and then insert the data. And if there is just no space in the entire stack, we need to make a new one and then put all the data in the thing and then move forward with the similar approach as I conveyed above.

Now comes the main issue. It doesn’t work pretty much like I am facing segmentation errors everywhere. The issue is in the stack and the indexes which we are handling. Somewhere the pointers are gone and I just can’t find the solution. So, I have quoted the code and probably raised an issue as well. If someone is curious and wants to fix up the thing, here is the approach, and I am noting down what exactly was in my mind while doing it exactly below.

As I said about the bottom-up approach, the representative is updated every time. We use the variable which tracks the representative which needs to be the thing, `catch_which_to_put_as_parent`, and go down the stack. Then we start the main thing. We start a loop which goes from the index to the size of the stack and move across them. The first thing we take is a temporary variable which uses the node as per the way we move in the thing.

Now, if the thing is 0, meaning the very first or the newest node which we made, we select that one up. If not, we just pick the current one and move forward. Then, about the turner, it is like this: once we find the space in the node to go forward, we have to rearrange all the stuff, all the primary keys, and then insert our last representative and move forward with all the rest of the iterations. This thing is not used for obvious reasons.

Then we start making one temporary half node which we transfer all the nodes or children into, and then merge it up with the main branch later on. So, we start by copying half the primary keys into the new node. Then, if the node is not the leaf, meaning it does not have any children and it has the data, we just split it up and add the node to the parent.

Pretty much, we make another new temporary node. We go a bit back to the parent for the current split node and then pick the node and add the new half node, and then move forward. Also, we need to update the stack as per any of the nodes which are there. So, we have the index term stored up for every node. We just look in which of the nodes it lies and then put it in the stack and move forward. My thinking is that we directly update the pointer node, so no external work is needed. That was my thought process.

Now, moving to the else version, if it is the data, the normal data, we just need to go one node earlier and access the parent of the current node, as it is the one who controls all the child nodes, and the data is just the data in the thing. So, we go one earlier. The same way, we split up all the things, the primary keys and the data, and then insert the newer one in the thing. Done.

This one is quite resolved now. The issue was the same: I was using the stack thing to change the stuff, and the pointers were the issue in here. So, pretty much, the thing was simpler. The whole logic is the same, just a few of the unnecessary conditions which I had put in were not needed. The main splitting logic covers that one as well, so I removed it.

Then I just used only two variables. The one which takes care of the internal node, which we just use, and we track that one up. Also, while splitting, we just use that one and add stuff in it.

Anyone reading this, AI doesn’t understand the logic or even when we give the approach, it doesn’t. So, it is quite the thing. Just give the whole working process of it and then give AI the code for debugging it. It may give something similar.
