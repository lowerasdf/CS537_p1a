# CS537 P1A: Persistent Key-Value Storage

##### Table of Contents
* [Installation](#installation)
* [File System](#file-system)
* [Data Structures](#data-structures)
  * [Hash Table](#hash-table)
  * [Linked Lists](#linked-lists)

## Installation
To compile and run the program, run:
<pre><code>gcc -o kv kv.c</code></pre>
Then:
<pre><code>./kv [ARGV_1] [ARGV_2] ... </code></pre>
Available <code>[ARGV_N]</code>:
* <code>p,[key],[val]</code>: put a new or existing key-value pair
* <code>g,[key]</code>: get the value of the given key
* <code>d,[key]</code>: delete the value of the given key
* <code>c</code>: clear all key-value pairs
* <code>a</code>: get all key-value pairs

## File System
The program stores the data in a text file called <code>database.txt</code> in the same directory. To run the program, you do not have to provide the <code>database.txt</code>. It will automatically create one if it does not exist.

## Data Structures
The program makes use of a <b>hash table</b> with <b>linked lists</b> to hold the conflicting keys.

#### Hash Table
The hash function is simple: it takes the modulo of the given key by the pre-defined <code>hash-capacity</code> (1000 key-value pairs by default). With that being said, it does <b>not</b> mean that the hash table can only hold 1000 key-value pairs. The <code>hash-capacity</code> is solely used for the hash function to determine index.

#### Linked Lists
A single linked list node represents a key-value pair where each value is dynamically allocated. Each node is also dynamically allocated.
