#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stddef.h>

int main(int argc, char* argv[]){
	if(argc<2){
		std::cout<<"Enter the socket name"<<std::endl;
		return-1;
	}

	// server socket
	int fd_s;
	struct sockaddr_un addr_s;
	socklen_t addr_s_len;

	// client socket
	int fd_c1;
	struct sockaddr_un addr_c1;
	socklen_t addr_c_len1;
	
	int fd_c2;
	struct sockaddr_un addr_c2;
	socklen_t addr_c_len2;


	// ***   create socket   ***

	fd_s = socket( AF_UNIX, SOCK_STREAM, 0 );
	if( fd_s < 0 ){
		std::cerr << "Error in socket()" << std::endl;
		return -1;
	}


	// ***   set address and bind socket   ***
	char address[40];
	strcpy (address, "./");
	strcat (address, argv[1]);
	unlink(address);
	addr_s.sun_family = AF_UNIX;
	strcpy( addr_s.sun_path, address );
	addr_s_len = offsetof( struct sockaddr_un, sun_path ) + strlen( addr_s.sun_path );
	if( bind( fd_s, ( struct sockaddr* )&addr_s, addr_s_len ) < 0 ){
		std::cerr << "Error in bind()" << std::endl;
		return -1;
	}

	
	// ***   listen for connections   ***
	for(int i=0;i<2;i++){
		if( listen( fd_s, 1 ) < 0 ){
			std::cerr << "Error in listen()" << std::endl;
			return -1;
		}
		char temp[1];
		// ***   accept an incoming connection   ***
		if(i==0){
			addr_c_len1 = sizeof( struct sockaddr_un );
			fd_c1 = accept( fd_s, ( struct sockaddr* )&addr_c1, &addr_c_len1 );
			temp[0]='X';
			write(fd_c1,temp,1);
			
			if( fd_c1 < 0 ){
				std::cerr << "Error in accept() for the first client" << std::endl;
				return -1;
			}
		}	
		else{
			addr_c_len2 = sizeof( struct sockaddr_un );
			fd_c2 = accept( fd_s, ( struct sockaddr* )&addr_c2, &addr_c_len2 );
			temp[0]='O';
			write(fd_c2, temp,1);
			
			if( fd_c2 < 0 ){
				std::cerr << "Error in accept() for the second client" << std::endl;
				return -1;
			}
		}	
	}
	
	
	char board[]="012345678n";
	bool turn = true;
	bool playing=true;
	while(playing){
	
		//if it's player 1's turn
		if(turn==true){
			board[9]='X';
			write(fd_c1,board,10);
			read (fd_c1,board,10);
			turn=false;
		}
		//if it's player 2's turn
		else{
			board[9]='O';
			write(fd_c2,board,10);
			read (fd_c2,board,10);
			turn=true;
		}
		
		std::cout<<"After player "<<board[9]<<'\n'<<
			board[0]<<'|'<<board[1]<<'|'<<board[2]<<'\n'<<
			board[3]<<'|'<<board[4]<<'|'<<board[5]<<'\n'<<
			board[6]<<'|'<<board[7]<<'|'<<board[8]<<'\n'<<'\n'<<
			std::endl;
		
		
		//if player 1 wins
		if((board[0]=='X'&&board[1]=='X'&&board[2]=='X')||
			(board[3]=='X'&&board[4]=='X'&&board[5]=='X')||
			(board[6]=='X'&&board[7]=='X'&&board[8]=='X')||
			(board[0]=='X'&&board[4]=='X'&&board[8]=='X')||
			(board[2]=='X'&&board[4]=='X'&&board[6]=='X')||
			(board[0]=='X'&&board[3]=='X'&&board[6]=='X')||
			(board[7]=='X'&&board[4]=='X'&&board[1]=='X')||
			(board[8]=='X'&&board[5]=='X'&&board[2]=='X')){
				write(fd_c1,"WWWWWWWWWW",10);
				write(fd_c2,"LLLLLLLLLL",10);
				playing=false;
		}
		//if Player 2 wins
		else if((board[0]=='O'&&board[1]=='O'&&board[2]=='O')||
			(board[3]=='O'&&board[4]=='O'&&board[5]=='O')||
			(board[6]=='O'&&board[7]=='O'&&board[8]=='O')||
			(board[0]=='O'&&board[4]=='O'&&board[8]=='O')||
			(board[2]=='O'&&board[4]=='O'&&board[6]=='O')||
			(board[0]=='O'&&board[3]=='O'&&board[6]=='O')||
			(board[7]=='O'&&board[4]=='O'&&board[1]=='O')||
			(board[8]=='O'&&board[5]=='O'&&board[2]=='O')){
				write(fd_c1,"LLLLLLLLLL",10);
				write(fd_c2,"WWWWWWWWWW",10);
				playing=false;
		}
		//if no more moves are possible
		for(int i=0;i<9;i++){
			if(board[i]!='O' && board[i]!='X'){
				break;
				}
			else if(i==8){
				playing==false;
				write(fd_c1,"CCCCCCCCCC",10);
				write(fd_c2,"CCCCCCCCCC",10);
			}
		}
	}
	
	// ***   close sockets   ***
	close( fd_c1 );
	close( fd_c2 );
	close( fd_s );

	unlink( address );

	return 0;
}
