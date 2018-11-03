# index-builder

The objective of this project is to index million of web pages and then be able to do a text search in those documents. A "search-engine" of sorts. I have divided the project into primarily 2 major parts. Part 1 is to parse through the millions of documents and build a useful index. Part 2 focuses on effectively using this index to search the documents, rank the possible matches and show the best match.

Project Status : On-going
Part 1: complete
Part 2:On-going

# Part 1

This project uses the Common Crawl dataset (commoncrawl.org). Common crawl is an organization that makes large web crawls available to the public. About 50 wet files equate to 1.5 million web-pages. Given this dataset, we are going to parse through the files to create the following structures
URL table -> contains a mapping from docID to URL 
Index --> contains which document each word was found in. It is of the form apple --> 0,2,3,...n This file is wirtten in binary and does not contains the word "apple", just the docID lists.
IndexFrequency --> contains the frequency count mapping for the above Index. It is of the form apple -->1,5,6..n. This file is written in binary and does not contain the word "apple" just the frequencies as a list.
Lexicon --> The lexicon contains the mapping for word --> the start, end byte-offset for the word in Index and IndexFrequency.

generatePostings()
This function Is responsible for converting all the wet-files to postings files. I have created my own parser for parsing wet files and creating postings. It creates one posting file per wet file. The format of the posting file is word docID \n. This posting file per wet file is in sorted order. In conclusion, it takes a 300 MB wet file, strips it of punctuation, non-english letters and creates a sorted posting of the form word docID. It also ads the URL and docID’s to a URL map.
 
How are we doing this?
We can provide any length to the char array buffer and the program handles the rest. Let us say we provided length of buffer as 9000 characters. We then load the first 9000 characters and look for a url. Once we find the url we add it to the url map and assign a docID. We then go and find all English words till the next url and create word, DocID postings. These postings are then sorted using C’s inbuilt sort and written to a postings file. We then go and do the same thing for the next wet file.

writeURLMaptoDisk()
Since we might run the program again without doing generate postings, we have persisted the URLTable to disk. This function does exactly that. It takes each key (dociD)→value (docID) pair and writes it to disk. The URL map is written in ASCII.

loadURLMapFromDisk()
This function is the inverse operation of writeURLMaptoDisk(), for the times when we run the index_builder without calling generate_postings() it takes the URLTable stored on disk and loads it into an unordered_map to be used.

system(mergeCommand.c_str())
This is a system command that is used to merge the posting files. It takes all the posting files and merges them into one large posting file. Since the posting files are already sorted, to create the large sorted postings we do not sort, we just merge all the files. So we end up merging 50 200-300 MB files into one large 14-15 GB merged large posting file.
The command we used to merge the files is:
sort --merge -k1,1 -k2n,2 ../posting_files/postings* -o ../unix_sorted_postings/sorted_posting

writeLexiconToDisk(lexicon)
persists the lexicon to disk

createInvertedIndex()
This critical function is responsible for creating the index, indexFrequencies and Lexicon

flags.h → defines flags for execution Eg: writeBinary (if true write in binary, if false write in ASCII)
helper.cpp/helper.h → contains the functions for creating index, lexicon from merged posting file
main.cpp → function calls to different actions as mentioned above 
utils.cpp/utils.h → contain functions for parsing wet file and creating posting files

# Part 2 

In-Progress
