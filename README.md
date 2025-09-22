# Patricia-trie-in-c
this repository contains an implementation of the Patricia trie dictioanry in c, there are some test dataset and test input files for you to test with
The input dataset has the following assumption:
1. every record in the dataset has the same number of fields that are seperated by comma
2. the content of each field should not contains any commas and comma is only used to seperate two different fields

The default behaviour of this program is to:
0. take command line arguments:
The first argument will be the stage, for this part, the value will always be 2 (for Patricia tree lookup and comparison counting).
The second argument will be the name of the input CSV data file. 
The third argument will be the name of the output file.
Example call: ./dict2 2 tests/dataset_1067.csv output.txt < tests/test1067.in > output.stdout.out

1. read the input file and store it into a patricia trie (suffix trie) 

2. answer queries about a look up for some keys in a feild, note you can query as many keys as you like for the one field that you specify and the keys you query doesn't have to be a valid key. If an invalid key is given, the program will find records associates with the key that has the least edit distance with your query

Some commnon features:
1. this program comtains both a patricia trie and a hashtable implementation, you need to uncomment the insert and search lines for the hashtable to use it in comparison with the patricia trie
2. to change the index of the field to are querying (default is index 0), change the value at line 26 where an index of 0 corresponds to the first field in a record etc.
3. the output of the program is quite self explanatory and contains the comparison counts for each of your query: b16 n16 s16 means this query takes 16 bit comparison, 16 node comparison and 16 string comparison

Contact for bugfix: hyc018018@163.com
