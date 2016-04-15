#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stddef.h>
#include <cstdlib>


int main(int argc, char* argv[])
{
	if(argc<2){
		std::cout<<"Enter the filename"<<std::endl;
		return -1;
	}

	int fd_c;

	struct sockaddr_un addr_s;
	socklen_t addr_s_len;


	// ***   create socket   ***

	fd_c = socket( AF_UNIX, SOCK_STREAM, 0 );
	if( fd_c < 0 ){
		std::cerr << "Error in socket()" << std::endl;
		return -1;
	}


	// ***   establish connection   ***
	char address[30];
	strcpy(address, "./");
	strcat(address, argv[1]);
	
	addr_s.sun_family = AF_UNIX;
	strcpy( addr_s.sun_path, address);
	addr_s_len = sizeof( addr_s );
	if( connect( fd_c, ( struct sockaddr* )&addr_s, addr_s_len )  <  0 ) {
		std::cerr << "Error in connect()" << std::endl;
		return -1;
	}
	char me[1];
	read(fd_c, me,1);
	std::cout<<"You are player: " <<me<<std::endl;
	// ***   write message   ***
	
	bool playing=true;
	char board[10];
	while(true){
		read(fd_c, board,10);
		//std::cout<<"Before move "<<board<<std::endl;
		
		if(board[0]=='W'){
			std::cout<<"You Won!"<<std::endl;
			break;
			}
		else if(board[0]=='L'){
			std::cout<<"You Lost :("<<std::endl;
			break;
		}
		else if(board[0]=='C'){
			std::cout<<"Cats Game..."<<std::endl;
			break;
		}

		char token = board[9];
		int rand_pos;
		std::cout<<board[0]<<'|'<<board[1]<<'|'<<board[2]<<'\n'<<
			board[3]<<'|'<<board[4]<<'|'<<board[5]<<'\n'<<
			board[6]<<'|'<<board[7]<<'|'<<board[8]<<std::endl;
		while (true){
			int input;
			std::cout<<"Enter your move "<<std::endl;
			std::cin>>input;
			if(board[input]=='X' || board[input]=='O'){
				std::cout<<"That place is taken. Try again"<<std::endl;
				continue;
			}
			
			board[input]=token;
			break;
		}
		
		write(fd_c,board,10);
		
	}
  // ***   close socket   ***

  close( fd_c );

  return 0;
}

















