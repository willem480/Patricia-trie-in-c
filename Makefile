


dict2: main.o Record.o HashTable.o Dictionary.o Statistics.o Trie.o TrieNode.o
	gcc -g -o dict2 main.o Record.o HashTable.o Dictionary.o Statistics.o Trie.o TrieNode.o -lm

main.o: main.c Dictionary.h Trie.h
	gcc -Wall -o main.o main.c -c

Dictionary.o: Dictionary.c Dictionary.h HashTable.h
	gcc -Wall -o Dictionary.o Dictionary.c -c

HashTable.o: HashTable.c HashTable.h Record.h Statistics.h
	gcc -Wall -o HashTable.o HashTable.c -c

Record.o: Record.c Record.h
	gcc -Wall -o Record.o Record.c -c

Statistics.o: Statistics.c Statistics.h
	gcc -Wall -o Statistics.o Statistics.c -c

Trie.o: Trie.c Trie.h TrieNode.h
	gcc -Wall -o Trie.o Trie.c -c

TrieNode.o: TrieNode.c TrieNode.h Record.h
	gcc -Wall -o TrieNode.o TrieNode.c -c

# the following target is conventional, 
#   for removing the product of previous uses of this Makefile
clean:
	rm -f *.o dict2