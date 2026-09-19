# The Pager

This piece of code is merely responsible for getting the data which we have in the page. The page is a data structure made for storing all the data blobs which we have.

Before anything, let’s just go and look at what exactly this structure carries. So first, the structure is like the slots, header, and the normal data. The slots have three things: offset, size, and primary key number. Now these kind of come in clutch when we need to remove some of the data and then replace it with something, put the data, and just update the slot. Easy as that.

Coming to the header, we have the page_num, which is unique for each page. The page_type is intended to keep track of whether the page is used for normal purposes, hashing, or a TOAST table. Then we have current_offset and current_size. These are just for keeping track of the most recent insertion from the very back, so yeah, pretty much used for the temporary stuff. Then we have the free_size, which represents how much free size the entire page has, and the normal_free_size, which represents from the back of the last data to the end how much size is available. It represents the continuous free space available there. And then we have the dead_slots. These are the binary numbers for each and every slot in the page which basically look for if the slot is dead or not. I have used bit manipulation for this in its extent, so yeah.

I am heavily inspired by the PostgreSQL storage engine for this version. The very first thing coming is the FSM. It is like wherever there is space to insert data, we insert it and manage all the space and put the data in it. That is the basic idea.

get_page_from_file is used to get the page from the file. Suppose we have a page in which already something is present and we need that one thing, this function helps us to get it. Same is the get_pagezero purpose: just read the header from the file and then use it.

find_the_size_of_stuff_get_the_data_and_get_the_primary_key_num — the bigger the name, the bigger its purpose is. We just have the data coming from the insert parser, but we do need to get the size of the entire data. Then the data is stored in the format of the blob, the data type, and then the data, and if the string is there, the size as well. Now we do need the primary keys as well if we need to sort it, which we of course do, so we get it from here as well. The header is passed, the data we got is passed, and then all these three essential things are gotten.

Now coming on the main function, if there is enough space for the data to be in the page and also there is enough space to accommodate the data from the back, meaning there is no need to reshuffle anything, we just put the data directly in it with the updation of the slots.

If the page has enough size but not the space from behind, we first check all the dead slots which are there from the bit manipulation from the struct which I talked about earlier. Each and every piece of data is picked and checked if it can accommodate the data, and then we move forward. If we found one, well and good, just put it in and update the sizes, as there can be some space left in the earlier slot if it was bigger and the new one is smaller, so that way.

And even if that thing doesn't work, we need to rearrange everything. All the small, small spaces which are there and all the small dead slots which are of no use, we need to reshuffle all and then get the thing. We pretty much keep the pointer if the slot is not dead, keep the pointer to the tail of it, and move forward. Even if the new slots are dead or some space was left, we can just get it at the end so that we can end these internal fragmentations.

So pretty much we do the memmove tool and shift all the data that way, and after that we just add the data which we had in the page. Simple as that.
