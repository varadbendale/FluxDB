##buffer pool 

Starting with the I/O stuff, reading and writing on the disk. If any page is required we directly use it from there. There is also one thing which is like we always track if the code is going into I/O or not, for the mutex purposes.

Now there can be multiple people or users accessing the thing at the same time so it is quite important that we put a lock on each of the frames we are using.

The backbone function of this whole thing is getting which frame is free so we can take that one and put the page in it. We have bitmaps for checking each frame, the similar way which was used in the FSM. If a frame is dead we pick it, else we just move to the next one. Now if none of the frames are dead we need to remove one that nobody is using, that responsibility is given to the clock algorithm. We have two options, if the request is forced we just get it directly, else we wait till the optimal condition is met. The clock function is called at timed intervals and each one has mutex locks on it.

Now the clock algorithm tracks two things. The use count, how many threads are using the same frame, so if the use count is 0 we can remove it. And the clock counter, how many times that frame has been reviewed for eviction. Whichever frame has a bigger clock counter means it has been used more, so whichever is smaller we pick that one. If the type is forced we directly give the smaller one, else we decrement all clock counters by one and move on. If any hits zero we return that frame, else we just wait. When a frame is found we update the stats and if the frame is dirty we first write it back to the file before moving on. There is also a valid bit per frame, if the bit is valid only then we treat it as live, else it is already dead and we can use it directly.

The background dirty writer is like at a regular interval we keep writing all the dirty pages back to the file, so things can be a bit faster, basically an optimization.

A few of the normal functions now. Delete page, we get the input, put it in the lock, change the parameters and then write zeros to the whole page in the file. Insert page in frame, we get the page id, fetch the page from the file or cache or wherever it is and then put it in whichever frame we got from the clock. Insert the thing is the same, just the frame is updated as per the page number and we write it back to the file. Not in use get out basically just makes the frame dead, sets all parameters to dead and then the clock can remove it or reuse it later.

A few stats are tracked to understand how well the buffer is working. Hits means the page asked is already present in a frame. Misses means the page is not in any frame. Clock is how many times the clock has been invoked. Evictions means no dead frame was found so we had to remove one that was not in use. Flushes means the frame was dirty so we had to write it back to the file first.

There is also an advanced thing where if we are asking for a particular page we also tend to pre-fetch the nearby pages using an OS level read-ahead, so by the time we actually need those pages they are already in the cache. Implementation of this is still pending though.
