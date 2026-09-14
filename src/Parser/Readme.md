## Parser

So basically we get queries right, just plain text, alphabets, nothing special. and we need to make sense of it somehow so what we do is we first tokenise all of it, break it down into actual words and symbols that mean something, and then we take all those tokens and build a tree out of them, an AST, which just makes it way easier to actually run the query later on.

right now there are four things going on here —

**Tokenizer** — this runs first, always. takes the raw string and just chops it up into pieces the rest of the parser can understand. nothing else works without this.

**Select Parser** — all the select queries land here. what columns, which table, what conditions, all of that gets figured out and put into the tree properly.

**Create Table Parser** — this is where you define a table, the columns, their types, how many bits, all of it. basically the whole structure of the table lives here as AST nodes.

**Insert Parser** — once the table is defined you gotta put data in it right, so this takes whatever you're inserting and builds that into the tree too.

so yeah that's pretty much it, raw query string goes in, a clean tree comes out the other end.
