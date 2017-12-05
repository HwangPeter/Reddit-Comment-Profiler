#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include "clean.h"
#include "vectorize.h"

std::vector<word> find_top(std::vector<std::vector<word>> &matrix);
bool cmp_count(word &w1, word &w2);
void print_top5_subs(std::vector<word> &top);
void print_top10_topics(std::vector<std::vector<word>> &matrix);
void exec_python();
void write_script(std::string user);
//int python_script(); NOT WORKING

int main(){
	//std::string username = "poisonideanewcults";
	//write_script(username);
	//exec_python();
	std::ifstream ifs ("/Users/Peter/Documents/Python/CommentCrawler/Comments.txt");
	std::cout << "Opening Comments.txt" << std::endl;
	std::ofstream clnFile ("/Users/Peter/Documents/Python/CommentCrawler/Cleaned.txt");
	if(!clnFile.is_open())
		std::cout << "File not found." << std::endl;
	clean(ifs, clnFile);
	ifs.close();
	clnFile.close();
	ifs.open("/Users/Peter/Documents/Python/CommentCrawler/Cleaned.txt");
	std::vector<std::vector<word>> matrix;
	matrix = vectorize(ifs);
	if(matrix[0][1].freq == 0)
		{
			std::cout << "User has not posted any comments." << std::endl;
			return 0;
		}
	std::vector<word> top = find_top(matrix);
	print_top5_subs(top);
	print_top10_topics(matrix);
}

std::vector<word> find_top(std::vector<std::vector<word>> &matrix){
	std::vector<word> top;
	bool found = false;
	word first;
	if(matrix.size() <= 1)
		return top;
	first.sub = matrix[1][0].sub;
	//first.karma = matrix[1][0].karma;
	top.push_back(first);
	for(int i=1; i<matrix.size(); i++)
	{
		found = false;
		for(int j=0; j<top.size(); j++)
		{
			if(matrix[i][0].sub.subr == top[j].sub.subr && found == false)
			{
				top[j].karma += matrix[i][0].karma;
				top[j].sub.cmt_count++;
				if(matrix[i][0].karma >= 5)
					top[j].sub.weighted_score += 2;
				else if(matrix[i][0].karma > 1 && matrix[i][0].karma < 5)
					top[j].sub.weighted_score++;
				else if(matrix[i][0].karma == 1){}
				else
				{
					top[j].sub.weighted_score -= 2;
				}
				found = true;
			}
		}
		if(found == false)
		{

			word temp;
			temp.sub.subr = matrix[i][0].sub.subr;
			temp.karma = matrix[i][0].karma;
			temp.sub.cmt_count = 1;
			if(matrix[i][0].karma >= 5)
				temp.sub.weighted_score += 2;
			else if(matrix[i][0].karma >= 1 && matrix[i][0].karma < 5)
				temp.sub.weighted_score++;
			else if(matrix[i][0].karma == 1){}
			else
			{
				temp.sub.weighted_score -= 2;
			}
			top.push_back(temp);
		}
	}
	return top;
}

bool cmp_count(word &w1, word &w2)
{
	return w1.sub.cmt_count > w2.sub.cmt_count;
}

void print_top5_subs(std::vector<word> &top)
{
	std::sort(top.begin(), top.end(), cmp_count);
	std::cout << "Top 5 subreddits:\n";
	for(int i=0; i<top.size() && i<5; i++)
	{
		if(top[i].sub.subr.empty())
			break;
		std::cout << i+1 << ". Subreddit:" << top[i].sub.subr << "\nKarma: " << top[i].karma;
		std::cout << "      # of comments: " << top[i].sub.cmt_count;
		std::cout << std::endl;
	}
	const int sub_count{14};
	std::string warning_subs[sub_count];
	warning_subs[0] = "the donald";
	warning_subs[1] = "theredpill";
	warning_subs[2] = "pussypassdenied";
	warning_subs[3] = "uncensorednews";
	warning_subs[4] = "sjwhate";
	warning_subs[5] = "mgtow";
	warning_subs[6] = "pussypass";
	warning_subs[7] = "redpillwomen";
	warning_subs[8] = "whiterights";
	warning_subs[9] = "darkenlightenment";
	warning_subs[10] = "thiscrazybitch";
	warning_subs[11] = "new right";
	warning_subs[12] = "feministpassdenied";
	warning_subs[13] = "conspiracy";
	for(int i=0; i<top.size(); i++)
	{
		for(int j=0; j<sub_count; j++)
		{
			if(top[i].sub.subr == warning_subs[j])
			{
				std::cout << "Note: this user has commented in /r/" << warning_subs[j] << " " << top[i].sub.cmt_count << " time(s) with " << top[i].karma << " net karma and a weighted score of " << top[i].sub.weighted_score << "." << std::endl;
				std::cout << std::endl; //TODO
			}
		}
	}
}

void print_top10_topics(std::vector<std::vector<word>> &matrix)
{
	std::sort(matrix[0].begin()+1, matrix[0].end(), sort_comp);
	std::cout << "Top 10 'topics':\n";
	for(int i=1; i<matrix[0].size() && i<=10; i++)
	{
		if(matrix[0][i].s.empty())
			break;
		std::cout << i << ". " << matrix[0][i].s << "   Count: " << matrix[0][i].freq << std::endl;
	}
}

void exec_python()
{
	string filepath = "/Users/Peter/Documents/Python/CommentCrawler/main.command";
	string command = "open " + filepath;
	system(command.c_str());
	bool processIsRunning = system("ps -Ac | grep 'Python' > /dev/null") == 0;
	do {
		processIsRunning = system("ps -Ac | grep 'Python' > /dev/null") == 0;
	}while(!processIsRunning);
	std::cout << "Grabbing comments." << std::endl;
	while(processIsRunning)
	{
		processIsRunning = system("ps -Ac | grep 'Python' > /dev/null") == 0;
	}
}

void write_script(std::string user)
{
	std::ofstream python_script("/Users/Peter/Documents/Python/CommentCrawler/main.command");
	python_script << "#!/usr/local/bin/python3\n"
			<< "import praw\n"
			<< "import os\n\n"

			<< "reddit = praw.Reddit(client_id='8pTH5EvuT6BREQ',\n"
			<< "	client_secret='BQSAlS3ei8GMhfL04su4RfCQrSs',\n"
			<< "	password='CMPE130bot',\n"
			<< "	user_agent='testscript by /u/TinyPooter',\n"
			<< "	username='TinyPooter')\n\n"

			<< "path = \"/Users/Peter/Documents/Python/CommentCrawler/\"\n"
			<< "fullpath = os.path.join(path, 'Comments.txt')\n"
			<< "f = open(fullpath,'w')\n\n"

			<< "user = reddit.redditor('" << user << "')\n\n"

			<< "for comment in user.comments.new(limit=None):\n"
			<< "	print(\"|||\", file = f)\n";
	python_script << "	print(comment.subreddit, file = f)\n"
			<< "	print(comment.score, file = f)\n"
			<< "	print(comment.body, file = f)\n"

			<< "f.close()\n";

	python_script.close();
}

//int python_script()
//{
////  Py_SetProgramName(argv[0]);  /* optional but recommended */
//  Py_Initialize();
//  FILE* PythonScriptFile = fopen("/Users/Peter/Documents/Python/CommentCrawler/main.py", "r");
//  if(PythonScriptFile)
//  {
//      PyRun_SimpleFile(PythonScriptFile, "/Users/Peter/Documents/Python/CommentCrawler/main.py");
//      fclose(PythonScriptFile);
//  }
//  Py_Finalize();
//  return 0;
//}
