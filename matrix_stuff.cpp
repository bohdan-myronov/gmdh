#include <vector>
#include<iostream>

using namespace std;
class matrix_stuff
{
public:
	vector<vector<float>> multiply(vector<vector<float>> A, vector<vector<float>> B){
		if (A[0].size() != B.size()){
			cout << "Matrices "
			<<A.size()<<"x"<< A[0].size()
			<<" and "
			<<B.size()<<"x"<<B[0].size()
			<<" are not multiplicable\n";
			 throw;
		}
		int n = A.size();
		int m = A[0].size();
		int p = B[0].size();

		vector<vector<float>> C;

		for (int i = 0; i < n;i++){
			vector<float> entry(p,0);
			C.push_back(entry);
			for (int j = 0; j < p;j++){
				for (int k = 0; k < m; k++){
					C[i][j] += A[i][k]*B[k][j];
				}
			}
		}
		return C;
	}
	vector<vector<float>> invert(vector<vector<float>> content)
	{
		// augmented matrix
		vector<vector<float>> augmented;
		vector<vector<float>> content_aug = content;
		int size = content.size();
		for (int i = 0; i < size;i++){
			vector<float> entry(size,0);
			augmented.push_back(entry);
			augmented[i][i] = 1;
		}
		for (int i = 0; i < size; i++){
			for (int j =0; j < size; j++){
				content_aug[i].push_back(augmented[i][j]);
			}
		}
		
		//content_aug.emplace(content_aug.begin(),content_aug.back());
		//content_aug.erase(content_aug.end());
		

		
		
		float temp;
		for (int i = 0; i < size;i++){
			for (int j = 0; j < size; j++){
				if (j != i){
					temp = content_aug[j][i]/content_aug[i][i];
					for (int k =0; k < 2*size;k++){
						content_aug[j][k] -= content_aug[i][k] * temp;					
					}
				}	
			}
		}
		for (int i =0; i< size; i++){
			temp = content_aug[i][i];
			for (int j = 0; j<2*size;j++){
				content_aug[i][j] = content_aug[i][j] /temp;
			}
		}

		for (int i = 0;i<size; i++){
			for (int j = 0; j < size; j++){
				content_aug[i].erase(content_aug[i].begin());
			}		
		}
		
		return content_aug;
	}
	vector<vector<float>> transpose(vector<vector<float>> source)
	{
		int n = source.size();
		int m = source[0].size();
		
		vector<vector<float>> transposed;
		vector<float> entry;
		
		for (int i = 0;i < m; i++){
			transposed.push_back(entry);
			for (int j = 0; j< n;j++){
				transposed[i].push_back(source[j][i]);
			}
		}
		return transposed;
		
	}
	
};
