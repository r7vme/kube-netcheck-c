/* App starts TCP listener and then attempts
   to establish TCP connection to Kubernetes
   service (backed by this app). After successful
   connection it sleeps for 10 seconds and then
   attempts again. App will exit(1) on any error. */
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <unistd.h>

const char *HOSTNAME = "kube-netcheck";
const unsigned int PORT = 6666;
const unsigned int CHECK_INTERVAL = 10;
const unsigned int WARMUP_INTERVAL = 30;

int main()
{
    int l_sockfd;
    struct sockaddr_in l_addr;

    // Server part.
    l_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (l_sockfd == -1)
    {
        perror("server socket");
        exit(1);
    }

    memset(&l_addr, 0, sizeof(l_addr));

    l_addr.sin_family = AF_INET;
    l_addr.sin_addr.s_addr = INADDR_ANY;
    l_addr.sin_port = htons(PORT);

    if (bind(l_sockfd, (struct sockaddr *) &l_addr,
             sizeof(l_addr)) == -1)
    {
        perror("server bind");
        exit(1);
    }

    if (listen(l_sockfd, 128) == -1)
    {
        perror("server listen");
        exit(1);
    }

    // Let Kubernetes start all pods.
    printf("warming up for %d seconds\n", WARMUP_INTERVAL);
    sleep(WARMUP_INTERVAL);

    // Client part.
    while (1) {
        int r_sockfd;
        struct hostent *he;
        struct sockaddr_in r_addr;

        // Get k8s service IP address.
        // TODO: replace by getnameinfo.
        he = gethostbyname(HOSTNAME);
        if (he == NULL)
        {
            herror("gethostbyname");
            exit(1);
        }

        r_sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (r_sockfd == -1)
        {
            perror("client socket");
            exit(1);
        }

        memset(&r_addr, 0, sizeof(r_addr));

        r_addr.sin_family = AF_INET;
        r_addr.sin_addr = *((struct in_addr *) he->h_addr);
        r_addr.sin_port = htons(PORT);

        if (connect(r_sockfd, (struct sockaddr *) &r_addr, \
                                        sizeof(r_addr)) == -1)
        {
            perror("client connect");
            exit(1);
        }

        printf("successfully connected\n");
        sleep(CHECK_INTERVAL);
    }

    return 0;
}
