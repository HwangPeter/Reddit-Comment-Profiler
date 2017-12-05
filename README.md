# Reddit-Comment-Profiler
Required Files: main.py, profiler.cpp, clean.h, vectorize.h
Requires C++11 and Python

To run: 
1. Enter the username of the redditor you want to input on line 13 of main.py. Ex. user = reddit.redditor('username')
2. In profiler.cpp, inside int main() enter the file path of the where the Python script will output Comments.txt. By default this should be whereever main.py is stored.
Ex. std::ifstream ifs ("/Users/Peter/Documents/Python/CommentCrawler/Comments.txt"); 
3. Do the same but with the file path where you want Cleaned.txt to output.
Ex. std::ofstream clnFile ("/Users/Peter/Documents/Python/CommentCrawler/Cleaned.txt");
4. Further down in main() there is another line starting with ifs.open. Enter the SAME file path as for step 3.
5. In vectorize.h, inside the vectorize function, insert the filepath to your stopwords.txt. 
Ex. std::ifstream ifsstop("/Users/Peter/Documents/Python/CommentCrawler/stopwords.txt");
6. Run main.py first, then after it has finished grabbing comments and writing to Comments.txt, run profiler.cpp.
