/*
 * Vectorize.h
 *
 *  Created on: Nov 3, 2017
 *      Author: Peter
 */

#ifndef VECTORIZE_H_
#define VECTORIZE_H_
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_set>

#define max_cmnts 1000
struct sub{
	std::string subr = "";
	int cmt_count{0};
	int weighted_score{0};
};
struct word{
	std::string s;
	sub sub;
	int freq{0};
	int karma{0};
};

bool sort_comp(word &w1, word &w2)
{
	return w1.freq > w2.freq;
}

void sort_matrix(std::vector<std::vector<word>> &matrix)
{
	for(int j=0; j<matrix.size(); j++){
		for(int k=0; k<matrix[j].size(); k++){
			std::sort(matrix[j].begin()+1, matrix[j].end(), sort_comp);
		}
	}
}

void export_to_excel(std::vector<std::vector<word>>& matrix)
{
	std::ofstream out("test.csv");
	for(int i=0; i<matrix[0].size(); i++)
	{
		out << matrix[0][i].s << "=" << matrix[0][i].freq << ',';
	}
	out << '\n';
	for(int j=1; j<matrix.size(); j++)
	{
		for(int k=0; k<matrix[j].size(); k++)
		{
			if(k == 0)
				out << matrix[j][k].s << " sub = " << matrix[j][k].sub.subr << " karma = " << matrix[j][k].karma << ',';
			else
			{
				if(matrix[j][k].freq > 0)
					out << matrix[j][k].s << "=" << matrix[j][k].freq  << ',';
			}
		}
		out << '\n';
	}
}

void print_matrix(std::vector<std::vector<word>> &matrix)
{
	for (int i = 0; i < matrix.size(); i++)
	{
		for (int j = 0; j < matrix[i].size(); j++)
		{
			std::cout << "i is:" << i << "  j is:" << j
					<< "       ";
			std::cout << matrix[i][j].s << std::endl;
		}
	}
}

std::vector<std::vector<word>> vectorize(std::ifstream &ifs){
	std::vector<std::vector<word>> matrix; //
	//Rows represent a comment, columns represent topics
	std::vector<word> row, topics(1);
	matrix.push_back(topics);
	int row_index{0};
	std::string s;
	std::string stopwrd;
	std::unordered_set<std::string> stopwords;
	std::ifstream ifsstop("/Users/Peter/Documents/Python/CommentCrawler/stopwords.txt");
	if(ifsstop.is_open())
		std::cout << "stopwords.txt opened." << std::endl;
	else
		std::cout << "File not found." << std::endl;
	while(getline(ifsstop, stopwrd))
	{
		stopwords.insert(stopwrd);
	}
	ifsstop.close();

	while(!ifs.eof())
	{
		std::getline(ifs, s);
		std::stringstream ss(s);
		while(std::getline(ss, s, ' '))
		{
			if(s.find("|||")!= std::string::npos) //new comment/end of previous comment
			{
				row_index++;
				if(row_index == 1)
				{
					word temp;
					std::string num;
					temp.s = "Comment 1";
					row.push_back(temp);
				}
				if(row_index != 1) // not the first comment
				{
					matrix.push_back(row);
					row.clear();
					word temp;
					temp.s = s;
					temp.freq = 0;
					row.resize(topics.size(), temp);
				}
				std::string label = "Comment ";
				label.append(std::to_string(row_index));
				row[0].s = label;
				std::string num;
				getline(ifs, row[0].sub.subr);
				getline(ifs, num);
				row[0].karma = std::stoi(num);

			}
			else // not a new comment
			{
				auto search = stopwords.find(s);
				if(search != stopwords.end())
					s.clear();

				if(s.empty()){} // a check for if s is null
				else
				{
					bool found = false;
					for(int i=0; i<topics.size(); i++)
					{
						if(s == topics[i].s && found == false)
						{
							topics[i].freq++;
							row[i].freq++;
							found = true;
						}
					}
					if(found == false)
					{
						word temp;
						temp.s = s;
						temp.freq = 1;
						topics.push_back(temp);
						row.resize(topics.size(), temp);
					}
				}
			}
		}
	}
//	if(row_index != 1) //pushes last comment into matrix
//	{
		matrix.push_back(row);
		row.clear();
	//}
	for(int i=1; i<=row_index; i++)
	{
		matrix[i].resize(topics.size());
		for(int j=1; j<topics.size(); j++)
		{
			matrix[i][j].s = topics[j].s;
		}
	}
	matrix[0] = topics;
	//sort_matrix(matrix);
	//print_matrix(matrix);
	export_to_excel(matrix);
	return matrix;
}




#endif /* VECTORIZE_H_ */
