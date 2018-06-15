#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <arpa/inet.h>
#include <error.h>
#include <fcntl.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
 
#define FAIL    -1

//compling with gcc -Wall -o client  Client.c -L/usr/lib -lssl -lcrypto
//run with ./client <ip> <port> 

//Attemt to connect to the server
//const char *hostname is the ip address of the server
//int port Port number 
int OpenConnection(const char *hostname, int port)
{   int sd;
    struct hostent *host;
    struct sockaddr_in addr;
	
    host = gethostbyname(hostname); 
    if (host == NULL )
    {
        perror(hostname);
        abort();
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);
    
    //if connection fails
    if ( connect(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0 )
    {
	//printf( "Connection Failed \n" );
	//clost the connection
        close(sd);
	//print error message for the host #
        perror(hostname);
	//Exit program
        abort();
    }
    return sd;
}
 

//Load OpenSSL and create a context
SSL_CTX* InitCTX(void)
{   
    SSL_METHOD *method;
    SSL_CTX *ctx;
 
    //Load Open SSL
    OpenSSL_add_all_algorithms();  
    SSL_load_error_strings();   
    //new client method
    method = TLSv1_2_client_method();  
    //new context
    ctx = SSL_CTX_new(method);   

    //Check to make sure it was created
    if ( ctx == NULL )
    {
        ERR_print_errors_fp(stderr);
        abort();
    }
    return ctx;
}
 

//Request a Certificate from the Server
void ShowServerCerts(SSL* ssl)
{   
    X509 *cert;

    //Grab the certificate 
    cert = SSL_get_peer_certificate(ssl); 
    //If theres a certificate print it
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
	printf("Subject: %s\n", X509_NAME_oneline(X509_get_subject_name(cert),0,0) );   
        printf("Issuer: %s\n", X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0));
	//Free the space the certificate was in
        X509_free(cert);     
    }
    else
        printf("No Server Certificates\n");
}


//Trying to connect if we do not know the port but know the host name -- Not Working
/*
unsigned short getPort() {
	unsigned long temp;
	char port[6];		//065,535
	int fd = open( "location", O_RDWR );

	//error opening file
	if ( fd < 0 ) {
		printf( "File 'location' not found. Terminating.\n" );
		exit(1);
	}

	//read in port
	read( fd, port, 5 );

	//convert string to ushort
	temp = strtoul( port, NULL, 10 );
	if ( errno == ERANGE || temp < 0 || temp > UINT_MAX ) {
		printf( "File 'location' has a port not within 0-65535.\n" );
		exit(1);
	}
	else if ( temp == 0 && strcmp(port,"0")!=0 ) {
		printf( "File 'location' has a port not within 0-65535.\n" );
		exit(1);
	}
	return ntohs(temp);
}
*/

//Server set up currently to port 8080
int main(int count, char *strings[])
{   SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char *hostname, *portnum;
 
    if ( count != 3 )
    {
        printf("usage: %s <host> <port>\n", strings[0]);
        exit(0);
    }
    SSL_library_init();
    hostname=strings[1];
    portnum=strings[2];
 
    //Attempt to connect to server
    server = OpenConnection(hostname, atoi(portnum));


    //Create a SSL Connection
    ctx = InitCTX();
    ssl = SSL_new(ctx);      
    SSL_set_fd(ssl, server);    
    //if it does not connect error out else print the server certificate
    if ( SSL_connect(ssl) == FAIL )
        ERR_print_errors_fp(stderr);
    else
    {  
        printf("\n\nConnected with %s encryption\n", SSL_get_cipher(ssl));
	//Print Server Certificates
        ShowServerCerts(ssl);        
	//Disconnect
        SSL_free(ssl);        
    }
    //Disconnect and Free Space
    close(server);         
    SSL_CTX_free(ctx);     
    return 0;
}
