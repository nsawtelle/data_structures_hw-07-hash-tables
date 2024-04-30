# Hashing

In this assignment you will implement a Hash Table. 

Hash tables are based on mathematical _hash functions_ that deterministically
reduce a large data object into a number called a _hash code_. These numbers can
then be used to efficiently store and look up references to the original data.

For example, you could take a large corpus of text (such as the collected works
of Shakespeare), run it through a specific hash function, which yields a single
32-bit integer. We can use that number as a _key_ in a lookup table.

There are many kinds of hash functions. For the purposes of this homework
assignment, we will use a hash function that uses the full range of possible
outputs, and will always give the same output for the same input. If you change
the input even slightly, the output will be wildly different.

An ideal hash function will transform all inputs into distinct hash codes.
However, since there are more inputs than there are outputs, it is possible that
two different inputs will yield the same output. This is called a _collision_.
Managing hash code collisions is the tricky thing about building a good hash
table, and is the main topic of this assignment.

Further, even though we can reduce a potentially huge data object into a single
32-bit number (or whatever other bit length you need), we can't reasonably have
a `2^32`-element array to perform a direct lookup. To be memory efficient, hash
tables will put hash codes into _buckets_, often using the simple modulo
operator (`%`).

```
    hash_code = hashfunc(shakespeare)
    bucket_code = hash_code % array_size
```

With this method, the hash is independent of the array size and it is then
reduced to an index (a number between `0` and `array_size − 1`) by using the
modulo operator (%).

So we have three things: (1) the original value, which could be huge; (2) a hash
code, which has a small fixed size; (3) a bucket code, which has a tiny fixed
size.

Because the number of buckets corresponds to our backing array, there will be
collisions with the bucket code when we want to put several objects in the same
place. To overcome this, we will use _linear probing_ to store values in nearby
buckets in a way that they can be found easily. Use the hash code to determine
if the item in a bucket is the thing that you're looking for.

## Video Lectures & Slides

The lectures and slides will be a good resource here.

## Approach for working the homework

The functions in the implementation file are provided in a logical order for
progressing through. Take a look at the unit test file and the `main.cpp` for
inspiration on syntax and to understand how your functions will be invoked.

In several of the functions you'll need code that identifies which hash node a
piece of data might reside. I highly recommend that you write a single function
for that and call it from your other functions.

