
[github repo](https://github.com/simply-pleb/Simple-Container)

# Simple-Container

**Requirements**:

1. Create your own container that is able to run processes via command line using clone().
2. Isolate created container by creating new namespaces for PID, mount and networking.
3. Set up a network connection between parent namespace and child namespace.
4. Put container filesystem in a file by using ​loop ​and try to create file in your new mount point.
5. Run processor listing, network information and benchmark in your container and Docker guest containers.
6. Compare results and write a brief report about comparison results.

## 1-2. Creating an isolated container using clone()

```c
pid_t pid = clone(child, stack+STACK_SIZE,
      CLONE_NEWNET | CLONE_NEWUTS | CLONE_NEWIPC | 
      CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);

printf("child process: %ld\n", (long) pid);
```

## 3. Setting up network connection between parent namespace and child namespace

Currently sending pings from the parent process successfully transmits and receives packages, but the child cannot.

## 4. Put container filesystem in a file by using ​loop ​and try to create file in your new mount point.

```sh
$ sudo create_loop_device.sh
```

will create a loop device with ext4 formatting.

To mount it on our container, we use

```c
int ret = mount("/dev/loop30", "/home", "ext4", MS_NOATIME, "");    
  if (ret != 0) {
    perror("Failed to mount virtual storage device");
    exit(EXIT_FAILURE);
  }

  chdir("/home");

```

## 5.  Run processor listing, network information and benchmark in your container and Docker guest containers.

WIP

## 6. Compare results and write a brief report about comparison results.

