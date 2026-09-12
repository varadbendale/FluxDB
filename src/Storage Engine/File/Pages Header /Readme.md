Page Zero — Header

The very first page in the pages is the header, also very well known as **page zero**. It pretty much contains all the major or, in general, information which we get from the CREATE queries which we write. So yeah, we get the CREATE parser, then we need to simplify the data in another struct and then put it in the file. This is known as the header.

Struct Stored in the File

Coming to the struct which we are storing in the file, we pretty much need to store the version and a specific number for recognising that this file belongs to this database engine. Then we have the page size, as each page contains a specific fixed size. Here I took **4096 bits**. Along with this, we have a different struct for the table information. That struct contains the table name, the root page (the page number), and the number of columns. It then extends to the column information, which is stored in a different struct. This struct contains all the basic column information which we get from the parser. Not going in deep because the stuff is already stated in the parser's documentation.

Default Values

We have a few functions specifically designed to just set default values to all the information in the structs. These are `default_column_info`, `default_table_info`, and `default_pagezero`.

 Main Function

Coming to the main function now, this one pretty much calls the parser and gets the tree, and then we operate on that tree. We do take a hash for each of the columns so that multiple columns shouldn't come as it is a clear error. We make use of the function `hashnum` for getting the hash number for hashing. The normal bool functions like primary key, not null, etc. etc. are done here.

Primary Key

While we are on the primary keys, the thing was that in the CREATE parser we often tend to directly put the primary key as the root name and then define the column name directly. Now this can be an issue, as in the header we just want to know whether the column is a primary key or not directly.

So we pretty much have a function called `take_care_of_stuff`, which looks at all the names, and if it finds the primary key as a root name, then we have the hash for the primary keys as well. When we move our cursor, we will look at the stuff and then directly turn the bool primary key on, more better.

Default and Unique

Also, if the default value is present, then the unique must not be present, else it is a problem. That condition is applied to the thing.

Foreign Key

Then, if the foreign key comes into play, we pretty much need three things: the name in the header for the column, the reference table, and the reference column. Now, the parser just gives normal stuff like normal query things. We need to get it simplified as well. That work is done by `do_the_foreign_key`.

Writing the Page

Once the struct is filled, this page is written in the file by the **page manager**.
