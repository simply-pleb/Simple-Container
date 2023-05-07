// https://devarea.com/simple-container-app-with-namespaces/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define STACK_SIZE (1024 * 1024)

static char stack[STACK_SIZE];

int setip(char *name,char *addr,char *netmask) {
  struct ifreq ifr;
  int fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_IP);

  strncpy(ifr.ifr_name, name, IFNAMSIZ);
  
  ifr.ifr_addr.sa_family = AF_INET;
  inet_pton(AF_INET, addr, ifr.ifr_addr.sa_data + 2);
  ioctl(fd, SIOCSIFADDR, &ifr);

  inet_pton(AF_INET, netmask, ifr.ifr_addr.sa_data + 2);
  ioctl(fd, SIOCSIFNETMASK, &ifr);

  //get flags
  ioctl(fd, SIOCGIFFLAGS, &ifr);  
  strncpy(ifr.ifr_name, name, IFNAMSIZ);
  ifr.ifr_flags |= (IFF_UP | IFF_RUNNING);
  // set flags
  ioctl(fd, SIOCSIFFLAGS, &ifr);

  return 0;
}

int run_sh(void* arg)
{
  execlp("/bin/sh", "/bin/sh" , NULL);
  return EXIT_SUCCESS;
}

int child(void* arg)
{
  char c;
  sleep(1);
  sethostname("myhost", 6);

  int ret = mount("/dev/loop30", "/home", "ext4", MS_NOATIME, "");    
  if (ret != 0) {
    perror("Failed to mount virtual storage device");
    exit(EXIT_FAILURE);
  }

  chdir("/home");
  setip("veth1","10.0.0.15","255.0.0.0");

  static char sh_stack[STACK_SIZE];
  pid_t sh_pid = clone(run_sh, sh_stack+STACK_SIZE, SIGCHLD, NULL);
  waitpid(sh_pid, NULL, 0);

  return 0;
}

int main()
{
  system("sudo mount --make-private -o remount /");
  char buf[255];
  pid_t pid = clone(child, stack+STACK_SIZE,
      CLONE_NEWNET | CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);

  printf("child process: %ld\n", (long) pid);

  sprintf(buf,"sudo ip link add name veth0 type veth peer name veth1 netns %d",pid);
 
  system(buf);
  setip("veth0","10.0.0.13","255.0.0.0");


  waitpid(pid, NULL, 0);
  return 0;
}