#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<stdlib.h>
#include"fibHeap.h"
int main (int argc, char *argv[]) 
{
	if(argc!=2)					// first argument is the binary name 
	{
		cout<<"Please enter filename. Exiting.\n";
		exit(1);
	}
	ifstream inputFile(argv[1]);			// open input file
	if(!inputFile.is_open())
	{
		cout<<"Cannot open the input file. Exiting.\n";
		exit(1);
	}
	ofstream outputFile("output_file.txt");		// open output file
	if(!outputFile.is_open())
	{
		cout<<"Cannot open the output file. Exiting.\n";
		inputFile.close();
		exit(1);
	}
	
	std::map<string,node*> hashMap;
	FibHeap h;
	string temp;
	while (getline(inputFile,temp))			// read one line at a time
	{
		if(temp[0]=='#') 			// if line starts with a '#', its a hashtag
		{
			int i;
			for(i=1;;i++) 			// find position of first space character
			{
				if(temp[i]==' ')
					break;
			}
			string hashtag=temp.substr(1,i-1); // extract hashtag value from the string
			string num=temp.substr(i+1); 	// get frequency from the string 
                        const char *c=num.c_str(); 	// convert to c string for atoi
                        int frequency=atoi(c);	
			if(hashMap.find(hashtag)!=hashMap.end())	// if found
			{
				// increase the key	
				node *t=hashMap.find(hashtag)->second;
				h.increaseKey(t,frequency);		
			}
			else	// if not found
			{
				// insert a new node
				node *t=h.getNode(hashtag,frequency);
				hashMap.insert(std::pair<string,node*>(hashtag,t));
				h.insert(t);
			}
		
		}
		else if(temp[0]>='0' && temp[0]<='9') 	// if line starts with a number
		{
                        const char *c=temp.c_str();
                        int z=atoi(c);			// get the integer from the string
			node **temparr=new node*[z];	// array to store the removed nodes, to be inserted later
			int i;
			// perform z removeMax operations
			for(i=0;i<z;i++)
			{
				temparr[i]=h.removeMax();
				outputFile<<(i>0?",":"")<<temparr[i]->hashtag;
			}
			outputFile<<"\n";
			// reinsert extracted nodes
			for(i=0;i<z;i++)
				h.insert(temparr[i]);
			delete[] temparr;		// free allocated array
		}
		else if((temp[0]=='s' || temp[0]=='S') && (temp[1]=='t' || temp[1]=='T') && (temp[2]=='o' || temp[2]=='O') && (temp[3]=='p' || temp[3]=='P'))
		{
			break;
		}
	}
	inputFile.close();
	outputFile.close();
	return 0;
}

