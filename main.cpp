#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <time.h>

using namespace std;

int main(int ac, char** av){
	//Noise amount depends from these frequencies and the audio and video bitrate or image size. Higher numbers = less bit rot, except "send"

	//enable scratches
	bool scrON=true;
	//enable random noise
	bool noiON=true;

	//scratch start chance, 1 in x bytes
	int ssrt = pow(10,5);
	//scratch end chance, 	1 in x bytes
	int send = pow(10,4);
	//noise chance in scratches, 1 in x bytes
	int nocs = 128;
	
	//random noise chance,	1 in x bytes
	long int noi = 5*pow(10,4);

	//chance to change a bit in random noise or scratch 
	int nb = 64;

	//header size bytes
	int hsz = 255;

	//input file name
	char const *infi = "./original.mkv";
	//output file name
	char const *outfi = "./bitrot.mkv";

//--------
//	long int seed = time(NULL);
	long int seed = (69420+1337);
	printf("seed: %ld\n",seed);
	srand(seed);

	ifstream inputc (infi,  ios::in  | ios::binary);
    ofstream outfile(outfi, ios::out | ios::binary);

	//copy file to buffer
    vector<unsigned char> buffer(istreambuf_iterator<char>(inputc), {});


	for(int i=0;i<hsz;i++){ //skip header
		outfile << buffer[i];
	}

	bool scratch=0;
	unsigned char o;
	
	for(int i=hsz;i<buffer.size();i++){

		o=buffer[i];
		
		//stripes of noise
		if(scratch){
			scratch=(bool)(rand()%send); //end scratch
		}else{
			scratch=!(bool)(rand()%ssrt); //start scratch
		}

		//bit switch
		bool bw=scratch;

		//combine scratch random noise
		bw = !(bool)(rand()%noi)*noiON || scratch*!(bool)(rand()%nocs)*scrON;

		unsigned char mbc=0;
		mbc = rand()%nb ? mbc : mbc|((unsigned char)(128)) ;
		mbc = rand()%nb ? mbc : mbc|((unsigned char)(64)) ;
		mbc = rand()%nb ? mbc : mbc|((unsigned char)(32)) ;
		mbc = rand()%nb ? mbc : mbc|((unsigned char)(16)) ;
		mbc = rand()%nb ? mbc : mbc|((unsigned char)(8)) ;
		mbc = rand()%nb ? mbc : mbc|((unsigned char)(4)) ;
		mbc = rand()%nb ? mbc : mbc|((unsigned char)(2)) ;
		mbc = rand()%nb ? mbc : mbc|((unsigned char)(1)) ;

		//bit toggle with xor
		o= bw ? o^mbc : o;

		outfile << o; 
	}
		

}
		