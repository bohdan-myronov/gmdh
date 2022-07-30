#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
class csv_reader{
public:
	vector<vector<float>> read_file(string path, int matrix_column_size)
	{	
		if (matrix_column_size < 1)
		{
			cout << "input error\n";
			throw;
		}
		
		vector<vector<float>> content;
		vector<float> row;
		string line, word;
		fstream file(path, ios::in);
		if(file.is_open())
		{	
		int row_counter = -1;
		int column_counter = 0;
			while(getline(file,line))
			{
				row.clear();
				row_counter++;
				column_counter = 0;
				stringstream str(line);
				while(getline(str,word,',') && column_counter < matrix_column_size){
					column_counter++;
					
					row.push_back(std::stod(word));
					}
				content.push_back(row);

			}
		}
		else
		{
			cout << " Error reading " << path<< "\n";
			throw;
		}
		
		int rows = content.size();
		int columns = content[0].size();
		
		if (rows !=3)
		{
			cout << path <<" must be 3 rows of length\n";
			throw;
		}
		for (int i =1;i<content.size();i++){
			if (content[i].size() != columns)
			{
				cout << "Columns of the " << path << " are not the same size. \n";
				throw;
			}
		}
		
		
		return content;
	}
	vector<vector<float>> fill_X_Matrix(vector<vector<float>> content)
	{
		vector<vector<float>> X;
		vector<float> entry;
		for (int i = 0;i<content[0].size();i++)
		{
			entry.clear();
			entry.push_back(1); //k
		
			entry.push_back(content[0][i]);//x1
			entry.push_back(content[1][i]);//x2
			entry.push_back(content[0][i]*content[0][i]); //x1^2
			
			X.push_back(entry);
		}

		return X;
	}
	vector<vector<float>> fill_Y_Matrix(vector<vector<float>> content)
	{
		vector<vector<float>> Y;
		vector<float> entry;
		for (int i = 0;i<content[0].size();i++)
		{
			entry.clear();
			entry.push_back(content[2][i]);
			Y.push_back(entry);
		}

		return Y;
	}
	vector<vector<vector<float>>> divide_content_to_A_B(vector<vector<float>> content)
	{
		vector<vector<vector<float>>> AB;
		vector<vector<float>> A;
		vector<vector<float>> B;
		vector<float> entry;
		A.push_back(entry);
		A.push_back(entry);
		A.push_back(entry);
		
		B.push_back(entry);
		B.push_back(entry);
		B.push_back(entry);
		

		for (int i = 0; i < content[0].size();i++){
			
			if ((i+1)%3 == 0){
				B[0].push_back(content[0][i]);
				B[1].push_back(content[1][i]);
				B[2].push_back(content[2][i]);
			}
			else {
				A[0].push_back(content[0][i]);
				A[1].push_back(content[1][i]);
				A[2].push_back(content[2][i]);
			}
		}
		AB.push_back(A);
		AB.push_back(B);
	
		return AB;
	}

	
	void test(vector<vector<float>> content)
	{	
		vector<vector<vector<float>>> AB = divide_content_to_A_B(content);
		cout << "A\n";
		print(AB[0]);
		cout << "B\n";
		print(AB[1]);
	}
	void print(vector<vector<float>> matrix)
	{
		for (int i =0;i<matrix.size();i++){
			for (int j =0;j<matrix[i].size();j++){
				cout<<matrix[i][j]<< " ";
			}
			cout<<"\n";
		}	
	}
};
